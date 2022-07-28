/*
 * Created Date: 2022-07-28 (Wed)
 * Author: Doyun Jung (정도윤) / rabbit.white@daum.net
 * Subject: Smart Logistics IoT
 * Filename: webconn.h
 * Description:
 * 
 */
 
#ifndef WEBSITE_H
#define WEBSITE_H

class Webconn {

public:
  Webconn();
  ~Webconn();
  WiFiClient* getClient();
  void setClient(WiFiClient* client);
  
  char* getServer();
  void setServer(char* server);
  int getPort();
  void setPort(int port);

  String getUrl();
  void setUrl(String url);

  String getHost();
  void setHost(String host);
  
  void httpPostConn(String postData);
  void httpGetConn();

private:
  WiFiClient* client;
  char* server;
  int port;
  String url;
  String host;
};

Webconn::Webconn(){
  
}

Webconn::~Webconn(){
  
}

WiFiClient* Webconn::getClient(){
  return this->client;
}

void Webconn::setClient(WiFiClient* client){
  this->client = client; 
}

char* Webconn::getServer(){
  return this->server;
}

void Webconn::setServer(char* server){
  this->server = server;
}

int Webconn::getPort(){
  return this->port;
}

void Webconn::setPort(int port){
  this->port = port;
}

String Webconn::getUrl(){
  return this->url;
}
 
void Webconn::setUrl(String url){
  this->url = url;
}


String Webconn::getHost(){
  return this->host;
}

void Webconn::setHost(String host){
  this->host = host;
}

void Webconn::httpPostConn(String postData){

  char *usr_server = this->getServer();
  int usr_port = this->getPort();
  String usr_url = this->getUrl();
  String usr_host = this->getHost();

  if (client->connect(usr_server, usr_port)) {
    
    Serial.println("connected to server");
    // Make a HTTP request:
    //client->println("POST /search_post.php HTTP/1.1");
    client->println(usr_url);
    //client->println("Host: 100.30.0.5");
    client->println(usr_host);
    client->println("Content-Type: application/x-www-form-urlencoded");
    client->print("Content-Length: ");
    client->println(postData.length());
    client->println();
    client->print(postData);
    
  }
  
}

void Webconn::httpGetConn(){

  char *usr_server = this->getServer();
  int usr_port = this->getPort();
  String usr_url = this->getUrl();
  String usr_host = this->getHost();
  
  if (client->connect(usr_server, usr_port)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    //client->println("GET /search.php?q=arduino HTTP/1.1");
    client->println(usr_url);
    //client->println("Host: 100.30.0.5");
    client->println(usr_host);
    client->println("Connection: close");
    client->println();
  }
  
}

#endif
