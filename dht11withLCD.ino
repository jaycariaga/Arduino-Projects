#include <dht.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

dht DHT;

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
  Serial.println("Temperature Humidity");
  lcd.begin(16, 2);
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
    float finaltemp = (DHT.temperature * 9 / 5)+ 32;
      Serial.print(finaltemp);
      Serial.print(", ");
      Serial.println(DHT.humidity);
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(finaltemp);
  lcd.print((char)223);
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(4000);
  lcd.clear();
  lcd.print("Updating...");
  delay(1000);

}
