# 이상봉

## 로봇·스마트팩토리 현장의 문제를 소프트웨어로 풀어내는 개발자

안녕하세요. C++/Qt와 Python을 중심으로 AGV/AMR, 스마트팩토리, 현장 엔지니어링 도구를 만들고 있는 이상봉입니다.

제가 관심을 두는 개발은 단순히 화면을 만들거나 기능을 붙이는 일이 아니라, 실제 현장에서 생기는 문제를 소프트웨어 구조로 정리하는 일입니다. 장비와 통신하고, 상태를 모니터링하고, 알람을 분석하고, 작업자가 판단할 수 있는 화면과 리포트를 만드는 흐름에 집중하고 있습니다.

현재는 로봇 소프트웨어 엔지니어로 일하며, AGV/AMR 제어 구조, 장비 통신, 운영 UI, ROS 2, FAE/솔루션 엔지니어링 도구를 포트폴리오로 확장하고 있습니다.

- 현재 역할: 로봇 소프트웨어 엔지니어
- 관심 분야: AGV/AMR, ROS 2, C++/Qt, Python, 산업용 통신, 스마트 물류 자동화
- 목표 직무: 로봇 소프트웨어, FAE, 솔루션 엔지니어, 스마트팩토리 자동화 엔지니어
- 포트폴리오 사이트: [FleetOps 스마트팩토리 AMR 통합 운영 콘솔](https://bongfae-production.up.railway.app/#about)
- 링크드인: [Sangbong Lee](https://www.linkedin.com/in/%EC%83%81%EB%B4%89-%EC%9D%B4-775580203/)
- 이메일: [batmantwo7233@gmail.com](mailto:batmantwo7233@gmail.com)

## 제가 집중하는 문제

### 로봇 제어 프로그램 구조화

AGV/AMR 제어 프로그램은 단순히 로봇을 움직이는 코드만으로는 완성되지 않는다고 생각합니다. 안전 상태, 장치 상태, 운영 모드, 알람, 로그, 작업 지시, 운영자 UI가 함께 맞물려야 실제 현장에서 설명 가능한 프로그램이 됩니다.

그래서 포트폴리오에서도 UI와 제어 로직을 분리하고, 장치 드라이버를 모듈화하며, mock 장비나 simulator로 하드웨어 없이도 흐름을 검증할 수 있는 구조를 선호합니다.

### 현장 엔지니어링 도구 개발

FAE나 필드 엔지니어 업무에서는 장비가 정상인지 빠르게 확인하고, 고객에게 현재 상태를 설명하고, 문제를 재현하는 능력이 중요합니다.

Serial, TCP, UDP, CAN 통신 테스트 도구, Modbus 프레임 송수신, 티칭 포인트 관리, 알람 분석, 점검 체크리스트 같은 기능을 포트폴리오에 넣는 이유도 여기에 있습니다. 코드를 작성하는 능력뿐 아니라 현장에서 바로 사용할 수 있는 도구를 설계하는 능력을 보여주고 싶습니다.

### 스마트 물류 자동화 이해

스마트팩토리와 물류 자동화에서는 로봇 한 대보다 전체 운영 흐름이 더 중요할 때가 많습니다. 작업 배정, 관제, 알람 대응, 리포트, ROI, 고객 제안, 시운전 체크리스트까지 연결되어야 실제 프로젝트처럼 보입니다.

그래서 단일 기능보다 운영 모니터링에서 고객 보고까지 이어지는 전체 흐름을 구현하려고 합니다.

## 대표 포트폴리오

아래 프로젝트들은 제가 로봇·스마트팩토리·현장 엔지니어링 방향으로 직접 포트폴리오의 중심에 두고 관리하는 프로젝트입니다.

### 1. FleetOps 스마트팩토리 AMR 통합 운영 콘솔

라이브 데모: [bongfae-production.up.railway.app](https://bongfae-production.up.railway.app/#about)

가상의 스마트팩토리에서 운영되는 AMR fleet을 모니터링하고, 알람을 분석하고, VDA5050 메시지를 확인하고, 고객 보고서와 제안 자료를 생성하는 통합 웹 콘솔입니다.

이 프로젝트는 제가 가장 앞에 보여주고 싶은 포트폴리오입니다. 단순한 CRUD 앱이 아니라, 현장 운영자가 실제로 사용할 법한 흐름을 하나의 제품처럼 구성했습니다.

주요 기능은 다음과 같습니다.

- AMR 위치, 상태, 배터리, 작업 상태 실시간 관제
- 알람 이력, 패턴, 재발 원인, 예방정비 우선순위 분석
- VDA5050 기반 상위 관제 메시지 모니터링
- 도면 기반 티칭 맵 관리
- 일일/주간 운영 리포트 생성
- AGV/AMR/OHT 장비 선정 추천
- ROI와 도입 효과 예측
- 현장 점검 및 커미셔닝 체크리스트

사용 기술은 Python, FastAPI, WebSocket, MQTT/VDA5050, SQLite, Pandas, Chart.js, Docker입니다.

### 2. FAE Toolkit

레포지토리: [FAE_Toolkit_Bong](https://github.com/bong7233/FAE_Toolkit_Bong)

현장 엔지니어가 장비 통신을 테스트하고, 티칭 데이터를 관리할 수 있도록 만든 크로스플랫폼 도구 모음입니다.

이 프로젝트는 실제 장비 연동 업무를 상상하면서 만들었습니다. 통신 포트나 소켓이 없으면 성공한 척하지 않고 실패하도록 만들고, 하드웨어가 없을 때는 가상 시리얼 페어, 로컬 에코, 디바이스 에뮬레이터로 검증할 수 있게 구성했습니다.

주요 기능은 다음과 같습니다.

- Serial, TCP, UDP, CAN 통신 테스트
- HEX/ASCII 프레임 송수신
- Modbus CRC-16 자동 첨부와 수신 프레임 디코딩
- 자주 쓰는 프레임 저장과 불러오기
- BMS, IO, CAN 시뮬레이터
- TeachingManager를 통한 티칭 포인트와 상태 관리
- C++ 코어와 Python 바인딩 검증
- Windows와 Linux 대상 CI 구성

사용 기술은 Python, PySide6, C++17, pybind11, Serial/TCP/UDP/CAN, Modbus, GitHub Actions입니다.

### 3. ROS2_Prac

레포지토리: [ROS2_Prac](https://github.com/bong7233/ROS2_Prac)

Linux PC 기반 AMR 제어 프로그램을 ROS 2 구조로 다시 설계하는 포트폴리오입니다.

이 프로젝트의 핵심은 ROS 2 튜토리얼을 모아두는 것이 아니라, 실제 AMR 제어 프로그램에 필요한 책임을 노드와 패키지로 나누는 것입니다. 배터리, IO, 모터, 안전 감시, 베이스 제어, 시스템 상태, 운영 UI를 분리하고, mock hardware로 전체 흐름을 검증할 수 있게 구성했습니다.

주요 기능은 다음과 같습니다.

- ROS 2 Jazzy 기반 AMR 패키지 구조
- mock BMS, IO, motor driver node
- safety monitor와 cmd_vel safety gate
- differential drive base controller와 odometry
- system manager와 robot state 관리
- Qt operator UI 연동
- Gazebo와 RViz 기반 시뮬레이션
- diagnostics와 rosbag 기반 디버깅 흐름

사용 기술은 C++17, ROS 2 Jazzy, Qt 6, Gazebo, colcon, ament_cmake입니다.

## AI 도구 활용 실험 프로젝트

아래 프로젝트들은 일반적인 의미의 대표 포트폴리오라기보다, **제가 직접 코드를 손으로 수정하지 않고 AI 개발 도구만 사용해서 어느 정도 자연스러운 프로그램을 만들 수 있는지 훈련하는 프로젝트**입니다.

목적은 코딩 실력을 과장하는 것이 아니라, AI에게 요구사항을 정확히 전달하고, 생성된 결과를 제품 관점에서 평가하고, 프로그램 구조와 기능 완성도를 끌어올리는 능력을 익히는 것입니다. 이후에도 이 프로젝트들은 제가 직접 코드를 수정하기보다 AI 도구를 활용해 개선하는 방식으로 관리할 계획입니다.

### 4. Cpp_Qt_ClaudeCode AGV Control System

레포지토리: [Cpp_Qt_ClaudeCode](https://github.com/bong7233/Cpp_Qt_ClaudeCode)

C++20과 Qt를 기반으로 한 AGV 제어 시스템 실험 프로젝트입니다. Linux 차량 PC 환경을 가정하고, 수동 운전 UI와 상위 시스템의 자동 명령을 모두 처리하는 SI 프로그램 형태로 구성되어 있습니다.

이 프로젝트는 AI 도구를 사용해 AGV 제어 시스템의 구조를 어디까지 자연스럽게 설계하고 구현할 수 있는지 확인하기 위한 목적이 큽니다. 제가 직접 코드를 계속 다듬는 프로젝트라기보다, AI와 함께 요구사항을 정리하고 결과물을 검토하는 훈련용 프로젝트입니다.

구조적으로는 Qt에 의존하지 않는 core를 두고, 그 위에 선택적인 Qt UI를 붙일 수 있게 되어 있습니다. 하드웨어 모듈은 simulator backend와 real backend를 분리하는 방향이며, 실제 장비 없이도 end-to-end로 동작을 검증할 수 있는 구조를 목표로 합니다.

주요 기능은 다음과 같습니다.

- Idle, Moving, Charging, Paused, Error, EmergencyStop 기반 safety state machine
- Qt UI 수동 조작과 fleet/supervisory system 명령 처리
- mission sequencer와 autonomous task 흐름
- Battery, Motor, IO, LiDAR 모듈 구조
- EventBus 기반 상태, 알람, 로그 전달
- SQLite 기반 fault history 저장 구조
- watchdog 기반 data freshness 감시
- systemd, Docker 기반 배포 흐름

사용 기술은 C++20, CMake, Qt, SQLite, SocketCAN, systemd, Docker입니다.

### 5. AI Speaking Tool

레포지토리: [AI_Speaking_Tool](https://github.com/bong7233/AI_Speaking_Tool)

영어와 중국어 말하기 훈련을 위한 Streamlit 웹앱입니다.

이 프로젝트도 제가 직접 코드를 작성하거나 이후에 수동으로 수정하기 위한 프로젝트가 아닙니다. AI 도구만 사용해서 사용자가 자연스럽게 쓸 수 있는 학습용 프로그램을 어느 수준까지 만들 수 있는지 실험하고, 요구사항을 AI에게 전달하는 능력을 훈련하기 위한 프로젝트입니다.

로봇 분야 포트폴리오와 직접 연결되는 프로젝트는 아니지만, AI 기반 개발 도구를 활용해 작은 사용자용 앱을 빠르게 구성하고 개선하는 연습으로 남겨두고 있습니다.

주요 기능은 다음과 같습니다.

- 사용자의 답변과 질문의 관련성 평가
- STT가 사용자의 답변을 어떻게 이해했는지 표시
- 답변 품질 점수화
- 자연스러운 표현 제안
- 중국어 병음, 뜻, 발음 주의점 제공
- 복습 카드와 약점 누적 저장
- 교정 문장 TTS 재생

사용 기술은 Python, Streamlit, OpenAI API입니다.

## 기술 스택

주로 사용하는 기술은 다음과 같습니다.

- 언어: C++, Python, JavaScript
- 로봇/제어: ROS 2, Gazebo, RViz, Qt operator UI, diagnostics, rosbag
- GUI: Qt Widgets, PySide6, Streamlit, 웹 기반 대시보드
- 통신: Serial, TCP, UDP, CAN, Modbus, MQTT, VDA5050
- 백엔드/데이터: FastAPI, SQLite, Pandas, WebSocket
- 빌드/검증: CMake, colcon, pytest, ruff, GitHub Actions, Docker
- AI 도구 활용: AI 기반 코드 생성, 요구사항 프롬프팅, 결과물 검토, 기능 개선 방향 설계

## 개발할 때 중요하게 보는 것

저는 포트폴리오를 만들 때 화면이 그럴듯한지만 보지 않습니다. 다음 질문에 답할 수 있는 구조인지 확인하려고 합니다.

- 실제 장비가 없을 때도 어떻게 검증할 수 있는가?
- UI와 핵심 로직이 분리되어 있는가?
- 문제가 생겼을 때 로그, 알람, 상태로 원인을 추적할 수 있는가?
- 면접에서 기능을 단순히 보여주는 것을 넘어 설계 이유를 설명할 수 있는가?
- 고객이나 현장 담당자가 이해할 수 있는 문서와 시연 흐름이 준비되어 있는가?
- AI 도구로 만든 프로젝트라면, 내가 직접 구현한 코드와 AI 활용 실험 결과를 명확히 구분하고 있는가?

## 현재 방향

현재는 스마트 물류 자동화와 로봇 소프트웨어 분야에 맞춰 포트폴리오를 정리하고 있습니다.

특히 AGV/AMR 제어 구조, Linux robot PC, ROS 2, C++/Qt 운영 UI, 산업용 통신, FAE 도구, 스마트팩토리 운영 콘솔을 하나의 방향으로 묶고 있습니다. 앞으로도 단순히 많은 레포지토리를 공개하기보다, 설명 가능한 대표 프로젝트를 선별해서 완성도를 높이는 쪽으로 관리할 계획입니다.
