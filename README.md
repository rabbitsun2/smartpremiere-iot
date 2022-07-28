### Smart Logistics - Ai

##### 1. 프로젝트명: Smart Logistics Ai
##### 2. 작성자: 정도윤(Doyun Jung)
##### 3. 작성일자: 2022-06-05 (ver1.0)
##### 3. 이메일: rabbit.white at daum dot net
##### 4. 비고: 
##### - 1. 프로젝트 초안, 정도윤, 2022-07-27.
##### - Arduino Uno Wifi Rev 2.0 정품으로 개발함.
##### - 2. 핀 레이아웃 
#####      const int TRIG_PIN = 6;
#####      const int ECHO_PIN = 7;
#####      const int LED_GREEN = 8;
#####      const int LED_YELLOW = 9;
#####      const int LED_BLUE = 10;
#####      const int LED_RED = 11;
#####      const int DHT_PIN = 12;    // 연결한 아두이노 디지털 핀 번호
#####      const int SHOCK = 13;
##### 5. 개발 진행 상황
##### - WifiWebClient를 통해서 HTTP GET/POST 방식의 통신으로 센서의 유효한 정보를 수집하여
#####    DB에 저장 및 가공 처리할 예정임.
