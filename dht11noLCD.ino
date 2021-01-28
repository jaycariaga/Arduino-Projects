#include <dht.h>

dht DHT;

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
  Serial.println("Currently Reading Temperature and Humidity using DHT11 module");
  delay(1000);
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  float finaltemp = (DHT.temperature * 9 / 5)+ 32;
  Serial.println(finaltemp);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(5000);
}
