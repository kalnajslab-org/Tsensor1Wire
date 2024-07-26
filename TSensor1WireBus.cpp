
#include "TSensor1WireBus.h"

TSensor1Bus::TSensor1Bus(int sensorpin){

	OneWire();
	begin(sensorpin);

	search(sensoraddr);
	reset_search();
	delay(250);

}

void  TSensor1Bus::PrintSensorAddress()
{
	
	Serial.print("Sensor Address =");
	for( int i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(sensoraddr[i], HEX);
  	}
  	
  	Serial.println();
	
}

void TSensor1Bus::StartSensorConversion()
{

	reset();
	skip();
	write(0x44, 1);

	//Serial.println("Start Conversion");
	
	oneWtimer = millis();
}

void TSensor1Bus::ReadSensor()
{
	reset();
	skip();   
	write(0xBE);         // Read Scratchpad
	
	//Serial.println("Read Sensor");


	for ( int i = 0; i < 9; i++) { //
	    data[i] = read();
	}

}

bool TSensor1Bus::ValidateAddrCRC()
{
	//Serial.println(crc8(sensoraddr,7), HEX);

	if (crc8(sensoraddr, 7) != sensoraddr[7]) {
      	Serial.println("CRC is not valid!");
      	return 0;
	}	  
	else{
		return 1;
	}	  
}

bool TSensor1Bus::ValidateScrathPadCRC()
{

	//Serial.println(crc8(data,8), HEX);

	if (crc8(data, 8) != data[8]) {
      	Serial.println(" Scatchpad CRC is not valid!");
      	return 0;
	}	  
	else{
		return 1;
	}	  

}

float TSensor1Bus::ReturnTemperature()
{
	float celsius;

		ReadSensor();  

		if(!ValidateScrathPadCRC()){

			celsius = -999; // bad reading
		}

		else{
			int32_t rawbin = (data[1] << 8) | data[0];
			celsius = rawbin / 16.0;
			//Serial.println(celsius);
		}

	return celsius;
}


void TSensor1Bus::ManageState(float &TVAR){

	switch (Mode){
	
	case STARTUP:

		Mode = MEASURING;
		//Serial.println("in startup");
		StartSensorConversion();
		delay(750);
		break;

	case MEASURING:
		//Serial.println("measuring");
		if(millis()-oneWtimer>=convtime){
		  Mode = READY;
		}
		break;

	case READY:

		//Serial.println("ready");
		
		TVAR = ReturnTemperature();
		Mode = MEASURING;
		StartSensorConversion();
		//Serial.println(TVAR);
		break;

	default:
		Mode = STARTUP;
		break;

	}
}