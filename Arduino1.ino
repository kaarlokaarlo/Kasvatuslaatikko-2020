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
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  dht.begin();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  }

void loop() {

  delay(2000);
   
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
  
  lcd.setCursor(8, 0);
  lcd.print(temperature, 1);
  
  lcd.setCursor(12, 0);
  lcd.print("C");

  lcd.setCursor(7, 1);
  lcd.print("a");
  
  lcd.setCursor(8, 1);
  lcd.print(airHumidity, 0);
  
  lcd.setCursor(10, 1);
  lcd.print("%");

  lcd.setCursor(12, 1);
  lcd.print("s");

  lcd.setCursor(13, 1);
  lcd.print(soilHumidity, 0);

  lcd.setCursor(15, 1);
  lcd.print("%");

  Serial.println(soilHumidity);
}

void receiveEvent() {
  float x = Wire.read();    // receive byte as a float
  soilHumidity = x;
  //Serial.println(x);        // print the float
}
