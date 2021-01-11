#include <Wire.h>
#include <SoftwareWire.h>
#include "RTClib.h"

// the Arduino pin, which connects to the IN pin of relay
const int RELAY_PIN = 3;

#define SensorPin A3 

float sensorValueMapped = 0.0;
float sensorValue = 0;
int counter = 0;
float average = 0.0;
char my_str[] = " %";
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//a variable for watering the plant only once a day max
boolean waterToday = true;

//amount of water pumped in a single watering (in ms the pump is on)
int waterAmount = 500;

// Using pin 8 (software sda) and 9 (software scl).
SoftwareWire myWire(8, 9);
RTC_DS3231 rtc;

// the setup function runs once when you press reset or power the board
void setup() {
   Serial.begin(9600);
   // wait for console opening
   delay(3000);
   // initialize digital pin A3 as an output.
   pinMode(RELAY_PIN, OUTPUT);
   myWire.begin();

 //Alert if RTC is not found
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

 if (rtc.lostPower()) {
    //Alert if RTC needs to be recalibrated
    Serial.println("RTC lost power, lets set the time!");
  
    // Comment out below lines once you set the date & time.
    
    // Following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
 }
}


// the loop function runs over and over again forever
void loop() {

 //get DateTime from rtc module
 DateTime now = rtc.now();

 for (int i = 0; i <= 100; i++) 
 { 
   sensorValue = sensorValue + analogRead(SensorPin); 
   delay(1); 
 } 
 
 sensorValue = sensorValue/100.0;
 sensorValueMapped = map(sensorValue,1023,0,0,100);
 average = average + sensorValue;
 
 //reset average every 10 measurements
 counter = (counter + 1) % 10;
 
 if(counter == 0){
  average = 0;
 }

 //pumps water to if not yet today and soil humidity is low
 if(!waterToday && average / 10.0 <= 100.0 && counter == 9){
  pump();
 }

 //resets watering to be possible every day at 9:00:00 in the morning if it's not
 if(now.hour() == 9 && now.minute() == 0 && now.second() == 0) {
  waterToday = false;
 }
 
  //sending soil humidity sensor data to slave board
  myWire.beginTransmission(8);             // transmit to device #8
  myWire.write(100.0 - sensorValueMapped); // sends four bytes (float)
  myWire.endTransmission();                // stop transmitting

  //Serial prints for debugging
  
  //Print this for monitoring soil humidity sensor values
  //Serial.println(String(sensorValueMapped) + String(my_str)); 
  Serial.println("Current Date & Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.print(now.year() == 2021);
  Serial.print(" now year == 2021");
  Serial.println();
  Serial.print(waterToday);
  Serial.print(" watered today");
    
 delay(30);

}

//function for controlling the pump with the given amount
void pump() {
  // turn on pump for waterAmount * milliseconds
  digitalWrite(RELAY_PIN, HIGH);
  delay(waterAmount);
  digitalWrite(RELAY_PIN, LOW);
  delay(waterAmount);
  waterToday = true;
}
