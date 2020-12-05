const int RELAY_PIN = 3;  // the Arduino pin, which connects to the IN pin of relay
#define SensorPin A5 
float sensorValueMapped = 0.0;
float sensorValue = 0; 
   int laskin = 0;
 float average = 0.0;
  char my_str[] = " %";
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin A5 as an output.
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600);

}

// the loop function runs over and over again forever
void loop() {

  for (int i = 0; i <= 100; i++) 
 { 
   sensorValue = sensorValue + analogRead(SensorPin); 
   delay(1); 
 } 
 
 sensorValue = sensorValue/100.0;
 sensorValueMapped = map(sensorValue,1023,0,0,100); 
 Serial.println(String(sensorValueMapped) + String(my_str)); 
 average = average + sensorValue;
 laskin = (laskin + 1) % 10;

if(laskin == 0){
  average = 0;
}


 
 


if(average / 10.0 <= 100.0 && laskin == 9){

  digitalWrite(RELAY_PIN, HIGH); // turn on pump 5 seconds
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);  // turn off pump 5 seconds
  delay(1000);
}

 delay(30); 

}
