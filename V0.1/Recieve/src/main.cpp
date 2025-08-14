#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define IR 4
#define LED1 5
#define LED2 6
#define LED3 7

typedef struct message
{
  bool isThereHuman;  
  int sender_id;        //接受 发射机的编号
} mesage;

message recieve;

esp_now_peer_info_t peerInfo;

void LED_LONG(){

    digitalWrite(LED1,LOW);//提前拉低恢复状态
    digitalWrite(LED1,HIGH);
    delay(200);
    digitalWrite(LED1,LOW);
    //长闪一下
}

void LED_FAST(){

    digitalWrite(LED1,LOW);//提前拉低恢复状态
    digitalWrite(LED1,HIGH);
    delay(50);
    digitalWrite(LED1,LOW);
    delay(50);
    //连闪
}


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  memcpy(&recieve, incomingData, sizeof(recieve));
  bool isHuman = recieve.isThereHuman;
  int fromWho = recieve.sender_id;
  if(fromWho == 1){
    digitalWrite(LED2,HIGH);
  }
  if(fromWho == 2){
    digitalWrite(LED3,HIGH);
  }
  if(isHuman){
    digitalWrite(LED1,HIGH);
    delay(2000);
  }else{
    digitalWrite(LED1,LOW);
    delay(60);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
  }

}

void setup(){
  pinMode(IR,INPUT);//这个口其实现在没用了
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);

  digitalWrite(LED1,LOW);//LED提前拉低以防出意外
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);//进入STA无线终端模式

  if (esp_now_init() != ESP_OK) {//初始化ESPNOW
    LED_LONG();
  }else{
    LED_FAST();
    LED_FAST();//闪两下
  }

  esp_now_register_recv_cb(OnDataRecv);
  LED_LONG();
}

void loop(){
  //这里不用有任何东西，回调函数会自动执行
}