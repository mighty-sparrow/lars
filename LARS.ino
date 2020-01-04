/**
 * AUTHOR:     Nick Romans
 *  
 *  SUMMARY:    
 *              This sketch uses a barometric pressure sensor to determine the
 *              initial altitude of an object.  The Arduino will continue to poll
 *              the sensor to determine if the object has an altitude lower than
 *              the last reading.  If the object is falling, a servo will be
 *              activated.
 *  
 *  COMPONENTS:
 *              SG90 Servo
 *              BB3115  Barometric Pressure Sensor
 *              Piezoelectric Buzer
 */

#include <Servo.h>
#include <Wire.h>               // for IIC communication

/**
 * BAROMETRIC PRESSURE SENSOR
 * This device should be connected to the I2C pins. On Arduino Pro Mini,
 * this is likely the A4 (SDA) and A5 (SCL).
 * 
 * See also: https://www.arduino.cc/en/reference/wire
 */

const int SENSORADDRESS = 0x60; // address specific to the MPL3115A1, 
                                // value found in datasheet
                                
const float DIFF_TRIGGER = 1.0;// Number of positive, vertical change before
                               // monitoring negative change. These units are
                               // measured in "meters".
                               
float trigger_altitude = 0;    // Stores the elevation at which the switch is triggered.
float last_altitude = 0;       // Stores the elevation last used to compare descent.

float altitude, temperature;

boolean triggered = false;     // 0 if we haven't passed the DIFF_TRIGGER
                               // threshold.
boolean dropping = false;      // 0 if we aren't dropping in elevation,
                               // otherwise 1 (if dropping from the sky);
boolean deployed = false;      // 1 if we're done.
/**
 * PIEZOELECTRIC BUZZER
 */

 #include "pitches.h"

// notes in the melody:
int melody[] =       {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int melody_init[] =  {NOTE_G3, NOTE_F4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4,4,4,4,4 };

/**
 * SERVO
 */

Servo myservo;                // create servo object to control a servo 
                              // a maximum of eight servo objects can be created
int myServoPin = 9;           // The pin number on which the servo is connected.
 
int pos = 0;                  // variable to store the servo position 

/**
 * BUTTONS AND INPUTS
 */

const int buttonPin = 8;      // the number of the pushbutton pin

const int resetPin = 7;       // the number of the pushbutton pin
const int ledPin =  13;       // the number of the LED pin

// variables will change:
int buttonState = 0;           // variable for reading the pushbutton status
int resetPinState = 0;         // variable for reading the pushbutton status
//int triggerState = 0;        // 0 if in original position;
 
void setup() 
{ 
  myservo.attach(myServoPin);  // attaches the servo on pin 9 to the servo object 
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
//  pinMode(buttonPin, INPUT);
//  pinMode(resetPin, INPUT);
  
  /*******************************************/
  Wire.begin();        // join i2c bus
  Serial.begin(9600);  // start serial for output
  
  // This is a basic II2 communication check. If the sensor doesn't
  // return decicmal number 196 (see 0x0C register in datasheet), 
  // "IIC bad" is printed, otherwise nothing happens. 
 if(IIC_Read(0x0C) == 196) //checks who_am_i bit for basic I2C handshake test
 {
   Serial.println("Connected to MPL3115A2."); // Output we are connected to the computer.
   Serial.print("trigger_altitude:  ");
   Serial.println(trigger_altitude);
   Serial.print("last_altitude:  ");
   Serial.println(last_altitude);
   Serial.print("triggered:  ");
   Serial.println(triggered);
   Serial.print("dropping:  ");
   Serial.println(dropping);
   Serial.println("===============================");
 }
 else
 {
   Serial.println("No sensor found.");
 }
  // Enable and configure the sensor. 
  sensor_config();
  init_recovery();
}

void loop()
{
  sensor_read_data();
  where_are_we();
}
