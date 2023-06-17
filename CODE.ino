#include<LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13);
#include <SoftwareSerial.h>
const int buzzer = 7;
#define sensor A0
byte degree[8] =
{
0b00011,
0b00011,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};
#define RX 2
#define TX 3
String AP = "Huzaifa";       // WiFi SSID
String PASS = "123456790"; // WiFi PASSWORD
String API = "HJ2UT6FF6BPDJQ9E";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
float temperature;
float reading;

SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  pinMode(buzzer, OUTPUT);
lcd.begin(16,2);
lcd.createChar(1, degree);
lcd.setCursor(0,0);
lcd.print(" Digital ");
lcd.setCursor(0,1);
lcd.print(" Thermometer ");
delay(2000);
lcd.clear();
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
reading=analogRead(sensor);
temperature=reading*(5.0/1023.0)*100;
delay(10);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Temperature");
lcd.setCursor(4,1);
lcd.print(temperature);
lcd.write(1);
lcd.print("C");
delay(1000);
char buf[16];
  String strTemp = dtostrf(temperature, 4, 1, buf);
if (temperature>34)
{
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);
}

 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(strTemp);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
