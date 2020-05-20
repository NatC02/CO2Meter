/*
 * Carbon Dioxide Parts Per Million Sensor
 * CO2PPM
 * 
 * Avearge Global Atmospheric CO2 Lvl..............415.70 PPM
 * Average Indoor CO2 PPM.............350-450PPM
 * Maxiumum acceptable CO2 PPM...........1000PPM
 * Dangerous CO2 Lvl............. above 2000PPM
 */


//LIBRARIES

#include <Wire.h>                             //I2C for OLED
    #include <Adafruit_GFX.h>                     //Grafix library for OLED Screen
#include <Adafruit_SSD1306.h>                 //OLED Driver
#include "dht.h"


//DEFINE

#define DHTPin 

#define anInput     A0                        // Analog feed from MQ135
#define digTrigger   2                        // Digital feed from MQ135
#define CO2Zero      5                        // Calibrated CO2 0 level
#define LEDBlue      3                        // Blue LED on pin 3
#define LEDGreen     5                        // Green LED on pin 5
#define LEDYellow    6                        // Yellow LED on pin 6
#define LEDRed       9                        // Red LED on pin 9
#define OLED_RESET   4                        // OLED reset on pin 4
#define DHT_pinA1    A1                       // Humidity and Temp Sensor on pin A1


//LIBRARY CALL

Adafruit_SSD1306 display(OLED_RESET);         // Create instance of OLED called display
dht DHT;


//SETUP

void setup() {

  Serial.begin(9600);
  Serial.println("CO2 PPM, Humidity % & Temperature Sensor in C\n\n");
  
  pinMode(anInput, INPUT);                     // MQ135 analog feed set for input
  pinMode(digTrigger, INPUT);                  // MQ135 digital feed set for input
  pinMode(LEDBlue, OUTPUT);                    // LEDBlue set for output
  pinMode(LEDGreen, OUTPUT);                   // LEDGreen set for output
  pinMode(LEDYellow, OUTPUT);                  // LEDYellow set for output
  pinMode(LEDRed, OUTPUT);                     // LEDRed set for output
  Serial.begin(9600);                          // Serial comms for debuging
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // Begin display @ hex addy 0x3C
  display.display();                           // Show buffer
  display.clearDisplay();                      // Clear buffer
  
}


//MAIN LOOP

void loop() {

    DHT.read11(DHT_pinA1);
    
    Serial.print("Humidity = ");
    Serial.print(DHT.humidity);
    Serial.println("%  ");
    
    Serial.print("Temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C ");
    
    delay(2000);                              //Wait 2 Seconds before accessing sensor again.
  
int CO2now[10];                               // int array for CO2 readings
int CO2raw = 0;                               // int for raw value of CO2
int CO2comp = 0;                              // int for compensated CO2 
int CO2PPM = 0;                               // int for calculated PPM
int CO2avg = 0;                                  // int for averaging
int grafX = 0;                                // int for x value of graph

  display.clearDisplay();                     // Clear display @ beginning of each loop

  for (int x = 0;x<10;x++){                   // Sample CO2 10x over 2 Seconds
    CO2now[x] = analogRead(A0);
    delay(2000);
  }

for (int x = 0;x<10;x++){                     // add samples together
    CO2avg=CO2avg + CO2now[x];
    
  }
  CO2raw = CO2avg/10;                            // Divide samples by 10
  CO2comp = CO2raw - CO2Zero;                 // Get compensated value
  CO2PPM = map(CO2comp,0,1023,400,5000);      // Map value for atmospheric levels

  
  display.setTextSize(1);                     // Set text size
  display.setTextColor(WHITE);                // Set text color
  display.setCursor(0,0);                     // Set cursor
  //display.println("TEMP =        HUM =");
  //display.print(DHT.temperature);
  //display.print(" C       ");
  //display.print("Hum: ");
  //display.print(DHT.humidity);
  //display.println(" %");
  //display.println(" ");                     // Skip a line
  display.setTextSize(1);
  display.print("  CO2 Lvl | ");              // Print title
  display.print(CO2PPM);                      // Print CO2 PPM
  display.print(" PPM");                      // Print Units
  Serial.print("CO2 = ");
  Serial.print(CO2PPM);
  Serial.println(" PPM");
  grafX = map(CO2PPM,0,2000,0,127);            // Map value to screen width
  display.fillRect(0, 25, grafX, 50, WHITE);   // Print graph 400min 1000max
  display.display();                           // Show the buffer

  if(CO2PPM<415){
  digitalWrite(LEDBlue, HIGH);     // Turn LEDBlue on
  }
  else {
    digitalWrite(LEDBlue, LOW);    // Turn LEDBlue off
  }
  if(CO2PPM>415 && CO2PPM<450){
    digitalWrite(LEDGreen,HIGH);   // Turn LEDGreen on
  }
  else {
    digitalWrite(LEDGreen,LOW);    // Turn LEDGreen off
  }
  if (CO2PPM>450 && CO2PPM<999){
    digitalWrite(LEDYellow,HIGH);  // Turn LEDYellow on
  }
  else {
    digitalWrite(LEDYellow,LOW);   // Turn LEDYellow off
  }
  if(CO2PPM>999){
    digitalWrite(LEDRed,HIGH);     //turn LEDRed on
  }
  else {
    digitalWrite(LEDRed,LOW);      //turn LEDRed off
  }
}
