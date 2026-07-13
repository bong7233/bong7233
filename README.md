C++/Qt와 Python을 중심으로 AGV/AMR, 스마트팩토리, 현장 엔지니어링 도구를 개발합니다.

현재는 로봇 소프트웨어 엔지니어로 일하며, AGV/AMR 제어 구조, 장비 통신, 운영 UI, ROS 2, FAE/솔루션 엔지니어링 도구를 포트폴리오로 확장하고 있습니다.

- 포지션 : 로봇 애플리케이션 엔지니어
- 분  야 : AGV/AMR/RobotArm, ROS, C++/Qt, Python, 스마트팩토리, 물류 자동화
- 이메일 : [batmantwo7233@gmail.com](mailto:batmantwo7233@gmail.com)
- 휴대폰 : 010-4669-7233

## Focus

### 로봇 제어 프로그램 구조화

안전 상태, 장치 상태, 운영 모드, 알람, 로그, 작업 지시, 운영자 UI가 함께 맞물려야 실제 현장에서 설명 가능한 프로그램이 됩니다.
UI와 제어 로직을 분리하고, 장치 드라이버를 모듈화하며, mock 장비나 simulator로 하드웨어 없이도 흐름을 검증할 수 있는 구조를 설계합니다.

### 엔지니어링 도구 개발

Serial, TCP, UDP, CAN 통신 테스트 도구, Modbus 프레임 송수신, 티칭 포인트 관리, 알람 분석, 점검 체크리스트 같은 기능을 개발합니다. 코드를 작성하는 능력뿐 아니라 현장에서 바로 사용할 수 있는 도구를 설계하는 능력을 가지고 있습니다.

### 스마트 물류 자동화 이해

작업 배정, 관제, 알람 대응, 리포트, ROI, 고객 제안, 시운전 체크리스트까지 연결시켜서
단일 기능보다 운영 모니터링에서 고객 보고까지 이어지는 전체 흐름을 구현했습니다

## 진행중인 포트폴리오

**AGV, AMR, RobotArm Middleware 개발 경력과 현장경험을 토대로 개인 포트폴리오로 재기획하고 AI Tool(ClaudeCode && Codex)을 활용**해 구현했습니다. 
AI Tool이 작성한 모든 코드를 리팩토링하며 학습목적으로도 사용중입니다.

---

### 1. FleetOps 스마트팩토리 AMR 통합 운영 콘솔

라이브 데모: [AMR 통합 모니터링 도구](https://bongfae-production.up.railway.app/#about)

가상의 스마트팩토리에서 운영되는 AMR fleet을 모니터링하고, 알람을 분석하고, VDA5050 메시지를 확인하고, 고객 보고서와 제안 자료를 생성하는 통합 웹 콘솔입니다.

- AMR 위치, 상태, 배터리, 작업 상태 실시간 관제
- 알람 이력, 패턴, 재발 원인, 예방정비 우선순위 분석
- VDA5050 기반 상위 관제 메시지 모니터링
- 도면 기반 티칭 맵 관리
- 일일/주간 운영 리포트 생성
- AGV/AMR/OHT 장비 선정 추천
- ROI와 도입 효과 예측

---

### 2.Robot Toolkit

레포지토리: [Robot_Toolkit](https://github.com/bong7233/FAE_Toolkit_Bong)

로봇의 통신을 테스트하고, 티칭 데이터를 관리할 수 있도록 만든 크로스플랫폼 도구 모음입니다.

이 프로젝트는 실제 장비 연동 업무를 위해 만들었습니다. 하드웨어가 없을 때는 가상 시리얼 페어, 로컬 에코, 디바이스 에뮬레이터로 검증할 수 있게 구성했습니다.

- Serial, TCP, UDP, CAN 통신 테스트
- HEX/ASCII 프레임 송수신
- Modbus CRC-16 자동 첨부와 수신 프레임 디코딩
- 자주 쓰는 프레임 저장과 불러오기
- BMS, IO, CAN 시뮬레이터
- TeachingManager를 통한 티칭 포인트와 상태 관리
- C++ 코어와 Python 바인딩 검증
- Windows와 Linux 대상 CI 구성

---

### 3. Cpp_Qt_ClaudeCode AGV Control System

레포지토리: [Cpp_Qt_ClaudeCode](https://github.com/bong7233/Cpp_Qt_ClaudeCode)

C++20과 Qt를 기반으로 한 AGV 제어 시스템 실험 프로젝트입니다. Linux 차량 PC 환경을 가정하고, 수동 운전 UI와 상위 시스템의 자동 명령을 모두 처리하는 SI 프로그램 형태로 구성되어 있습니다.

구조적으로는 Qt에 의존하지 않는 core를 두고, 그 위에 선택적인 Qt UI를 붙일 수 있게 되어 있습니다. 하드웨어 모듈은 simulator backend와 real backend를 분리하는 방향이며, 실제 장비 없이도 end-to-end로 동작을 검증할 수 있는 구조를 목표로 합니다.

- Idle, Moving, Charging, Paused, Error, EmergencyStop 기반 safety state machine
- Qt UI 수동 조작과 fleet/supervisory system 명령 처리
- mission sequencer와 autonomous task 흐름
- Battery, Motor, IO, LiDAR 모듈 구조
- EventBus 기반 상태, 알람, 로그 전달
- SQLite 기반 fault history 저장 구조
- watchdog 기반 data freshness 감시
- systemd, Docker 기반 배포 흐름

---

### 4. Ros_Copilot

레포지토리: [Ros_Copilot](https://github.com/bong7233/Ros_Copilot)

AI 스택(RAG / AI Agent / LLM Wiki) 을 한 프로젝트 안에서 동시에 증명하는 것을 목표로 합니다. LLM × Robotics, embodied agent를 제작합니다.

Gazebo 시뮬레이션 위에서 돌아가는 ROS2 모바일 로봇을 두고, 자연어로 질문하고 · 명령하고 · 문서화하는 시스템입니다.


- 로봇: ROS2 (Humble/Jazzy), Nav2, Gazebo, tf2, colcon
- 언어: C++17 (rclcpp), Python 3.10+ (rclpy)
- LLM: Claude API (Opus / Sonnet)
- RAG: 임베딩 + 벡터DB (pgvector 또는 Chroma)

---

### 5. Self Made AMR

레포지토리 : Coming soon... ( 추후 프로젝트 완성 후 일괄공개 예정 )

직접 AMR 부품 선정 및 구입, 임베디드, ROS 개발까지 진행하는 프로젝트입니다. 
주요 하드웨어는 NVIDIA JetsonOrin + STM 보드이며
SLAM, 음성인식, 물체인식 기능까지 구현 진행 중 입니다.
