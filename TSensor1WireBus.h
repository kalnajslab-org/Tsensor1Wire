
//////////// Created by Doug Goetz June 2020
//////////// Built to simplify the use of the DS18B20 1-Wire temperature sensor that is on a single bus 
///// June 2020 - tested with a Teensy 3.6 on DIO pin 4
///// June 2020 - µ-SOT DS18B20 tested



#ifndef TSensor1WireBus_h
#define TSensor1WireBus_h

#include <OneWire.h> //found in applications: Teensyduino/contents/Java/hardware/teensy/avr/libraries


class TSensor1Bus: public OneWire 
{
	private:
	public:

	TSensor1Bus(int sensorpin);
	//function to get sensor address

	enum OneWireState : uint8_t {
		STARTUP,
		MEASURING,
		READY
	};

	byte i;
  	byte present = 0;
  	byte type_s;
  	byte data[9];
  	byte sensoraddr[8];
	uint8_t Mode = STARTUP;
	unsigned long oneWtimer = 0;
	unsigned long convtime = 750; //ms
 	//float celsius;

	void PrintSensorAddress();

	bool ValidateAddrCRC();

	// function to start conversion and set conversion in progress flag
	void StartSensorConversion();
	
	// read scratchpad data
	void ReadSensor();

	// validate checksum from read data
	bool ValidateScrathPadCRC();
	
	float ReturnTemperature();
	//function to convert data to celcius and return converted temperature

	float GetTemperature();

	void ManageState(float &TVAR);
	
};

#endif