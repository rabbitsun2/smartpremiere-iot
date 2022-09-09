/*
 * Created Date: 2022-07-27 (Mon)
 * Author: Doyun Jung (정도윤) / rabbit.white@daum.net
 * Subject: Smart Logistics IoT
 * Filename: sm-iot.ino
 * Description:
 * 1. webconn으로 client 함수 이전, 정도윤, 2022-07-28(Wed)
 */
 
#include <DHT11.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
#include "webconn.h"

const int START_ID = 1;
const int END_ID = 4;

const int CHOOSE_LED_GREEN = 1;
const int CHOOSE_LED_YELLOW = 2;
const int CHOOSE_LED_BLUE = 3;
const int CHOOSE_LED_RED = 4;

// 핀 레이아웃
const int TRIG_PIN = 6;
const int ECHO_PIN = 7;

const int LED_GREEN = 8;
const int LED_YELLOW = 9;
const int LED_BLUE = 10;
const int LED_RED = 11;

const int DHT_PIN = 12;    // 연결한 아두이노 디지털 핀 번호
const int SHOCK_PIN = 13;

// 무선랜 설정 정보
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            

int status = WL_IDLE_STATUS;
int ardu_status = 0;

float temp, humi;
String url;
String host;
String shock;
String message;

char server[] = "100.30.0.5";    // 네임서버: smartLogistics

WiFiClient client;      // WifiClient 기본값 포트 80포트
DHT11 dht11(DHT_PIN);   // 온습도 센서

Webconn webconn;

void setup() {
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Wifi 셋업
  wifiSetup();
  Serial.println("Welcome!!");

  // 무선랜 클라이언트 설정
  webconn.setClient(&client);
  webconn.setServer(server);

  // 핀 모드 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(SHOCK_PIN, INPUT);

  // "환영합니다" 출력
  welcome();

}

void loop() {

  // 시리얼 가능 여부
  if(Serial.available()){

    switch(Serial.read()){

      case '1':
        ardu_status = 1;
        break;
        
      case '0':
        ardu_status = 0;
        break;
      
    }
      
  }

  // 프로그램 실행
  op_program();

}

void op_program(){

  String postData;
  long duration, distance;

  // 활성화 상태
  if ( ardu_status == 1 ){

    // 무선랜 접속 여부
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
   
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = ((float)(340 * duration) / 1000) / 2;
    
    Serial.print("Duration:");
    Serial.print(duration);
    Serial.println();
    Serial.print("Distance:");
    Serial.print(distance);
    Serial.println();
  
    // 충격 감지 센서
    if ( digitalRead( SHOCK_PIN ) == HIGH ){
      // Serial.println("SHOCK!!!!");
      shock = "SHOCK";
    }else{
      shock = "STD";
    }
  
    if ( distance > 500 ){
      choose_led( CHOOSE_LED_GREEN );
    }else{
      choose_led( CHOOSE_LED_YELLOW );
      delay(500);
      
      choose_led( CHOOSE_LED_RED );
      delay(500);
    }
  
    // DHT 센서(온습도 센서)
    dht_sensor();
    delay(100);
  
    // 메시지
    message = "정상";
    
    // POST 호출
    postData = "distance=";
    postData = postData + String(distance);
    postData = postData + "&duration=" + String(duration);
    postData = postData + "&humi=" + String(humi);
    postData = postData + "&temp=" + String(temp);
    postData = postData + "&shock=" + shock;
    postData = postData + "&machine_id=" + String(1);
    postData = postData + "&message=" + message;
    postData = postData + "&uuid=" + SECRET_MACHINE;
  
    //Serial.println(postData);
  
    // 호스트 설정
    host = "Host: 100.30.0.5";
    webconn.setHost(host);
  
    // URL 설정
    url = "POST /smartLogistics/index.php/iot/collect HTTP/1.1";
    //url = "GET /smartLogistics/index.php/iot/collect?" + postData + " HTTP/1.1";
    webconn.setUrl(url);
  
    Serial.println(url);
    
    // 포트 설정
    webconn.setPort(80);
  
    // 웹 클라이언트 시작
    webconn.httpPostConn(postData);
    //webconn.httpGetConn();
    delay(100);
  
    // 연결이 끊겼을 때, 연결 종료
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
  
      // do nothing forevermore:
      while (true);
    }

  }
  // 비활성화 상태
  else if ( ardu_status == 0 ){
    Serial.println("disconnected Machine");
    delay(1000);
  }
  
}

void wifiSetup(){
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  
}

void printWifiStatus() {

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

}


void welcome(){

  for ( int i = 0; i < 2; i++ ){
    
    for ( int j = START_ID; j <= END_ID; j++ ){
      choose_led( j );
      delay(500);
    }

    for ( int j = END_ID; j >= START_ID; j-- ){
      choose_led( j );
      delay(500);
    }
    
  }
  
}

void choose_led(int choose){

  switch( choose ){

      case 1:
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_RED, LOW);
        
        break;

     case 2:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, HIGH);
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_RED, LOW);
        
        break;

     case 3:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_BLUE, HIGH);
        digitalWrite(LED_RED, LOW);
        
        break;

     case 4:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_RED, HIGH);
        
        break;
      
    }
  
}


void dht_sensor(){
  
  int err;
 
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();

  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    

  }

  delay(DHT11_RETRY_DELAY); //delay for reread
  
}
