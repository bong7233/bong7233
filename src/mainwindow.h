#pragma once

#include <QComboBox>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>

#include <array>

class NodeItem : public QGraphicsEllipseItem
{
public:
    explicit NodeItem(int nodeId, const QPointF &center);
    [[nodiscard]] int nodeId() const;

private:
    int m_nodeId;
};

class StationItem : public QGraphicsRectItem
{
public:
    enum class TeachingStatus {
        Completed = 0,
        NeedsImprovement = 1,
        NotTaught = 2,
        RepeatedError = 3
    };

    StationItem(int stationId, QString stationNumber, int nodeId, const QPointF &center);

    [[nodiscard]] int stationId() const;
    [[nodiscard]] int nodeId() const;
    [[nodiscard]] const QString &stationNumber() const;
    [[nodiscard]] int alarmCount() const;
    [[nodiscard]] TeachingStatus floorStatus(int floor) const;
    [[nodiscard]] TeachingStatus effectiveStatus(int floorFilter) const;

    void setNodeId(int nodeId);
    void setStationNumber(const QString &stationNumber);
    void setAlarmCount(int alarmCount);
    void setFloorStatus(int floor, TeachingStatus status);

    [[nodiscard]] QColor effectiveColor(int floorFilter) const;

private:
    TeachingStatus mergedStatus() const;

    int m_stationId;
    QString m_stationNumber;
    int m_nodeId;
    int m_alarmCount;
    std::array<TeachingStatus, 3> m_statusByFloor;
};

class PathItem : public QGraphicsLineItem
{
public:
    PathItem(int edgeId, NodeItem *first, NodeItem *second);

    [[nodiscard]] int edgeId() const;
    [[nodiscard]] int firstNodeId() const;
    [[nodiscard]] int secondNodeId() const;
    void refresh();

private:
    int m_edgeId;
    NodeItem *m_first;
    NodeItem *m_second;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void loadMapImage();
    void exportJson();
    void importJson();
    void saveSqlite();
    void loadSqlite();

    void setAddNodeMode();
    void setAddStationMode();
    void setConnectPathMode();
    void setSelectMode();

    void onSceneSelectionChanged();
    void applyStationEditorToSelected();
    void applyFilters();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    enum class EditMode { Select, AddNode, AddStation, ConnectPath };

    void setupUi();
    void setupMenus();
    void setupDock();

    NodeItem *createNode(const QPointF &pos, int forcedId = -1);
    StationItem *createStation(const QPointF &pos, const QString &number, int nodeId, int forcedId = -1);
    PathItem *createPath(NodeItem *a, NodeItem *b, int forcedId = -1);

    void clearProject();
    void refreshPathGeometry();
    void refreshStationTable();
    void refreshStationColors();

    static QString statusToText(StationItem::TeachingStatus status);
    static StationItem::TeachingStatus statusFromIndex(int index);

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_mapItem;

    QTableWidget *m_stationTable;
    QLabel *m_selectedLabel;
    QComboBox *m_floorCombo;
    QComboBox *m_statusCombo;
    QSpinBox *m_alarmSpin;

    QComboBox *m_filterFloorCombo;
    QComboBox *m_filterStatusCombo;
    QSpinBox *m_filterAlarmSpin;

    EditMode m_mode;
    NodeItem *m_pendingPathNode;

    int m_nextNodeId;
    int m_nextStationId;
    int m_nextPathId;

    QList<NodeItem *> m_nodes;
    QList<StationItem *> m_stations;
    QList<PathItem *> m_paths;

    QString m_mapImagePath;
};
