// TEMPERATURE SENSOR (THERMISTOR TUTORIAL) - referred Circuit Basis - https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
// Steinhart-Hart equation is used to convert the resistance of the thermistor to a temperature reading.

// Libraries
#include <Servo.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

// Make servo "object", here - doorMotor
Servo fin1Motor, fin2Motor;
const int FIN1MOTORPIN = 9;
const int FIN2MOTORPIN = 13;

int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf, servo1Angle, servo2Angle;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  Serial.begin(9600);
  servo1Angle = 0.0;
  servo2Angle = 0.0;
  
  // Tell Arduino where the motor is plugged
  fin1Motor.attach(FIN1MOTORPIN);
  fin2Motor.attach(FIN2MOTORPIN);

  fin1Motor.write(servo1Angle);
  fin2Motor.write(servo2Angle);
  
  // LCD format
  lcd.begin(16, 2);
}

void loop() {
  // Reading thermoristor values
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);

  // Voltage to K Temperature converstion
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));

  // Celcius and Farenheit conversion
  Tc = T - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0; 
  
  // Display temperature in LCD
  lcd.clear();
  lcd.print(Tc);
  lcd.print(" C");
  lcd.setCursor(0,1);  
  lcd.print(Tf);
  lcd.print(" F");


  servo1Angle = fin1Motor.read();
  servo2Angle = fin2Motor.read();
  // Move servoMotor
  if (Tc <= 24){
    while ((0 < servo1Angle) and (servo2Angle < 180)) {
      fin1Motor.write(servo1Angle + 5);
      fin2Motor.write(servo2Angle - 5);
      delay(1000);
      servo1Angle = fin1Motor.read();
      servo2Angle = fin2Motor.read();
    } 
  }
  // Move servoMotor
  if (24 < Tc){
    while ((servo1Angle < 180) and (0 < servo2Angle)){
      fin1Motor.write(servo1Angle - 5);
      fin2Motor.write(servo2Angle + 5);
      delay(1000);
      servo1Angle = fin1Motor.read();
      servo2Angle = fin2Motor.read();
    } 
  }
   
  // Serial monitor prints - Motor Angles
  Serial.print("Current Fin 1 Angle: "); 
  Serial.print(servo1Angle);
  Serial.print("Current Fin 2 Angle: "); 
  Serial.print(servo2Angle);
  delay(5000);
}
