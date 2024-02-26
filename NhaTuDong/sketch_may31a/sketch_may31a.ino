//Final

//Include the library files
//#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
//#include <DHT.h>

//Initialize the LCD display
//LiquidCrystal_I2C LCD (0x27, 16, 2);
char auth[] = "Ap5-ulpx3UYCKseQjTbOFPPRvuYchLU4"; //Enter your Auth token
char ssid[] = "iPhone"; //Enter your WIFI name
char pass[] = "24022001"; //Enter your WIFI password
DHT dht (D3, DHT11); // (sensor pin, sensor type)
BlynkTimer timer;
unsigned long times-millis();
bool is PushOn = false;
int Light_button;
bool isPush0n2= false;
int Pump_button;
bool isPushOn3 = false;
int Fan_button;
Widget LED led (VO);
// Define component pins
#define Buzzer D4
#define MQ2 AO
#define light DO
#define relayl D7
#define relay2 D8

// VO LED Widget-LED connect is blinking
void blinkLedWidget() {
if (led.getValue()) {
led.off();
} else {
led.on();
}
}

void setup(){
Serial.begin(115200);
delay(100);
Blynk.syncVirtual (V9);
Blynk.syncVirtual (V11);
delay(100);
dht.begin();
timer.setInterval (100L, blinkLedWidget);

timer.setInterval (100L, DHT11sensor);
pinMode (AO, INPUT);
pinMode (DO, INPUT);
pinMode (D1, INPUT);
pinMode (D2, INPUT);
pinMode (D3, INPUT);
pinMode (D4, OUTPUT);
pinMode (D5, INPUT);
pinMode (D6, OUTPUT);
pinMode (D7, OUTPUT);
pinMode (D8, OUTPUT);

Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
dht.begin();

lcd.init(); //Khởi tạo LCD
lcd.backlight(); //Bật đèn nền cho LCD
lcd.setCursor (4,0); //Cột 2, dòng 0
lcd.print ("Senior 2"); //Ghi chữ bắt đầu từ cột thứ 2 dòng 0
lcd.setCursor(4,1); //Cột 0, dòng thứ 1
lcd.print ("Project");
delay(4000);
lcd.clear();
}
//Get buttons values
BLYNK_WRITE (V12) {
Fan_button param. asInt();
}
BLYNK_WRITE (V9) {
Light_button = param.asInt();
}
void loop() {
Blynk.run(); //Run the Blynk library
imer.run(); //Run the Blynk timer

//Get the MQ2 sensor values
int value = analogRead (MQ2);
Serial.println(value);
value = map (value, 0, 1024, 0, 100);

Blynk.virtualWrite (Vl, value);
lcd.setCursor(0, 0);
lcd.print ("GAS:");
lcd.print (" ");
lcd.print (value);
//Get the DHT11 sensor values
float h=dht.readHumidity();
float t = dht.readTemperature();

Blynk.virtualWrite (V2, t);
Blynk.virtualWrite (V3, h);
lcd.setCursor(9, 1);
lcd.print(":");
lcd.print (t);

lcd.setCursor(0, 1);
lcd.print ("H:");
lcd.print (h);
if (value> 65) {

 digitalWrite(Buzzer, HIGH);
delay (0);
digitalWrite(D8,HIGH);
delay(0);
}
else if (Fan_button ==1){
digitalWrite(D8,HIGH);
}
else if (int(t)>= 33 ) {
digitalWrite(D8, HIGH);
}
 else{
 digitalWrite(D8, LOW);
 digitalWrite (Buzzer, LOW);
 }
 //Rain
 int Rain = digitalRead (D5);
 lcd.setCursor(9, 0);
 lcd.print ("Rain: ");
 lcd.print (Rain);

 if (Rain= 1) {
 Serial.println("SUNNY");
 digitalWrite(D6, LOW);
 delay (50); 
 }
 else {
 Serial.println("RAIN");
digitalWrite(D6, HIGH);
 delay (50);
 digitalWrite (D6, LOW);
 delay (50);
 digitalWrite(D6, HIGH);
 delay(50);
 digitalWrite(D4, HIGH);
 delay(50);
 digitalWrite(D4, LOW);
 delay (50);
 digitalWrite(D4, HIGH);
 delay (50);
 }

 //LDRsensor
 int Light_notification = digitalRead (DO);
 if (Light_button == 1) {
   digitalWrite(D7, HIGH);
 }
 else if (Light_notification == 1) {
 digitalWrite (D7, HIGH);
 }
else {
  digitalWrite (D7,LOW);
 }

 }
