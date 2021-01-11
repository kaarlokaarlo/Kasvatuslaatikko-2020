// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Wire.h>

#define DHTPIN 7  
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float soilHumidity = 0.0;

void setup() {
  //recieve data from device #8 (Master arduino)
  Wire.begin(8);
  Wire.onReceive(receiveEvent);

  //initialize DHT sensor
  dht.begin();
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  Serial.begin(9600);
  }

void loop() {

  delay(2000);

  //Read data from DHT sensor
  float temperature = dht.readTemperature();
  float airHumidity = dht.readHumidity();
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("temp:");
  lcd.setCursor(0, 1);
  lcd.print("humid:");

  //print temperature on lcd
  lcd.setCursor(8, 0);
  lcd.print(temperature, 1);
  lcd.setCursor(12, 0);
  lcd.print("C");

  //print air humidity on lcd
  lcd.setCursor(7, 1);
  lcd.print("a");
  lcd.setCursor(8, 1);
  lcd.print(airHumidity, 0);
  lcd.setCursor(10, 1);
  lcd.print("%");

  //print soil humidity on lcd
  lcd.setCursor(12, 1);
  lcd.print("s");
  lcd.setCursor(13, 1);
  lcd.print(soilHumidity, 0);
  lcd.setCursor(15, 1);
  lcd.print("%");

  //debug print for soil humidity
  Serial.println(soilHumidity);
}

//function to be called in Wire onReceive 
void receiveEvent() {
  float x = Wire.read();    // receive byte as a float
  soilHumidity = x;
  //Serial.println(x);        // print the float
}
