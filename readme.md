# Computer Networks Project - Socket Programming : Making a Simple HTTP Web Server
이 프로젝트는 C 언어의 소켓 프로그래밍을 이용하여 구현한 간단한 HTTP 웹 서버입니다. 컴퓨터 네트워크 과목의 과제로 진행되었으며, 브라우저에서 HTTP 요청을 수신하고 요청된 정적 파일을 전송하는 기능을 포함하고 있습니다.

## ✅ 주요 기능
- 브라우저의 HTTP GET 요청 수신 및 응답
- **수신한 HTTP 요청을 터미널에 출력** (`fwrite()` 사용으로 바이너리 안전)
- 파일이 존재하지 않을 경우 404 Not Found 응답 전송
- **지원되는 파일 확장자: `.html`, `.jpeg`, `.gif`, `.pdf`, `.mp3`**
- `Content-Type`, `Content-Length` 등의 HTTP 헤더 자동 설정 후 HTTP 응답 전송
- 클라이언트가 연결을 끊더라도 서버가 종료되지 않도록 SIGPIPE 무시 처리

## 📁 구성 파일
- `myserver.c` - 메인 서버 소스 코드
- `Makefile` - 컴파일을 위한 메이크파일
- `log.txt` - 실행 중 터미널에 출력되는 HTTP 요청 메시지를 파일로 기록한 예시입니다. (선택사항)
- 이외에 Project Result 폴더에는 테스트를 위한 견본 파일들이 포함되어 있습니다.
- Socket Programming Part 1과 Part 2 폴더에는 `myserver.c`의 스켈레톤 코드라고 할 수 있는 코드들이 들어있으며, [여기서 튜토리얼 영상을 시청할 수 있습니다.](https://youtu.be/LtXEMwSG5-8)

## ⚙️ 컴파일 및 실행 방법
이 웹서버는 리눅스 환경에서 컴파일 및 실행할 수 있습니다. 다음 명령어를 사용하여 컴파일합니다.
```bash
sudo make
./myserver <포트번호>
```
포트 번호에서도 알 수 있듯이 임의로 지정할 수 있습니다. 다만, 1024 이상의 포트를 사용하는 것이 권장됩니다.

실행 방법:
```bash
./myserver <포트번호>
```

브라우저에서 접속하는 방법:
```
http://<IP 주소>:<포트번호>/<원하는 파일>
```
만약 같은 머신에서 접속하는 경우에는 IP 주소 대신 `localhost` 또는 `127.0.0.1`을 사용하면 됩니다.

## 📌 실행 결과 예시 (터미널 출력)

```
GET /bear.gif HTTP/1.1
Host: <IP Address>:1111
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Accept-Encoding: gzip, deflate
Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7
```
포트 번호를 1111로 설정하였고, `bear.gif` 파일을 요청한 예시입니다.

## 💡 참고 사항
- 현재는 클라이언트 요청을 한 번에 하나씩만 처리하는 구조입니다. 여러 기기에서 동시에 접속하는 것은 구현되어있지 않습니다.
- `SIGPIPE`를 무시하여 클라이언트가 연결을 끊더라도 서버가 종료되지 않도록 처리하였습니다.
- 서버 실행 디렉토리에 정적 파일이 함께 있어야 합니다.
- Firefox, Chrome 등 주요 브라우저에서 테스트 완료