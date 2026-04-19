#include "mainwindow.h"

#include <QAction>
#include <QBrush>
#include <QDockWidget>
#include <QEvent>
#include <QFileDialog>
#include <QFile>
#include <QFormLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHash>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStatusBar>
#include <QTextStream>
#include <QVBoxLayout>

#include <algorithm>

namespace {
QColor colorForStatus(StationItem::TeachingStatus status)
{
    switch (status) {
    case StationItem::TeachingStatus::Completed:
        return QColor("#2E7D32");
    case StationItem::TeachingStatus::NeedsImprovement:
        return QColor("#F9A825");
    case StationItem::TeachingStatus::NotTaught:
        return QColor("#757575");
    case StationItem::TeachingStatus::RepeatedError:
        return QColor("#C62828");
    }
    return QColor("#757575");
}

QString dbConnectionName()
{
    return QStringLiteral("teaching_monitor_db");
}
} // namespace

NodeItem::NodeItem(int nodeId, const QPointF &center)
    : QGraphicsEllipseItem(-12.0, -12.0, 24.0, 24.0)
    , m_nodeId(nodeId)
{
    setPos(center);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setBrush(QBrush(QColor("#1976D2")));
    setPen(QPen(Qt::white, 2));
    setZValue(10);

    auto *label = new QGraphicsTextItem(QString::number(nodeId), this);
    label->setDefaultTextColor(Qt::white);
    label->setPos(-9.0, -11.0);
    label->setScale(0.8);
}

int NodeItem::nodeId() const
{
    return m_nodeId;
}

StationItem::StationItem(int stationId, QString stationNumber, int nodeId, const QPointF &center)
    : QGraphicsRectItem(-18.0, -18.0, 36.0, 36.0)
    , m_stationId(stationId)
    , m_stationNumber(std::move(stationNumber))
    , m_nodeId(nodeId)
    , m_alarmCount(0)
    , m_statusByFloor({ TeachingStatus::NotTaught, TeachingStatus::NotTaught, TeachingStatus::NotTaught })
{
    setPos(center);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2));
    setBrush(colorForStatus(TeachingStatus::NotTaught));
    setZValue(12);

    auto *label = new QGraphicsTextItem(m_stationNumber, this);
    label->setDefaultTextColor(Qt::white);
    label->setPos(-16.0, -8.0);
    label->setScale(0.75);
}

int StationItem::stationId() const
{
    return m_stationId;
}

int StationItem::nodeId() const
{
    return m_nodeId;
}

const QString &StationItem::stationNumber() const
{
    return m_stationNumber;
}

int StationItem::alarmCount() const
{
    return m_alarmCount;
}

StationItem::TeachingStatus StationItem::floorStatus(int floor) const
{
    return m_statusByFloor.at(std::clamp(floor, 0, 2));
}

void StationItem::setNodeId(int nodeId)
{
    m_nodeId = nodeId;
}

void StationItem::setStationNumber(const QString &stationNumber)
{
    m_stationNumber = stationNumber;
}

void StationItem::setAlarmCount(int alarmCount)
{
    m_alarmCount = std::max(0, alarmCount);
}

void StationItem::setFloorStatus(int floor, TeachingStatus status)
{
    m_statusByFloor.at(std::clamp(floor, 0, 2)) = status;
}

QColor StationItem::effectiveColor(int floorFilter) const
{
    return colorForStatus(effectiveStatus(floorFilter));
}

StationItem::TeachingStatus StationItem::effectiveStatus(int floorFilter) const
{
    if (floorFilter >= 0 && floorFilter < 3) {
        return m_statusByFloor.at(floorFilter);
    }
    return mergedStatus();
}

StationItem::TeachingStatus StationItem::mergedStatus() const
{
    if (std::find(m_statusByFloor.cbegin(), m_statusByFloor.cend(), TeachingStatus::RepeatedError) != m_statusByFloor.cend()) {
        return TeachingStatus::RepeatedError;
    }
    if (std::find(m_statusByFloor.cbegin(), m_statusByFloor.cend(), TeachingStatus::NeedsImprovement) != m_statusByFloor.cend()) {
        return TeachingStatus::NeedsImprovement;
    }
    if (std::find(m_statusByFloor.cbegin(), m_statusByFloor.cend(), TeachingStatus::NotTaught) != m_statusByFloor.cend()) {
        return TeachingStatus::NotTaught;
    }
    return TeachingStatus::Completed;
}

PathItem::PathItem(int edgeId, NodeItem *first, NodeItem *second)
    : QGraphicsLineItem()
    , m_edgeId(edgeId)
    , m_first(first)
    , m_second(second)
{
    setPen(QPen(QColor("#4E342E"), 3));
    setZValue(2);
    refresh();
}

int PathItem::edgeId() const
{
    return m_edgeId;
}

int PathItem::firstNodeId() const
{
    return m_first ? m_first->nodeId() : -1;
}

int PathItem::secondNodeId() const
{
    return m_second ? m_second->nodeId() : -1;
}

void PathItem::refresh()
{
    if (!m_first || !m_second) {
        return;
    }
    setLine(QLineF(m_first->scenePos(), m_second->scenePos()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new QGraphicsView(this))
    , m_scene(new QGraphicsScene(this))
    , m_mapItem(new QGraphicsPixmapItem())
    , m_stationTable(nullptr)
    , m_selectedLabel(nullptr)
    , m_floorCombo(nullptr)
    , m_statusCombo(nullptr)
    , m_alarmSpin(nullptr)
    , m_filterFloorCombo(nullptr)
    , m_filterStatusCombo(nullptr)
    , m_filterAlarmSpin(nullptr)
    , m_mode(EditMode::Select)
    , m_pendingPathNode(nullptr)
    , m_nextNodeId(1000)
    , m_nextStationId(1)
    , m_nextPathId(1)
{
    setupUi();
    setupMenus();
    setupDock();

    m_scene->addItem(m_mapItem);
    m_mapItem->setZValue(0);

    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MainWindow::onSceneSelectionChanged);

    statusBar()->showMessage("Ready");
}

void MainWindow::setupUi()
{
    resize(1400, 900);
    setWindowTitle("Teaching Status Monitor");

    m_scene->setSceneRect(0, 0, 2000, 1200);
    m_view->setScene(m_scene);
    m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->viewport()->installEventFilter(this);

    setCentralWidget(m_view);
}

void MainWindow::setupMenus()
{
    auto *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Load Map Image...", this, &MainWindow::loadMapImage);
    fileMenu->addSeparator();
    fileMenu->addAction("Import JSON...", this, &MainWindow::importJson);
    fileMenu->addAction("Export JSON...", this, &MainWindow::exportJson);
    fileMenu->addSeparator();
    fileMenu->addAction("Load SQLite...", this, &MainWindow::loadSqlite);
    fileMenu->addAction("Save SQLite...", this, &MainWindow::saveSqlite);

    auto *editMenu = menuBar()->addMenu("Edit Mode");
    editMenu->addAction("Select", this, &MainWindow::setSelectMode);
    editMenu->addAction("Add Node", this, &MainWindow::setAddNodeMode);
    editMenu->addAction("Add Station", this, &MainWindow::setAddStationMode);
    editMenu->addAction("Connect Path", this, &MainWindow::setConnectPathMode);
}

void MainWindow::setupDock()
{
    auto *dock = new QDockWidget("Station Monitor", this);
    auto *dockWidget = new QWidget(dock);
    auto *layout = new QVBoxLayout(dockWidget);

    auto *filterForm = new QFormLayout();
    m_filterFloorCombo = new QComboBox();
    m_filterFloorCombo->addItems({ "All Floors", "1F", "2F", "3F" });

    m_filterStatusCombo = new QComboBox();
    m_filterStatusCombo->addItems({ "All Status", "Completed", "Needs Improvement", "Not Taught", "Repeated Error" });

    m_filterAlarmSpin = new QSpinBox();
    m_filterAlarmSpin->setRange(0, 9999);
    m_filterAlarmSpin->setValue(0);

    filterForm->addRow("Floor", m_filterFloorCombo);
    filterForm->addRow("Status", m_filterStatusCombo);
    filterForm->addRow("Min Alarm(N)", m_filterAlarmSpin);
    layout->addLayout(filterForm);

    auto *legend = new QLabel("Legend: Green=Completed, Yellow=Needs Improvement, Gray=Not Taught, Red=Repeated Error");
    legend->setWordWrap(true);
    layout->addWidget(legend);

    m_stationTable = new QTableWidget(0, 6);
    m_stationTable->setHorizontalHeaderLabels({ "ID", "No", "Node", "1F", "2F", "3F" });
    m_stationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_stationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_stationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_stationTable, 1);

    m_selectedLabel = new QLabel("Selected: none");
    layout->addWidget(m_selectedLabel);

    auto *editForm = new QFormLayout();
    m_floorCombo = new QComboBox();
    m_floorCombo->addItems({ "1F", "2F", "3F" });

    m_statusCombo = new QComboBox();
    m_statusCombo->addItems({ "Completed", "Needs Improvement", "Not Taught", "Repeated Error" });

    m_alarmSpin = new QSpinBox();
    m_alarmSpin->setRange(0, 9999);

    editForm->addRow("Edit Floor", m_floorCombo);
    editForm->addRow("Teaching Status", m_statusCombo);
    editForm->addRow("Alarm Count", m_alarmSpin);
    layout->addLayout(editForm);

    auto *applyButton = new QPushButton("Apply To Selected Station");
    layout->addWidget(applyButton);

    dockWidget->setLayout(layout);
    dock->setWidget(dockWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(m_filterFloorCombo, &QComboBox::currentIndexChanged, this, &MainWindow::applyFilters);
    connect(m_filterStatusCombo, &QComboBox::currentIndexChanged, this, &MainWindow::applyFilters);
    connect(m_filterAlarmSpin, &QSpinBox::valueChanged, this, &MainWindow::applyFilters);
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyStationEditorToSelected);
}

void MainWindow::loadMapImage()
{
    const QString filePath = QFileDialog::getOpenFileName(this, "Load map image", QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (filePath.isEmpty()) {
        return;
    }

    QPixmap pixmap(filePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Image", "Cannot load image file.");
        return;
    }

    m_mapImagePath = filePath;
    m_mapItem->setPixmap(pixmap);
    m_scene->setSceneRect(m_mapItem->boundingRect());
    statusBar()->showMessage(QString("Map loaded: %1").arg(filePath), 3000);
}

void MainWindow::exportJson()
{
    const QString filePath = QFileDialog::getSaveFileName(this, "Export JSON", QString(), "JSON (*.json)");
    if (filePath.isEmpty()) {
        return;
    }

    QJsonObject root;
    root["mapImagePath"] = m_mapImagePath;

    QJsonArray nodes;
    for (const auto *node : m_nodes) {
        QJsonObject obj;
        obj["id"] = node->nodeId();
        obj["x"] = node->scenePos().x();
        obj["y"] = node->scenePos().y();
        nodes.append(obj);
    }
    root["nodes"] = nodes;

    QJsonArray paths;
    for (const auto *path : m_paths) {
        QJsonObject obj;
        obj["id"] = path->edgeId();
        obj["firstNode"] = path->firstNodeId();
        obj["secondNode"] = path->secondNodeId();
        paths.append(obj);
    }
    root["paths"] = paths;

    QJsonArray stations;
    for (const auto *station : m_stations) {
        QJsonObject obj;
        obj["id"] = station->stationId();
        obj["number"] = station->stationNumber();
        obj["nodeId"] = station->nodeId();
        obj["x"] = station->scenePos().x();
        obj["y"] = station->scenePos().y();
        obj["alarmCount"] = station->alarmCount();
        obj["f1"] = static_cast<int>(station->floorStatus(0));
        obj["f2"] = static_cast<int>(station->floorStatus(1));
        obj["f3"] = static_cast<int>(station->floorStatus(2));
        stations.append(obj);
    }
    root["stations"] = stations;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Export", "Cannot write JSON file.");
        return;
    }

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
    statusBar()->showMessage("JSON export complete", 3000);
}

void MainWindow::importJson()
{
    const QString filePath = QFileDialog::getOpenFileName(this, "Import JSON", QString(), "JSON (*.json)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Import", "Cannot read JSON file.");
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        QMessageBox::warning(this, "Import", "Invalid JSON format.");
        return;
    }

    clearProject();
    const QJsonObject root = doc.object();

    m_mapImagePath = root.value("mapImagePath").toString();
    if (!m_mapImagePath.isEmpty()) {
        QPixmap pixmap(m_mapImagePath);
        if (!pixmap.isNull()) {
            m_mapItem->setPixmap(pixmap);
            m_scene->setSceneRect(m_mapItem->boundingRect());
        }
    }

    QHash<int, NodeItem *> nodeById;

    for (const auto value : root.value("nodes").toArray()) {
        const auto obj = value.toObject();
        auto *node = createNode(QPointF(obj.value("x").toDouble(), obj.value("y").toDouble()), obj.value("id").toInt());
        nodeById.insert(node->nodeId(), node);
    }

    for (const auto value : root.value("paths").toArray()) {
        const auto obj = value.toObject();
        auto *first = nodeById.value(obj.value("firstNode").toInt(), nullptr);
        auto *second = nodeById.value(obj.value("secondNode").toInt(), nullptr);
        if (first && second) {
            createPath(first, second, obj.value("id").toInt());
        }
    }

    for (const auto value : root.value("stations").toArray()) {
        const auto obj = value.toObject();
        auto *station = createStation(QPointF(obj.value("x").toDouble(), obj.value("y").toDouble()),
            obj.value("number").toString(),
            obj.value("nodeId").toInt(),
            obj.value("id").toInt());

        station->setAlarmCount(obj.value("alarmCount").toInt());
        station->setFloorStatus(0, static_cast<StationItem::TeachingStatus>(obj.value("f1").toInt()));
        station->setFloorStatus(1, static_cast<StationItem::TeachingStatus>(obj.value("f2").toInt()));
        station->setFloorStatus(2, static_cast<StationItem::TeachingStatus>(obj.value("f3").toInt()));
    }

    refreshPathGeometry();
    refreshStationColors();
    refreshStationTable();
    applyFilters();
    statusBar()->showMessage("JSON import complete", 3000);
}

void MainWindow::saveSqlite()
{
    const QString filePath = QFileDialog::getSaveFileName(this, "Save SQLite", QString(), "SQLite DB (*.db)");
    if (filePath.isEmpty()) {
        return;
    }

    if (QSqlDatabase::contains(dbConnectionName())) {
        QSqlDatabase::removeDatabase(dbConnectionName());
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", dbConnectionName());
    db.setDatabaseName(filePath);
    if (!db.open()) {
        QMessageBox::warning(this, "SQLite", db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.exec("DROP TABLE IF EXISTS project");
    q.exec("DROP TABLE IF EXISTS nodes");
    q.exec("DROP TABLE IF EXISTS paths");
    q.exec("DROP TABLE IF EXISTS stations");

    if (!q.exec("CREATE TABLE project(map_image TEXT)")) {
        QMessageBox::warning(this, "SQLite", q.lastError().text());
        db.close();
        return;
    }
    q.exec("CREATE TABLE nodes(id INTEGER PRIMARY KEY, x REAL, y REAL)");
    q.exec("CREATE TABLE paths(id INTEGER PRIMARY KEY, first_node INTEGER, second_node INTEGER)");
    q.exec("CREATE TABLE stations(id INTEGER PRIMARY KEY, number TEXT, node_id INTEGER, x REAL, y REAL, alarm_count INTEGER, f1 INTEGER, f2 INTEGER, f3 INTEGER)");

    q.prepare("INSERT INTO project(map_image) VALUES(?)");
    q.addBindValue(m_mapImagePath);
    q.exec();

    db.transaction();

    q.prepare("INSERT INTO nodes(id, x, y) VALUES(?, ?, ?)");
    for (const auto *node : m_nodes) {
        q.bindValue(0, node->nodeId());
        q.bindValue(1, node->scenePos().x());
        q.bindValue(2, node->scenePos().y());
        q.exec();
    }

    q.prepare("INSERT INTO paths(id, first_node, second_node) VALUES(?, ?, ?)");
    for (const auto *path : m_paths) {
        q.bindValue(0, path->edgeId());
        q.bindValue(1, path->firstNodeId());
        q.bindValue(2, path->secondNodeId());
        q.exec();
    }

    q.prepare("INSERT INTO stations(id, number, node_id, x, y, alarm_count, f1, f2, f3) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
    for (const auto *station : m_stations) {
        q.bindValue(0, station->stationId());
        q.bindValue(1, station->stationNumber());
        q.bindValue(2, station->nodeId());
        q.bindValue(3, station->scenePos().x());
        q.bindValue(4, station->scenePos().y());
        q.bindValue(5, station->alarmCount());
        q.bindValue(6, static_cast<int>(station->floorStatus(0)));
        q.bindValue(7, static_cast<int>(station->floorStatus(1)));
        q.bindValue(8, static_cast<int>(station->floorStatus(2)));
        q.exec();
    }

    db.commit();
    db.close();
    statusBar()->showMessage("SQLite save complete", 3000);
}

void MainWindow::loadSqlite()
{
    const QString filePath = QFileDialog::getOpenFileName(this, "Load SQLite", QString(), "SQLite DB (*.db)");
    if (filePath.isEmpty()) {
        return;
    }

    if (QSqlDatabase::contains(dbConnectionName())) {
        QSqlDatabase::removeDatabase(dbConnectionName());
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", dbConnectionName());
    db.setDatabaseName(filePath);
    if (!db.open()) {
        QMessageBox::warning(this, "SQLite", db.lastError().text());
        return;
    }

    clearProject();

    QSqlQuery q(db);
    q.exec("SELECT map_image FROM project LIMIT 1");
    if (q.next()) {
        m_mapImagePath = q.value(0).toString();
        if (!m_mapImagePath.isEmpty()) {
            QPixmap pixmap(m_mapImagePath);
            if (!pixmap.isNull()) {
                m_mapItem->setPixmap(pixmap);
                m_scene->setSceneRect(m_mapItem->boundingRect());
            }
        }
    }

    QHash<int, NodeItem *> nodeById;
    q.exec("SELECT id, x, y FROM nodes");
    while (q.next()) {
        auto *node = createNode(QPointF(q.value(1).toDouble(), q.value(2).toDouble()), q.value(0).toInt());
        nodeById.insert(node->nodeId(), node);
    }

    q.exec("SELECT id, first_node, second_node FROM paths");
    while (q.next()) {
        auto *first = nodeById.value(q.value(1).toInt(), nullptr);
        auto *second = nodeById.value(q.value(2).toInt(), nullptr);
        if (first && second) {
            createPath(first, second, q.value(0).toInt());
        }
    }

    q.exec("SELECT id, number, node_id, x, y, alarm_count, f1, f2, f3 FROM stations");
    while (q.next()) {
        auto *station = createStation(QPointF(q.value(3).toDouble(), q.value(4).toDouble()), q.value(1).toString(), q.value(2).toInt(), q.value(0).toInt());
        station->setAlarmCount(q.value(5).toInt());
        station->setFloorStatus(0, static_cast<StationItem::TeachingStatus>(q.value(6).toInt()));
        station->setFloorStatus(1, static_cast<StationItem::TeachingStatus>(q.value(7).toInt()));
        station->setFloorStatus(2, static_cast<StationItem::TeachingStatus>(q.value(8).toInt()));
    }

    refreshPathGeometry();
    refreshStationColors();
    refreshStationTable();
    applyFilters();

    db.close();
    statusBar()->showMessage("SQLite load complete", 3000);
}

void MainWindow::setAddNodeMode()
{
    m_mode = EditMode::AddNode;
    m_pendingPathNode = nullptr;
    statusBar()->showMessage("Mode: Add Node");
}

void MainWindow::setAddStationMode()
{
    m_mode = EditMode::AddStation;
    m_pendingPathNode = nullptr;
    statusBar()->showMessage("Mode: Add Station");
}

void MainWindow::setConnectPathMode()
{
    m_mode = EditMode::ConnectPath;
    m_pendingPathNode = nullptr;
    statusBar()->showMessage("Mode: Connect Path (choose two nodes)");
}

void MainWindow::setSelectMode()
{
    m_mode = EditMode::Select;
    m_pendingPathNode = nullptr;
    statusBar()->showMessage("Mode: Select/Move");
}

void MainWindow::onSceneSelectionChanged()
{
    auto selected = m_scene->selectedItems();
    StationItem *station = nullptr;
    for (auto *item : selected) {
        if ((station = dynamic_cast<StationItem *>(item))) {
            break;
        }
    }

    if (!station) {
        m_selectedLabel->setText("Selected: none");
        return;
    }

    m_selectedLabel->setText(QString("Selected: %1 (Node %2)").arg(station->stationNumber()).arg(station->nodeId()));
    const int floor = m_floorCombo->currentIndex();
    m_statusCombo->setCurrentIndex(static_cast<int>(station->floorStatus(floor)));
    m_alarmSpin->setValue(station->alarmCount());
}

void MainWindow::applyStationEditorToSelected()
{
    auto selected = m_scene->selectedItems();
    for (auto *item : selected) {
        auto *station = dynamic_cast<StationItem *>(item);
        if (!station) {
            continue;
        }

        station->setFloorStatus(m_floorCombo->currentIndex(), statusFromIndex(m_statusCombo->currentIndex()));
        station->setAlarmCount(m_alarmSpin->value());
    }

    refreshStationColors();
    refreshStationTable();
    applyFilters();
}

void MainWindow::applyFilters()
{
    const int floorFilter = m_filterFloorCombo->currentIndex() - 1;
    const int statusFilter = m_filterStatusCombo->currentIndex() - 1;
    const int minAlarm = m_filterAlarmSpin->value();

    for (auto *station : m_stations) {
        const auto displayStatus = station->effectiveStatus(floorFilter);

        const bool statusOk = (statusFilter < 0) || (static_cast<int>(displayStatus) == statusFilter);
        const bool alarmOk = station->alarmCount() >= minAlarm;
        station->setVisible(statusOk && alarmOk);
    }

    refreshStationColors();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_view->viewport() && event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() != Qt::LeftButton) {
            return false;
        }

        const QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
        auto *clicked = m_scene->itemAt(scenePos, m_view->transform());

        if (m_mode == EditMode::AddNode) {
            createNode(scenePos);
            refreshPathGeometry();
            return false;
        }

        if (m_mode == EditMode::AddStation) {
            bool ok = false;
            const QString stationNumber = QInputDialog::getText(this, "Station Number", "e.g. H3-1", QLineEdit::Normal, QString(), &ok);
            if (!ok || stationNumber.trimmed().isEmpty()) {
                return false;
            }

            int nearestNodeId = -1;
            double nearestDistance = 1e9;
            for (auto *node : m_nodes) {
                const double dist = QLineF(node->scenePos(), scenePos).length();
                if (dist < nearestDistance) {
                    nearestDistance = dist;
                    nearestNodeId = node->nodeId();
                }
            }
            createStation(scenePos, stationNumber.trimmed(), nearestNodeId);
            refreshStationTable();
            refreshStationColors();
            applyFilters();
            return false;
        }

        if (m_mode == EditMode::ConnectPath) {
            auto *node = dynamic_cast<NodeItem *>(clicked);
            if (!node) {
                return false;
            }

            if (!m_pendingPathNode) {
                m_pendingPathNode = node;
                statusBar()->showMessage(QString("First node selected: %1").arg(node->nodeId()));
                return false;
            }

            if (m_pendingPathNode == node) {
                m_pendingPathNode = nullptr;
                statusBar()->showMessage("Path selection canceled");
                return false;
            }

            createPath(m_pendingPathNode, node);
            m_pendingPathNode = nullptr;
            refreshPathGeometry();
            return false;
        }
    }

    if (event->type() == QEvent::MouseMove) {
        refreshPathGeometry();
    }

    return QMainWindow::eventFilter(watched, event);
}

NodeItem *MainWindow::createNode(const QPointF &pos, int forcedId)
{
    const int id = (forcedId >= 0) ? forcedId : m_nextNodeId++;
    m_nextNodeId = std::max(m_nextNodeId, id + 1);

    auto *node = new NodeItem(id, pos);
    m_scene->addItem(node);
    m_nodes.append(node);
    return node;
}

StationItem *MainWindow::createStation(const QPointF &pos, const QString &number, int nodeId, int forcedId)
{
    const int id = (forcedId >= 0) ? forcedId : m_nextStationId++;
    m_nextStationId = std::max(m_nextStationId, id + 1);

    auto *station = new StationItem(id, number, nodeId, pos);
    m_scene->addItem(station);
    m_stations.append(station);
    return station;
}

PathItem *MainWindow::createPath(NodeItem *a, NodeItem *b, int forcedId)
{
    const int id = (forcedId >= 0) ? forcedId : m_nextPathId++;
    m_nextPathId = std::max(m_nextPathId, id + 1);

    auto *path = new PathItem(id, a, b);
    m_scene->addItem(path);
    m_paths.append(path);
    return path;
}

void MainWindow::clearProject()
{
    for (auto *path : m_paths) {
        m_scene->removeItem(path);
        delete path;
    }
    m_paths.clear();

    for (auto *station : m_stations) {
        m_scene->removeItem(station);
        delete station;
    }
    m_stations.clear();

    for (auto *node : m_nodes) {
        m_scene->removeItem(node);
        delete node;
    }
    m_nodes.clear();

    m_nextNodeId = 1000;
    m_nextStationId = 1;
    m_nextPathId = 1;
}

void MainWindow::refreshPathGeometry()
{
    for (auto *path : m_paths) {
        path->refresh();
    }
}

void MainWindow::refreshStationTable()
{
    m_stationTable->setRowCount(m_stations.size());

    int row = 0;
    for (auto *station : m_stations) {
        m_stationTable->setItem(row, 0, new QTableWidgetItem(QString::number(station->stationId())));
        m_stationTable->setItem(row, 1, new QTableWidgetItem(station->stationNumber()));
        m_stationTable->setItem(row, 2, new QTableWidgetItem(QString::number(station->nodeId())));
        m_stationTable->setItem(row, 3, new QTableWidgetItem(statusToText(station->floorStatus(0))));
        m_stationTable->setItem(row, 4, new QTableWidgetItem(statusToText(station->floorStatus(1))));
        m_stationTable->setItem(row, 5, new QTableWidgetItem(statusToText(station->floorStatus(2))));
        ++row;
    }
}

void MainWindow::refreshStationColors()
{
    const int floorFilter = m_filterFloorCombo ? m_filterFloorCombo->currentIndex() - 1 : -1;
    for (auto *station : m_stations) {
        station->setBrush(station->effectiveColor(floorFilter));
    }
}

QString MainWindow::statusToText(StationItem::TeachingStatus status)
{
    switch (status) {
    case StationItem::TeachingStatus::Completed:
        return "Completed";
    case StationItem::TeachingStatus::NeedsImprovement:
        return "Needs Improvement";
    case StationItem::TeachingStatus::NotTaught:
        return "Not Taught";
    case StationItem::TeachingStatus::RepeatedError:
        return "Repeated Error";
    }
    return "Unknown";
}

StationItem::TeachingStatus MainWindow::statusFromIndex(int index)
{
    switch (index) {
    case 0:
        return StationItem::TeachingStatus::Completed;
    case 1:
        return StationItem::TeachingStatus::NeedsImprovement;
    case 2:
        return StationItem::TeachingStatus::NotTaught;
    default:
        return StationItem::TeachingStatus::RepeatedError;
    }
}
