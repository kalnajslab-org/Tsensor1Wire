#include "TSensor1WireBus.h"

#define sensor 4 //put 1-wire sensor pin here

float sensorval; //use a float to store last temperature measurement
long timer1; //used for non-blocking millis() timer  

TSensor1Bus TempC(sensor); //instance of TSensor1Bus library that inherits from the 1-wire library.

void setup() {

  Serial.begin(9600);
  
  TempC.PrintSensorAddress(); //use to print sensor address if needed. The address is retrieved from the sensor with the TSensor1Bus object instance
  
  if(!TempC.ValidateAddrCRC()){ //use to check that a valid sensor address is being retreived. 
    Serial.println("sensor crc bad, check sensor connection");
   }

  timer1 = millis();

}

void loop() {

  TempC.ManageState(sensorval); //use this in the main loop outside of any control timers. A non blocking internal 750ms timer is used to between the start of the sensor conversion and when the conversion is ready to be read.

  if(millis()-timer1>20000){ // example of how to retrieve a sample every 20s
    
    Serial.print("20s sample: ");
    Serial.println(sensorval);
    timer1 = millis(); 
  } 

  
 // if(!TempC.ValidateScrathPadCRC()){ //use to check that a valid sensor address is being retreived
  //  Serial.println("scatch crc bad, dont use record");
  // }


}
