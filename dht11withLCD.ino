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

//60000 ms per second
//3600000 ms per hour
//1000 ms per seccond
static int runHours = 13;
int minrun = 15;
static bool wait = true;
static bool stopincr = false; //true if already incremented, false if hours need incr

char * getTime(){
  //calling millis() counts number of ms since start of the program call...
  unsigned long allSeconds=millis()/1000;
  //int runHours= (allSeconds/3600 + hourrun) % 24;
  int secsRemaining=allSeconds%3600;
  int runMinutes= (secsRemaining/60  + minrun) % 60;
  
  if(runMinutes ==0 && !wait && !stopincr){
    runHours += 1;
    wait = true;
    stopincr = true;
    }
  else if(runMinutes == 0){ 
    wait = false;
    }
  else{ //minutes from 1 - 59
    stopincr = false;
    wait = true;
    }
    
    
  char * buf = (char *) malloc(50);
  sprintf(buf,"Time: %02d:%02d",runHours, runMinutes);
  //Serial.println(buf);
  return buf;
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
  lcd.setCursor(0,0);
  char * time = getTime();
  lcd.print(time);
  free(time);
  lcd.setCursor(0,1);
  lcd.print("Updating...");
  delay(1000);

}
