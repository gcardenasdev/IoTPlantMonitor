const int AirValue = 502;   //you need to replace this value with Value_1
const int WaterValue = 259;  //you need to replace this value with Value_2

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}

void loop()
{

  int sensorVal = analogRead(A0);

  int percentageHumididy = map(sensorVal,WaterValue, AirValue, 100, 0); 

  Serial.print(percentageHumididy);
  Serial.println("%");
  delay(100);
}