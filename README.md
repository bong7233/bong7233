# 이상봉 | Robot Software Engineer

C++/Qt와 Python으로 로봇·스마트팩토리 현장의 문제를 제품 형태로 풀어내는 개발자입니다.  
AGV/AMR 제어, 장비 통신, 운영 모니터링, 현장 디버깅 도구처럼 **실제 현장에서 설명 가능하고 시연 가능한 소프트웨어**를 만드는 데 집중하고 있습니다.

- Robot S/W Engineer @ Zenix Robotics
- Main interests: AGV/AMR, ROS 2, C++/Qt, Python, FAE tools, smart logistics automation
- Location: Gyeonggi-do, Korea
- Portfolio: [FleetOps — 스마트팩토리 AMR 통합 운영 콘솔](https://bongfae-production.up.railway.app/#about)
- LinkedIn: [Sangbong Lee](https://www.linkedin.com/in/%EC%83%81%EB%B4%89-%EC%9D%B4-775580203/)
- Email: [batmantwo7233@gmail.com](mailto:batmantwo7233@gmail.com)

## Focus

- **AGV/AMR control software**: safety state machine, device driver structure, operator UI, diagnostics
- **Field engineering tools**: Serial/TCP/UDP/CAN communication testing, Modbus frame handling, equipment teaching data management
- **ROS 2 robot architecture**: lifecycle, launch/config, mock drivers, diagnostics, Gazebo/RViz simulation, Qt UI integration
- **Smart factory operations**: fleet monitoring, VDA5050/MQTT message flow, alarm analytics, reporting, commissioning checklist
- **Reliable delivery**: CMake, colcon, pytest, ruff, GitHub Actions, Docker, SQLite

## Featured Projects

| Project | What it shows | Stack |
| --- | --- | --- |
| [FAE Toolkit](https://github.com/bong7233/FAE_Toolkit_Bong) | 현장 엔지니어용 Serial/TCP/UDP/CAN 통신 테스트 도구와 TeachingManager. 실제 연결, 프레임 송수신, Modbus 디코딩, 장비 없이도 검증 가능한 에뮬레이터를 포함합니다. | Python, PySide6, C++17, pybind11, Serial/TCP/UDP/CAN, Modbus, ROS 2 bridge, GitHub Actions |
| [ROS2_Prac](https://github.com/bong7233/ROS2_Prac) | Linux PC 기반 AMR 제어 프로그램을 ROS 2 구조로 재설계한 포트폴리오. mock BMS/IO/motor driver, safety monitor, base controller, system manager, Qt operator UI, Gazebo simulation을 포함합니다. | C++17, ROS 2 Jazzy, Qt 6, Gazebo, colcon, ament_cmake |
| [FleetOps Live Portfolio](https://bongfae-production.up.railway.app/#about) | 스마트팩토리 AMR 운영 콘솔 라이브 데모. 관제, 티칭 맵, 알람 분석, VDA5050 모니터, 리포트 생성, ROI/장비 선정, 점검 체크리스트까지 한 흐름으로 보여줍니다. | Python, FastAPI, WebSocket, MQTT/VDA5050, SQLite, Pandas, Chart.js, Docker |
| [Smart Logistics Solution Designer](https://github.com/bong7233/Solution_Test) | 고객 요구사항을 입력받아 AGV/AMR 솔루션 추천, Cycle Time/처리량 계산, 리스크 분석, 제안서 초안을 생성하는 Streamlit 포트폴리오 앱입니다. | Python, Streamlit, pandas, pydantic, plotly, SQLite, pytest |
| [C++/Qt AGV Core](https://github.com/bong7233/Cpp_Qt_Codex) | UI와 하드웨어 벤더에 독립적인 AGV 제어 코어 설계. Serial/CAN/TCP 어댑터, 안전/모션 로직 분리, Qt 운영 UI 확장을 목표로 합니다. | C++20, CMake, Qt 6, CTest |
| [AI Speaking Tool](https://github.com/bong7233/AI_Speaking_Tool) | 영어/중국어 말하기 훈련용 웹앱. STT 이해 확인, 답변 품질 평가, 자연화 문장 제안, TTS 복습 흐름을 구현했습니다. | Python, Streamlit, OpenAI API |

## How I Work

- 현장 장비가 없어도 **mock, simulator, emulator**로 재현 가능한 구조를 먼저 만듭니다.
- UI, 장치 통신, 제어 로직을 분리해서 테스트와 설명이 가능한 구조를 선호합니다.
- 단순 코드보다 **문서, 시연 흐름, 검증 방법**까지 함께 정리해 포트폴리오 완성도를 높입니다.
- 로봇 소프트웨어와 FAE/솔루션 엔지니어링 사이를 연결하는 개발자가 되는 것이 목표입니다.

## Current Direction

스마트 물류·로봇 자동화 분야에서 C++/Qt, Python, ROS 2, Linux robot PC, 산업용 통신, 운영/진단 도구 역량을 깊게 쌓고 있습니다.  
면접이나 기술 설명에서는 "무엇을 만들었는가"보다 **왜 이렇게 구조화했고, 현장에서는 어떻게 검증할 수 있는가**를 중심으로 설명하려고 합니다.
