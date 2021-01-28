/*********************
Simple toggle switch
Created by: P.Agiakatsikas
*********************/

int button = 2;
int led = 13;
int status = false;

void setup(){
pinMode(led, OUTPUT);
pinMode(button, INPUT_PULLUP); // set the internal pull up resistor, unpressed button is HIGH
}

void loop(){
//a) if the button is not pressed the false status is reversed by !status and the LED turns on
//b) if the button is pressed the true status is reveresed by !status and the LED turns off

if (digitalRead(button) == true) {
status = !status;
digitalWrite(led, status);
} while(digitalRead(button) == true);
delay(50); // keeps a small delay
}
