# Teaching Status Monitor (C++ / Qt Widgets)

로봇 스테이션 티칭 진행 상태를 지도 기반으로 모니터링/관리하기 위한 학습용 데스크톱 프로그램입니다.

## 핵심 목표
- 맵(이미지) 위에서 노드/주행로/스테이션을 직접 배치
- 스테이션의 층별(1F/2F/3F) 티칭 상태 관리
- 상태(완료/보완 필요/미티칭/반복 에러) 색상으로 한눈에 모니터링
- `N회 이상 알람` 조건으로 반복 에러 후보 스테이션 필터링
- 프로젝트 데이터 Import/Export (JSON + SQLite)

## 기술 스택
- C++17
- Qt 6.6 LTS (Widgets, SQL)
- CMake
- SQLite

## 주요 기능
1. **맵 기반 편집**
   - 배경 맵 이미지(PNG/JPG) 로드
   - 노드 추가/이동
   - 노드 간 주행로 연결
   - 스테이션 추가 및 최근접 노드 자동 연결

2. **층별 티칭 상태 관리**
   - 층 선택 후 상태 변경
   - 상태: Completed / Needs Improvement / Not Taught / Repeated Error
   - 알람 카운트 저장

3. **모니터링 필터**
   - 층 필터: All, 1F, 2F, 3F
   - 상태 필터: All 포함 4개 상태
   - 알람 필터: `Min Alarm(N)`

4. **저장/불러오기**
   - JSON Import/Export (세부 좌표/상태 포함)
   - SQLite Load/Save

## 빌드
```bash
cmake -S . -B build
cmake --build build
```

> Qt 6.6 개발 환경이 설치되어 있어야 합니다. (`Qt6Config.cmake` 탐색 가능 상태)

## 실행 후 기본 사용 순서
1. `File > Load Map Image`로 배경 맵 업로드
2. `Edit Mode > Add Node`로 노드 배치
3. `Edit Mode > Connect Path`로 노드 간 주행로 연결
4. `Edit Mode > Add Station`에서 스테이션 번호(예: H3-1) 입력 후 배치
5. 우측 Dock에서 층 선택 + 상태/알람 수정
6. 필터 조건으로 층별/상태별 진행 현황 모니터링
7. JSON 또는 SQLite로 저장

## 향후 확장 아이디어
- 센서/설비 연동 어댑터 모듈 추가
- 실시간 이벤트 스트림 기반 관제 패널
- 상태 변경 이력 뷰(타임라인)
- Linux 배포를 위한 CI 빌드 파이프라인
