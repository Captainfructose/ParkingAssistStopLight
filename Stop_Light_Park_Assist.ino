/*
 * Parking Helper Stoplight Project
 * Created by Science1324
 * https://www.youtube.com/@science1324
*/

// Define the rotary encoder Pins
int CLK_IN = 4; // Clock
int  DT_IN = 2; // Data
int  SW_IN = 8; // Switch

// Define the variables for the rotary encoder
int COUNT = 0; 
int CURRENT_CLK_STATE = LOW;
int PREVIOUS_CLK_STATE = CURRENT_CLK_STATE;  
int SWITCH_STATE = HIGH;

// Define ultrasonic Sensor Pins
#define trigPin 9
#define echoPin 10

// Define LED Pins
#define ledY 11 // Yellow LED
#define ledR 12 // Red LED
#define ledG 13 // Green LED

// Values to calculate at what distance the lights turn on
int const LIGHT_1_S = 12;
int const LIGHT_2_S = 24;
int const LIGHT_3_S = 48;

// Variables for the distance calculations
int LIGHT_1;
int LIGHT_2;
int LIGHT_3;
int DISTANCE_1;

// OLED Display Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width (in pixels)
#define SCREEN_HEIGHT 32 // OLED display height (in pixels)

// Declaration for an SSD1306 I2C display
#define OLED_RESET 4 // Reset pin (-1 if sharing Arduino built in reset pin)

const unsigned char startBitmap []  PROGMEM = 
{ // Stoplight and car start up screen 128x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x03, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1f, 0xf1, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xf1, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1f, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1f, 0xc0, 0x7f, 0x00, 0x00, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xc0, 0x7e, 0x00, 0x00, 0x19, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0xe0, 0xf8, 0x00, 0x00, 0x01, 0xff, 0xc7, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0x3f, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0xfb, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x1f, 0xff, 0xfe, 0x00, 0x02, 0x3f, 0xff, 0xff, 0xfe, 0x07, 0x9f, 0xff, 0xff, 0xfe, 0x00, 
	0x00, 0x1f, 0xe0, 0xff, 0x00, 0x03, 0x1f, 0xff, 0xff, 0xfc, 0x43, 0xc7, 0xff, 0xff, 0xfe, 0x00, 
	0x00, 0x1f, 0xe0, 0xfe, 0x00, 0x03, 0x9f, 0xff, 0xff, 0xfd, 0xf3, 0xc0, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x0f, 0xc0, 0x7c, 0x00, 0x07, 0xdf, 0xff, 0xff, 0xfb, 0xf9, 0xe0, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x07, 0xc0, 0x78, 0x00, 0x07, 0xcf, 0xff, 0xff, 0xff, 0xf9, 0xf8, 0x07, 0xff, 0xff, 0x80, 
	0x00, 0x01, 0xe0, 0xe0, 0x00, 0x07, 0xcf, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0xf1, 0xe0, 0x00, 0x07, 0xcf, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x07, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x07, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x1f, 0xf1, 0xff, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x1f, 0xe0, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
	0x00, 0x1f, 0xc0, 0x7f, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
	0x00, 0x0f, 0xc0, 0x7e, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
	0x00, 0x03, 0xe0, 0xf8, 0x00, 0x01, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  // Declare SSD1306 library
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {  
  Serial.begin (9600);

  // Set Pinmodes for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set Pinmodes for LEDS
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledG, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(DT_IN), update, CHANGE);

   // Set Pinmodes for the rotary encoder pins
   pinMode (CLK_IN, INPUT);
   pinMode (DT_IN, INPUT);
   pinMode(SW_IN, INPUT_PULLUP);

   // SSD1306_SWITCHCAPVCC tells display to generate voltage from 3.3V internally
   // The address for 128x32 OLEDS is 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation  failed"));

    // if the display is not found don't continue, loop until the end of time! or reset whichever comes first
    for(;;);
  }

  // Clear the display's buffer
  display.clearDisplay();

  // Set the rotation of the display, 2 for horizontal
  display.setRotation (2);

  // display.drawBitmap(x position, y position, bitmap  data, bitmap width, bitmap height, color)
  display.drawBitmap(0, 0, startBitmap, 128, 32, WHITE);

  // Show whatever is in the display buffer on the screen
  // You MUST call display() after drawing commands or they wont show up on the screen!
  display.display();
  delay(3000);
}

void loop(){

  // Read the digital value of the button (LOW/HIGH)
  SWITCH_STATE = digitalRead(SW_IN);

  // If the button is pressed (LOW), then print message
  if (SWITCH_STATE == LOW) {
    // Serial.println("Switch pressed");
    COUNT = DISTANCE_1 - LIGHT_1_S;
  }

  long duration, distance;

  // Detect the rotation of the rotary encoder and if the button is pressed, if it feels like it...
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); //2
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance from the wall to the car. If the distance is less than 0 you probably hit the wall...
  distance = (duration / 2) / 73.914; //29.1(cm)

  // Calculate the distance for each light to turn on
  LIGHT_1 = COUNT + LIGHT_1_S;
  LIGHT_2 = LIGHT_1 + LIGHT_2_S;
  LIGHT_3 = LIGHT_2 + LIGHT_3_S;
  DISTANCE_1 = distance;

  // Red light ON (STOP!!!)
  if  (distance <= LIGHT_1) {
    digitalWrite(ledR,HIGH);
    digitalWrite(ledY,LOW);
    digitalWrite(ledG,LOW);
  }
  // Yellow light ON (Do not go very fast!)
  else if (distance > LIGHT_1 && distance < LIGHT_2) { 
    digitalWrite(ledR,LOW);
    digitalWrite(ledY,HIGH);
    digitalWrite(ledG,LOW);
  }
  // Green light ON (Go as fast as you want I guess?)
  else if (distance > LIGHT_2 && distance < LIGHT_3) {
    digitalWrite(ledR,LOW);
    digitalWrite(ledY,LOW);
    digitalWrite(ledG,HIGH);
  }
  // All lights OFF (So it doesn't stay on when your car isn't in the garage)
  else {
    digitalWrite(ledR,LOW);
    digitalWrite(ledY,LOW);
    digitalWrite(ledG,LOW);
  }

  displayStopDist();
  display.display();
}

void update() {
  // Read the current state of the CLK_IN pin
  CURRENT_CLK_STATE = digitalRead(CLK_IN);

  // Check if the button is pressed, if it is then change the set point to the current distance reading
  if ((PREVIOUS_CLK_STATE == LOW) && (CURRENT_CLK_STATE == HIGH)) {

    if  (digitalRead(DT_IN) == HIGH) { 
      COUNT--;
    }
    else  {
      COUNT++;
    }

  } 
  PREVIOUS_CLK_STATE = CURRENT_CLK_STATE;
}

void displayStopDist() {
  // Define d as the stop distance and c as the current distance to the car
  float d = LIGHT_1;
  float c = DISTANCE_1;
  // Clear the display (Dump it in the trash!)
  display.clearDisplay();
  // Set the color - always use white unless the display is RGB then use whatever color you want I guess...
  display.setTextColor(WHITE);
  // Set the font size (I made it small :) )
  display.setTextSize(1);
  // Title (So you can change it if you really want to...)
  display.setCursor(0 ,0);
  display.print("Parking Stop Helper");
	
  // Stop Distance Display
  display.setCursor(0 ,12); 
  display.print("Stop Dist: ");
  display.setTextSize(1);
  display.print(d ,0);
  display.print("in");
	
  // Current distance display
  display.setTextSize(1);
  display.setCursor(0 ,25);
  display.print("Current Dist: ");
  display.print(c ,0);
  display.print("in");
}
// The End!
