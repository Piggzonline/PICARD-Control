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
byte y;
byte z;

int p = 0;
int q = 0;
int r = 0;
int s = 0;
int x = 0;

String inputString = "";         // a string to hold incoming data

void setup() { 

  Serial.begin(38400);    // Open serial communications
     
  pinMode(2, OUTPUT); //Set Pin 2 - LAFORGE Trigger - as output
  pinMode(3, OUTPUT); //Set Pin 3 - Deployment Trigger - as output
  pinMode(4, OUTPUT); //Set Pin 4 - Cameras Trigger - as output
  pinMode(5, INPUT);  //Set Pin 5 - Lift off Signal - as input
  pinMode(6, OUTPUT); //Set Pin 6 - Dataloggers Trigger - as output

 // rtc.begin(DateTime()); //start RTC
    
  digitalWrite(4, HIGH); //turn cameras on
  delay(5000);
  digitalWrite(4, LOW);
             
  inputString.reserve(200); } // reserve 200 bytes for the inputString

void loop() {

if ( x < 4 ) {
  //does this work as a Latch???
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
  
x = a + b + c + d + e + f + g; }   //majority passes high LO signal when x > 3
    
y = EEPROM.read(address);          //read eeprom

if ( x > 3 && y < 1 ) {            //LO = high? & eeprom data = 0?
  DateTime now = rtc.now();        //if true, read rtc
  val = now.unixtime();            //rtc time = val
  EEPROM.write(address, val);}     //write val to eeprom  

else if ( x > 3 && y > 0 ) {       //LO = high? & eeprom data > 0?
  DateTime now = rtc.now();        //read rtc
  z = now.unixtime();              //rtc time  = z
  t = (z - y);                     //time elapsed is t, difference between z and y

  if ( t > 10 && p < 1 ) {         //record with cameras
     digitalWrite(4, HIGH);
     delay(100);
     digitalWrite(4, LOW);
     p = 1; }
     
  if ( t > 20 && q < 1 ){          //turn on data loggers
     digitalWrite(6, HIGH);
     q = 1; }

  if ( t > 30 && r < 1 ) {         //turn on deployment signal
     digitalWrite(3, HIGH);
     r = 1; }
     
  if ( t > 40 && s < 1 ) {         //turn on LAFORGE
     digitalWrite(2, HIGH);
     s = 1; }

  if ( t > 50 ) {                  //turn off deployment signal
     digitalWrite(3,LOW); }

  if ( t > 60 ) {                  //turn off LAFORGE
     digitalWrite(2,LOW); }
     
  if ( t > 70 ) {                  //turn off data loggers
     digitalWrite(6,LOW); }

  if ( t > 80 && p > 0 && p < 2 ) {//turn off cameras
     digitalWrite(4,HIGH);
     delay(5000);
     digitalWrite(4,LOW); 
     p = 2; }
    
}

  DateTime now = rtc.now();
  h = now.unixtime();
  k = h - y;
  
    String inputString = "";         // a string to hold incoming data
    for (int i=0; i<64; i++){
      u = Serial.read();
      inputString += char(u); }
    inputString += "\n\r";
 
    String dataString = "";    //string for assembling the data to log  
    for (int analogPin = 0; analogPin < 4; analogPin++) {   // read five sensors and append to the string
      int sensor = analogRead(analogPin);
      dataString += String(sensor);
      if (analogPin < 4) {
        dataString += ","; }}

    String alfaString = "";
    for(int analoguePin = 6; analoguePin < 8; analoguePin++) {
      int sensor = analogRead(analoguePin);
      alfaString += String(sensor);
      if (analoguePin < 7) {
        alfaString += ","; }}

    dataString += alfaString + "-Data," + inputString + "-LAFORGE," + h + "-RTC Time," + k + "-EEPROM Time"; //requires refinement
    Serial.println(dataString);
        
    delay(100); }    //requires refinement
