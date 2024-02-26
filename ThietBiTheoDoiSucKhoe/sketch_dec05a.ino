#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Ticker.h>

#include <MPU6050.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define WIFI_SSID "XuanSon"
#define WIFI_PASSWORD "xuanson67"
#define FIREBASE_HOST "datn-47efe-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Ytr9ML01EhmDcko7plpNge9ZI5WI679AIWwE7vYp"


//#define WIFI_SSID "Phong 11 BRVT"
//#define WIFI_PASSWORD "20002001"
//#define FIREBASE_HOST "datn-47efe-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "Ytr9ML01EhmDcko7plpNge9ZI5WI679AIWwE7vYp"


PulseOximeter pox;
Ticker updatePoxTicker;
uint32_t tsLastReport = 0;
#define REPORTING_PERIOD_MS 1000
int heartRate, spo2;
unsigned long timeCheckHeartRate=millis();
boolean checkHeartRateState=0;
const int buzzerPin = 5;
bool buzzerOn = false;
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define G_THRESHOLD 12000 // G là đơn vị gia tốc
#define GYRO_THRESHOLD 20000 // độ/s
#define rxGPS 16
#define txGPS 17
SoftwareSerial neogps(rxGPS, txGPS);
TinyGPSPlus gps;
String Latitude,Longitude;
String timeString, dateString;
String timeAlarm1, dateAlarm1;
String timeAlarm2, dateAlarm2;
String timeAlarm3, dateAlarm3;
Ticker checkTimeAlarm;

FirebaseData firebaseData;
String dataRead;
boolean wifiMode=0;
unsigned long timeFirebase=millis();

#define rxGSM 18
#define txGSM 19
SoftwareSerial sim800(rxGSM, txGSM);
String phoneNumber="0965671164";
//String phoneNumber="0336874092";
#define btnSOS 23
unsigned long timeDelay=millis();
boolean btnPress=0;
int index_RxData=-1;
boolean activateBuzzer=0;

void setup() {

  
  pinMode(buzzerPin, OUTPUT);
  pinMode(btnSOS,INPUT_PULLUP);
  attachInterrupt(btnSOS,handleBtnSOS,FALLING);
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setRate(100);
  neogps.begin(9600);
  neogps.listen();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Đang kết nối wifi: ");
  Serial.println(WIFI_SSID);
  int n=0;
  while(n<120){
    if(WiFi.status()==WL_CONNECTED){
      Serial.println("\nĐã kết nối đến wifi!");
      Serial.print("Ip kết nối: ");Serial.println(WiFi.localIP());
      wifiMode=1;
      break;
    }
    n++;
    delay(500);
    Serial.print(".");
  }
  if(wifiMode==0) Serial.println("Không kết nối được wifi!");
  else{
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    getDataFirebase();
    timeClient.begin();
    timeClient.setTimeOffset(25200);
  }

  
  if (!pox.begin()) {//Khởi tạo giao tiếp với cảm biến nhịp tim
      Serial.println("FAILED");
      for(;;);
  } else {
      Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  updatePoxTicker.attach_ms(100, updatePoxCallback); // Gọi mỗi 100ms
  checkTimeAlarm.attach(60,checkAlarm);

  sim800.begin(9600);
  sim800.listen();
  sim800.print("AT+CMGF=1\r");
  setupSim800();
  delay(1000);

  
}

void loop() {
  if(activateBuzzer==1){
    if (!buzzerOn) {
      digitalWrite(buzzerPin, HIGH);  // Kích hoạt còi kêu
      buzzerOn = true;
      delay(3000);  // Chờ 3 giây
      digitalWrite(buzzerPin, LOW);  // Tắt còi kêu
      buzzerOn = false;
    }
    activateBuzzer=0;
  }
  if(btnPress==1){
    String str = "SOS! https://nikho.mbsd.work/redirect?url=elderlycare%3A%2F%2Flocation%2Fparam%3Flat%3D" +String (Latitude) + "lng%"+ "3D" + String(Longitude);
    sim800SMS(phoneNumber, str);
    sim800Call(phoneNumber);
    
    btnPress=0;
  }
  timeClient.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float hr, sp;
    hr = pox.getHeartRate();
    sp = pox.getSpO2();
    Serial.print(String(hr)+"o"+sp); //Đọc nhịp tim và oxy gửi qua uart cho esp32
    Serial.println();
    if(hr>0 && sp>0){ //nếu nhịp tim >0 thì đợi sau 5 giây chờ kết quả
      if(millis()-timeCheckHeartRate > 5000){
        if(checkHeartRateState==0){
          checkHeartRateState=1;
        }
      }
      if(checkHeartRateState==1){ //nếu đã qua 30s thì đọc kết quả
//        Serial.println("Kiểm tra nhịp tim!");
        heartRate = hr;
        spo2 = sp;
        //viết điều kiện còi kêu ở đây
        timeCheckHeartRate=millis();
        checkHeartRateState=0;
      }
    }else{
      checkHeartRateState=0;
      timeCheckHeartRate=millis();
    }

    
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    if (abs(ax) < G_THRESHOLD && ax > 7000) {
    // Phát hiện té ngã do gia tốc
    Serial.println("Té ngã do gia tốc!");
        sendFallAlert();
            if (!buzzerOn) {
        digitalWrite(buzzerPin, HIGH);
        buzzerOn = true;
      }
    }
    
    else {
    // Nếu không còn tình trạng té ngã, tắt còi nếu nó đang được bật
    if (buzzerOn) {
      digitalWrite(buzzerPin, LOW);
      buzzerOn = false;
    }
    // Thêm các hành động xử lý sau khi phát hiện té ngã ở đây
  }

    setDataFirebase();
    time_t epochTime = timeClient.getEpochTime();
//    Serial.print("Epoch Time: ");
//    Serial.println(epochTime);
    String formattedTime = timeClient.getFormattedTime();
    Serial.print("Formatted Time: ");
    Serial.println(formattedTime);
    String weekDay = weekDays[timeClient.getDay()];
    Serial.print("Week Day: ");
    Serial.println(weekDay); 
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;
    char temp_string[12];
    sprintf(temp_string,"%02d/%02d/%04d",monthDay,currentMonth,currentYear);
//    Serial.println("DateNTP:");
//    Serial.print(temp_string);
//    Serial.print("\t");
    dateString=temp_string;
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    sprintf(temp_string,"%02d:%02d",currentHour,currentMinute);
//    Serial.println(temp_string);
    timeString=temp_string;
   
    tsLastReport=millis();
  }
  if(millis()-timeFirebase>1000){
    readGPS(1000);
    if (gps.location.isUpdated()) {
      Latitude = String(gps.location.lat(),6);
      Longitude = String(gps.location.lng(), 6);
//      Serial.print("Latitude= ");
//      Serial.print(Latitude);
//      Serial.print(" Longitude= ");
//      Serial.println(Longitude);
      
    }
    getDataFirebase();
    timeFirebase=millis();
  }

  if(sim800.available()>0){
    String str="";
    while(sim800.available()){
       Serial.write(sim800.read());
       str = sim800.readString();  
    } 
    Serial.println("SMS:"+str);
    index_RxData = -1;
    index_RxData = str.indexOf("vitri");
    if(index_RxData >= 0){
      Serial.println("Nhận tin nhắn gửi vị trí!");
      String str = "Vi tri: https://nikho.mbsd.work/redirect?url=elderlycare%3A%2F%2Flocation%2Fparam%3Flat%3D" +String (Latitude) + "lng%"+ "3D" + String(Longitude);
      sim800SMS(phoneNumber, str);
      index_RxData = -1;
    }
  } 
//  while (Serial.available()){ 
//      char temp = char(Serial.read());
//      Serial.print(temp);
//      sim800.write(temp);
//  }

}
ICACHE_RAM_ATTR void handleBtnSOS(){
  if(millis()-timeDelay>500){
    btnPress=1;
    Serial.println("Press button SOS!");
    timeDelay=millis();
  }
}
void updatePoxCallback(){
  pox.update();
}
int calculateTotalAcceleration(int ax, int ay, int az) {
//  return abs(ax) + abs(ay) + abs(az);
return abs(az);
}
void readGPS(unsigned long ms){
  unsigned long start = millis();
  do 
  {
    while (neogps.available())
      gps.encode(neogps.read());
  } while (millis() - start < ms);
}
void getDataFirebase(){
  FirebaseJson data;
  if (Firebase.RTDB.getJSON(&firebaseData, "/data/0965671164/event", &data)){
    dataRead = "";
    dataRead = data.raw();
//    Serial.print("Data Read:");
//    Serial.println(dataRead);
    DynamicJsonDocument doc(512);
    deserializeJson(doc, dataRead);
    JsonObject obj = doc.as<JsonObject>();
    timeAlarm1 = obj["time1"]["time"].as<String>();
    dateAlarm1 = obj["time1"]["date"].as<String>();
    timeAlarm2 = obj["time2"]["time"].as<String>();
    dateAlarm2 = obj["time2"]["date"].as<String>();
    timeAlarm3 = obj["time3"]["time"].as<String>();
    dateAlarm3 = obj["time3"]["date"].as<String>();
    Serial.print("TimeAlarm1: "+dateAlarm1+"\t");
    Serial.println(timeAlarm1);
    Serial.print("TimeAlarm2: "+dateAlarm2+"\t");
    Serial.println(timeAlarm2);
    Serial.print("TimeAlarm3: "+dateAlarm3+"\t");
    Serial.println(timeAlarm3);
    
  }
}
void setDataFirebase(){
  FirebaseJson json;
  json.set("healthParam/oxygen",spo2);
  json.set("mpu",ax);
  json.set("healthParam/heartRate",heartRate);
  json.set("location/lat",Latitude);
  json.set("location/lng",Longitude);
  json.set("DATE/time",timeString);
  json.set("DATE/date",dateString);
  if (Firebase.RTDB.setJSON(&firebaseData, "/data/0965671164/param", &json)){
    //Serial.println("Write json success!!!");
  }else{
    Serial.println("Write json error!!!");
  }
}
void checkAlarm(){
  Serial.println("\nCheck time------------------------");
  if(dateAlarm1==dateString){
   if(timeAlarm1==timeString){
    Serial.println("\n\n===============>Đã đến giờ hẹn1!");
    //activateBuzzer=1;
   }
  }
  if(dateAlarm2==dateString){
   if(timeAlarm2==timeString){
    Serial.println("\n\n===============>Đã đến giờ hẹn2!");
    //activateBuzzer=1;
   }
  }
  if(dateAlarm3==dateString){
   if(timeAlarm3==timeString){
    Serial.println("\n\n===============>Đã đến giờ hẹn3!");
    //activateBuzzer=1;
   }
  }
}

void sendFallAlert(){
  String str = "Te nga! https://nikho.mbsd.work/redirect?url=elderlycare%3A%2F%2Flocation%2Fparam%3Flat%3D" +String (Latitude) + "lng%"+ "3D" + String(Longitude);
    sim800SMS(phoneNumber, str);
   sim800Call(phoneNumber);

}
void setupSim800(){
  sim800.println("ATE0"); //Tắt chế độ echo khi gửi lệnh đi
  delay(1000);
  sim800.println("AT+IPR=9600"); //Cài tốc độ baud 9600
  delay(1000);
  sim800.println("AT+CMGF=1"); //Hiển thị tin nhắn ở chế độ txt
  delay(1000);
  sim800.println("AT+CLIP=1"); //Hiển thị số điện thoại gọi đến
  delay(1000);
  sim800.println("AT+CNMI=2,2"); //Hiển thị trực tiếp nội dung tin nhắn gửi đến
  delay(1000);

  sim800.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
    sim800.println("ATE1");
  delay(1000);
}
void sim800Call(String sdt){
  sim800.println("ATD" + sdt + ";");
  //delay(1000);
  Serial.println();
  Serial.print("Đang thực hiện cuộc gọi đến số: ");
  Serial.println(sdt);
  delay(5000);
}
void endCall(){
  sim800.println("ATH");
  delay(500);
}
void sim800SMS(String phone, String content){
  Serial.println("Thực hiện gửi SMS đến số "+String(phone));
  sim800.println("AT+CMGS=\"" + phone + "\"");     // Lenh gui tin nhan
  delay(3000);                                     // Cho ky tu '>' phan hoi ve 
  sim800.print(content);                           // Gui noi dung
  sim800.print((char)26);                          // Gui Ctrl+Z hay 26 de ket thuc noi dung tin nhan va gui tin di
  delay(5000);                                     // delay 5s
}
