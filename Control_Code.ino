//PICARD Control Script

//This sketch was written by: Michael McCallum

//The sketch operates the PICARD experiment
//See sketch flow chart in the SED

//includes

#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"

RTC_Millis rtc;

// EEPROM variables, start reading from the first byte (address 0) of the EEPROM

int address = 0;
byte val;

//variables

byte a;
byte b;
byte c;
byte d;
byte e;
byte f;
byte g;
byte h;
byte k;
byte t;
byte u;
byte x;
byte y;
byte z;

//Global variables

boolean stringComplete = false; // whether the string is complete

//Strings for groundstation

String inputString = "";         //a string to hold incoming data

void setup() { 

  Serial.begin(38400);    // Open serial communications
     
  pinMode(2, OUTPUT); //Set Pin 2 - LAFORGE Trigger - as output
  pinMode(3, OUTPUT); //Set Pin 3 - Deployment Trigger - as output
  pinMode(4, OUTPUT); //Set Pin 4 - Cameras Trigger - as output
  pinMode(5, INPUT);  //Set Pin 5 - Lift off Signal - as input
  pinMode(6, OUTPUT); //Set Pin 6 - Dataloggers Trigger - as output
 // pinMode(7, INPUT);  //Set Pin 7 - rtc scl - as input
 // pinMode(8, INPUT);  //Set Pin 8 - rtc soa - as input

  rtc.begin(DateTime()); //start RTC
    
  digitalWrite(4, HIGH); //turn cameras on
  delay(5000);
  digitalWrite(4, LOW);
             
  inputString.reserve(200); } // reserve 200 bytes for the inputString

void loop() {
  //majority voting for LO detection
    a = digitalRead(5);
    delay(10);
    b = digitalRead(5);
    delay(10);    
    c = digitalRead(5);
    delay(10);
    d = digitalRead(5);
    delay(10);
    e = digitalRead(5);
    delay(10);  
    f = digitalRead(5);
    delay(10);
    g = digitalRead(5);
    delay(10); 
        
    x = a + b + c + d + e + f + g; //majority passes high LO signal when x > 3

//LATCH???
    
    y = EEPROM.read(address);     //read eeprom

if ( x > 3 && y < 1 ) {           //LO = high? & eeprom data = 0?
  DateTime now = rtc.now();       //if true, read rtc
  val = now.unixtime();           //rtc time = val
  EEPROM.write(address, val);}    //write val to eeprom  

else if ( x > 3 && y > 0 ) {      //LO = high? & eeprom data > 0?
  DateTime now = rtc.now();       //read rtc
  z = now.unixtime();             //rtc time  = z
  t = (z - y);                    //time elapsed is t, difference between z and y

 switch (t) {  
  case 10:
   digitalWrite(4, HIGH);
   delay(100);
   digitalWrite(4, LOW);
   break;
  case 20:
   digitalWrite(6, HIGH); 
   break; 
  case 30:
   digitalWrite(3, HIGH); 
   break;
  case 40:
   digitalWrite(2, HIGH); 
   break; 
  case 50:
   digitalWrite(3, LOW); 
   digitalWrite(2, LOW);
   digitalWrite(6, LOW);
   break;
  case 60:
   digitalWrite(4, HIGH);
   delay(100);
   digitalWrite(4, LOW);
   break; }}

  DateTime now = rtc.now();
  h = now.unixtime();
  k = h - y;
  
    String inputString = "";         // a string to hold incoming data
    for (int i=0; i<64; i++){
      u = Serial.read();
      inputString += char(u); }
    inputString += "\n\r";
 
    String dataString = "";    //string for assembling the data to log  
    for (int analogPin = 0; analogPin < 3; analogPin++) {   // read five sensors and append to the string
      int sensor = analogRead(analogPin);
      dataString += String(sensor);
      if (analogPin < 3) {
        dataString += ","; }}

    String alfaString = "";
    for(int analogPin = 6; analogPin < 7; analogPin++) {
      int sensor = analogRead(analogPin);
      alfaString += String(sensor);
      if (analogPin < 6) {
        alfaString += ","; }}

    dataString += alfaString + "-Data," + inputString + "-LAFORGE," + h + "-RTC Time," + k + "-EEPROM Time";
    Serial.println(dataString);
        
    delay(1000); }    
