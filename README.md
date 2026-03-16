# ActionGame

Unreal Engine 5.4 기반 3인칭 액션 게임 캐릭터 시스템 프로젝트

## 개요

트리플 A 게임 수준의 캐릭터 조작감, 애니메이션 전환, 이동 처리를 목표로 하는 프로젝트입니다.

이전 프로젝트 Locomotion에서 부족했던 부분들을 보완하며, 더 고도화된 캐릭터 시스템을 구현합니다.

## 목표

- 자연스러운 캐릭터 조작감 — 카메라 기준 8방향 이동, 제자리 턴, 감속 정지 등 세밀한 이동 처리
- 애니메이션 품질 향상 — Blend Space, 상태 머신 기반의 매끄러운 애니메이션 전환 및 연속 턴 시스템
- IK(Inverse Kinematics) 도입 — 지형 적응형 발 배치 등 물리 기반 애니메이션 보정
- 인터페이스 활용 — 클래스 간 결합도를 낮추고 확장성을 높이는 인터페이스 설계
- UDataAsset 기반 데이터 관리 — 자주 변동되는 수치를 데이터 테이블로 분리하여, 단일 클래스에서 일괄 관리

## 기술 스택

| 항목 | 내용 |
|------|------|
| Engine | Unreal Engine 5.4 |
| Language | C++ |
| Input System | Enhanced Input |
| Animation | AnimInstance, Blend Space 2D, State Machine |
| Version Control | Git (GitHub) |

## 프로젝트 구조

```
Source/ActionGame/
├── MainCharacter.h / .cpp             // 캐릭터 이동, 회전, 입력 처리
├── MainCharacterAnimInstance.h / .cpp // 애니메이션 상태 제어
└── ActionGame.Build.cs                // 모듈 의존성 관리
```

## 기능

### 구현 완료

- [x] SpringArm + Camera 3인칭 카메라 시스템
- [x] Enhanced Input 기반 Move / Look / Sprint 입력 처리
- [x] 카메라 기준 WASD 8방향 이동 (Blend Space 2D)
- [x] 제자리 턴 시스템 (90° 단위, 연속 턴 지원, Force Root Lock)
- [x] 걷기 / 달리기 전환 (Shift 토글)
- [x] WalkStop / RunStop 감속 정지 애니메이션

### 예정

- [ ] IK 기반 발 배치 시스템
- [ ] 회피 시스템
- [ ] 패링 시스템
- [ ] UDataAsset 데이터 테이블 도입
- [ ] 인터페이스 기반 상호작용 시스템

## 브랜치 전략 (예시)

```
main
└── Dev
    ├── MainCharacter
    │   ├── feat/MainCharacter_Move
    │   └── feat/MainCharacter_Die
    └── Weapon
        ├── feat/Weapon_Raycast
        ├── feat/Weapon_Endpoint
        ├── update/Weapon_Raycast
        └── bugfix/Weapon_Raycast
```

- `main` — 안정 빌드 (PR 머지만 허용, 직접 push 금지)
- `Dev` — 개발 통합 브랜치 (feat 브랜치들의 머지 대상)
- `feat/` — 기능 단위 브랜치
- `update/` — 기존 기능 개선
- `bugfix/` — 버그 수정
- 브랜치 네이밍: `타입/카테고리_기능` (슬래시 구분, 언더스코어로 단어 연결)
- 머지 완료된 브랜치는 수명 종료 — 추가 작업 시 새 브랜치 생성

## 작업 흐름

1. **이슈 발행** — 작업 목적과 TODO 리스트 작성
2. **브랜치 생성** — 이슈에서 발행한 클래스의 기능마다 별도 브렌치 생성
3. **구현** — 커밋 단위로 push
4. **PR 요청** — 이슈 번호 연동 (`[#이슈번호] 제목`)
5. **머지** — PR 승인 후 Dev에 머지, 브랜치 수명 종료

## 커밋 컨벤션

커밋 메시지 형식: `tag: 제목`

| 태그 | 설명 |
|------|------|
| `feat` | 새로운 기능 추가 |
| `fix` | 버그 수정 |
| `refactor` | 코드 리팩토링 |
| `chore` | 빌드, 설정 변경 |
| `docs` | 문서 수정 |
| `style` | 코드 스타일 변경 (동작 변경 없음) |
| `update` | 기존 기능 개선 |
| `rename` | 파일/폴더명 변경 |
| `remove` | 코드/파일 삭제 |

## 이슈 컨벤션

- 제목: 명확하고 간결하게
- 본문: 작업 목적 + TODO 체크리스트
- 라벨: `Feature`, `Bug`, `Enhancement` 등 유형 지정

## PR 컨벤션

- 제목: `[#이슈번호] 작업 내용 요약`
- 본문: 변경 사항 요약 + 테스트 내용
- 머지 대상: `Dev` 브랜치

## 이전 프로젝트와의 차이점

이 프로젝트는 Locomotion 프로젝트의 후속작으로, 아래 항목들을 개선합니다:

- 코드 기반 캡슐 회전으로 애니메이션-물리 동기화 문제 해결
- 데이터 주도 설계 (UDataAsset) 적용
- 인터페이스 기반 아키텍처로 확장성 확보
