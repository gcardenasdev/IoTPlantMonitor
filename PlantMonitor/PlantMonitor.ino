#include <SoftwareSerial.h>    
SoftwareSerial esp(6, 7);
#include <DHT22.h>

#define DEBUG true

const long oneSecond = 1000;  // a second is a thousand milliseconds
const long oneMinute = oneSecond * 60;
const long halfHour = oneMinute * 30;
const long oneHour   = oneMinute * 60;
const long oneDay    = oneHour * 24;

String mySSID = "MitchFiberWiFi";  // Wi-Fi SSID
String myPWD = "TheDuncingJack1442"; // Wi-Fi Password
String myAPI = "Z3FMIA9R6EYVADJF";   // WRITE API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String soilMoisture = "field1"; 
String airTemp = "field2";
String humid = "field3";
String light = "field4";

DHT22 dht22(4); //Temp&Humidity sensor on digital pin 4

int lightSensorPin = 2; //define analog pin 2 for light sensor
int lightValue = 0; 

const int AirValue = 502;   
const int WaterValue = 259;  




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp.begin(115200);
  Send_AT_Cmd("AT+RST", 1000, DEBUG);                      
  Send_AT_Cmd("AT+CWMODE=1", 1000, DEBUG);                 
  Send_AT_Cmd("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   
  delay(1000); 

}

void loop() {

  //Temperature and Humidity Sensor Code:

  float temperature = dht22.getTemperature();
  float humidity = dht22.getHumidity();

  float f = (temperature * 1.8 ) + 32;
  Serial.print("humidity =");Serial.print(humidity,1);Serial.print("\t");
  Serial.print("temperature in Celcius =");Serial.println(temperature,1);Serial.print("\t");
  Serial.print("temperature in Fahrenheit =");Serial.println(f,1);

  //Light Sensor Code

  lightValue = pow(((((150 * 3.3)/(analogRead(lightSensorPin)*(3.3/1024))) - 150) / 70000),-1.25);
	Serial.println(lightValue, DEC); 

  //Soil Moisture Sensor Code
  int sensorVal = analogRead(A0); //define analog pin 0 for soil moisture sensor
  int percentageHumidity = map(sensorVal,WaterValue, AirValue, 100, 0); 

  Serial.print(percentageHumidity);
  Serial.println("%");

  String sendData = "GET /update?api_key="+ myAPI +"&"+ soilMoisture +"="+String(percentageHumidity)+"&"+ 
                      airTemp +"="+String(f)+"&"+ humid +"="+String(humidity)+"&"+ light +"="+String(lightValue);




  Send_AT_Cmd("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
  Send_AT_Cmd("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
  Send_AT_Cmd("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
  esp.find(">"); 
  esp.println(sendData);
  Send_AT_Cmd("AT+CIPCLOSE=0",1000,DEBUG);
  Serial.println("Done!");
  Serial.println("");
  delay(oneHour); //Saves data every sixty minutes

}

String Send_AT_Cmd(String command, const int timeout, boolean debug)
{
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  esp.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp.available())
    {
      char c = esp.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}
