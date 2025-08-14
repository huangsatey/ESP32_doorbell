#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define IR 4
#define LED1 5
#define LED2 6
#define LED3 7
#define ID 1  //发射机编号，A机写1，B机写2


uint8_t ToAddress[] = {0x24, 0xEC, 0x4A, 0x2F, 0xA0, 0xE4};//这是你要发送到的地址，填入接收机的MAC地址

//定义结构体 方便后期扩容
typedef struct message
{
  bool isThereHuman;  
  int sender_id;        //发射机的编号
} mesage;

message willSend;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}// ESPNOW测试用的回调函数，没串口的也没有用

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


void setup(){
  pinMode(IR,INPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);

  digitalWrite(LED1,LOW);//LED提前拉低以防出意外
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);//进入STA无线终端模式

  if (esp_now_init() != ESP_OK) {
    LED_LONG();
  }else{
    LED_FAST();
    LED_FAST();//闪两下
  }

  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, ToAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;//不加密

  if (esp_now_add_peer(&peerInfo) != ESP_OK){//初始化通讯参数
    LED_LONG();
  }else{
    LED_FAST();
    LED_FAST();//闪两下
  }

  digitalWrite(LED1,LOW);//保证完全灭灯
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);


}


void loop(){
  willSend.isThereHuman = digitalRead(IR);
  willSend.sender_id = ID;

  esp_now_send(ToAddress, (uint8_t *) &willSend, sizeof(willSend));

  delay(150);
  
}