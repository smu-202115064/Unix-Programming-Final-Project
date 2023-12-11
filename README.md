# UNIX Programming 프로젝트 과제

202115064 김동주 (조장)
201810901 김건태

## I. 개요

```
Client -> Load Balancer -> Server
```

UNIX Programming 강의에서 배운 내용을 바탕으로, 병렬 프로세스에서 필요한 **로드밸런싱**을 간단한 형태로 구현하고 실습해보는 것이 목표입니다.

로드밸런서(LB, master node)는 데이터 제공자로 부터 텍스트 데이터를 받아, 분산된 작업 노드(worker node)들에게 적절히 데이터를 분배하여 전달합니다.

분산된 작업 노드들은 forked process의 형태로 존재하도록 구현할 계획입니다.

각 작업 노드는 하나의 데이터를 처리하는데 1초의 시간이 걸리며, 데이터를 처리하는 동안에는 새로운 데이터를 받을 수 없습니다. (작업 노드가 바쁜상태)

로드밸런서는 작업 노드가 바쁜 지를 적절히 고려하여 요청을 분산하는 역할을 수행합니다.


## II. 기능 요구사항

프로세스는 세 종류로 구분합니다.

1. 클라이언트 (client)
2. 서버 마스터 노드 (service/master)
3. 서버 작업자 노드 (service/worker)

\[클라이언트]

* 클라이언트는 서버에 요청을 보내고, 돌아온 응답을 표준 출력에 표시하는 역할을 수행합니다.
* 서버로 보내는 요청은 표준 입력에서 받는데, 사용자가 입력한 문자열 한 줄을 하나의 요청으로 간주합니다.다.
* 클라이언트는 PIPE 파일을 통해 서버 마스터 노드에게 요청을 전달합니다.

\[서버 마스터 노드]
* 서버 마스터 노드는 3대의 작업자 노드를 생성하고 관리합니다.
* 서버 마스터 노드는 클라이언트가 요청한 내용을 파이프에서 추출하여, 현재 살아있는 서버 작업자 노드에게 전달합니다.
* 서버 마스터 노드는 작업자 노드가 죽는 것을 감지하고, 죽은 서버를 대신하여 새로운 서버를 생성합니다.

\[서버 작업자 노드]
* 하나의 서버 작업자 노드는 초당 1개의 Request만 처리가 가능합니다. 하나의 요청을 처리한 후에는 `sleep(1)`을 통해 대기 시간을 갖습니다.
* 서버 작업자 노드는 20%의 확률로 죽습니다. 요청을 처리하는 중에 죽는 경우는 없으며, 요청을 완전히 완수한 후, 클록틱을 10으로 나눈 나머지의 값을 구하고, 해당 값이 2보다 작다면 죽은 것으로 간주합니다.
* 서버 작업자 노드는 주기적으로 생존해있다는 시그널을 마스터 노드에게 전송합니다.

## III. 구조

```
src
 - main.c
 - constants.c // 상수나 설정 값들을 저장
 - server
    - master.h
    - master.c
    - worker.h
    - worker.c
 - client.h
 - client.c
```

## IV. 평가

다음은 평가 항목을 만족하는지를 요약한 일람입니다.

|             평가 항목              | 해당 기능         |
| :--------------------------------: | :---------------- |
|             난이도 (3)             | 매우 높아 보인다. |
|        시스템의 유용성 (3)         | 매우 높아 보인다. |
|       소스코드와 comment (3)       |                   |
|         컴파일과 작동 (3)          |                   |
|            document (3)            |                   |
| make사용과 modular programming (3) |                   |