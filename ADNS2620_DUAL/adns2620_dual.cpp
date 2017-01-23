/* Arduino ADNS2620 Library
 * Can be used to interface between an ATmega328 (Arduino) and the ADNS2620 Mouse Sensor
 * Copyright 2010 SparkFun ElectronicS
 * Written by Ryan Owens
*/

#include <avr/pgmspace.h>
#include "adns2620_dual.h"
#include "Arduino.h"

//Constructor sets the pins used for the mock 'i2c' communication
ADNS2620_DUAL::ADNS2620_DUAL(int sda1, int sda2, int scl)
{
	_sda1 = sda1;
	_sda2 = sda2;
	_scl = scl;
}

//Configures the communication pins for their initial state
void ADNS2620_DUAL::begin()
{
	pinMode(_sda1, OUTPUT);
	pinMode(_sda2, OUTPUT);
	pinMode(_scl, OUTPUT);
}

//Essentially resets communication to the ADNS2620 module
void ADNS2620_DUAL::sync()
{
    digitalWrite(_scl, HIGH);
    delay(1);
	digitalWrite(_scl, LOW);
    delay(1);
	digitalWrite(_scl, HIGH);
    delay(100);
}

//Reads a register from the ADNS2620 sensor. Returns the result to the calling function.
//Example: value = mouse.read(CONFIGURATION_REG);
struct ADNS2620_Return ADNS2620_DUAL::read(char address)
{
    char value1=0;
	char value2=0;
	pinMode(_sda1, OUTPUT); //Make sure the SDIO pin is set as an output.
	pinMode(_sda2, OUTPUT);
    digitalWrite(_scl, HIGH); //Make sure the clock is high.
    address &= 0x7F;    //Make sure the highest bit of the address byte is '0' to indicate a read.
 
    //Send the Address to the ADNS2620
    for(int address_bit=7; address_bit >=0; address_bit--){
        digitalWrite(_scl, LOW);  //Lower the clock
        pinMode(_sda1, OUTPUT); //Make sure the SDIO pin is set as an output.
		pinMode(_sda2, OUTPUT);
        
        //If the current bit is a 1, set the SDIO pin. If not, clear the SDIO pin
        if(address & (1<<address_bit)){
            digitalWrite(_sda1, HIGH);
			digitalWrite(_sda2, HIGH);
        }
        else{
            digitalWrite(_sda1, LOW);
			digitalWrite(_sda2, HIGH);
        }
        delayMicroseconds(10);
        digitalWrite(_scl, HIGH);
        delayMicroseconds(10);
    }
    
    delayMicroseconds(120);   //Allow extra time for ADNS2620 to transition the SDIO pin (per datasheet)
    //Make SDIO an input on the microcontroller
    pinMode(_sda1, INPUT);	//Make sure the SDIO pin is set as an input.
	pinMode(_sda2, INPUT);
	digitalWrite(_sda1, HIGH); //Enable the internal pull-up
	digitalWrite(_sda2, HIGH);
        
    //Send the Value byte to the ADNS2620
    for(int value_bit=7; value_bit >= 0; value_bit--){
        digitalWrite(_scl, LOW);  //Lower the clock
        delayMicroseconds(10); //Allow the ADNS2620 to configure the SDIO pin
        digitalWrite(_scl, HIGH);  //Raise the clock
        delayMicroseconds(10);
        //If the SDIO pin is high, set the current bit in the 'value' variable. If low, leave the value bit default (0).    
		//if((ADNS_PIN & (1<<ADNS_sda)) == (1<<ADNS_sda))value|=(1<<value_bit);
		if(digitalRead(_sda1))value1 |= (1<<value_bit);
		if(digitalRead(_sda2))value2 |= (1<<value_bit);

    }
    
	//char retval[2] = {value1, value2};
	struct ADNS2620_Return retval;
	retval.data[0] = value1;
	retval.data[1] = value2;
    return retval;
}	

//Writes a value to a register on the ADNS2620.
//Example: mouse.write(CONFIGURATION_REG, 0x01);
void ADNS2620_DUAL::write(char address, char value)
{
	pinMode(_sda1, OUTPUT);	//Make sure the SDIO pin is set as an output.
	pinMode(_sda2, OUTPUT);
    digitalWrite(_scl, HIGH);          //Make sure the clock is high.
    address |= 0x80;    //Make sure the highest bit of the address byte is '1' to indicate a write.

    //Send the Address to the ADNS2620
    for(int address_bit=7; address_bit >=0; address_bit--){
        digitalWrite(_scl, LOW); //Lower the clock
        
        delayMicroseconds(10); //Give a small delay (only needed for the first iteration to ensure that the ADNS2620 relinquishes
                    //control of SDIO if we are performing this write after a 'read' command.
        
        //If the current bit is a 1, set the SDIO pin. If not, clear the SDIO pin
        if(address & (1<<address_bit))digitalWrite(_sda1, HIGH);
        else digitalWrite(_sda1, LOW);
		if(address & (1<<address_bit))digitalWrite(_sda2, HIGH);
        else digitalWrite(_sda2, LOW);
        delayMicroseconds(10);
        digitalWrite(_scl, HIGH);
        delayMicroseconds(10);
    }
    
    //Send the Value byte to the ADNS2620
    for(int value_bit=7; value_bit >= 0; value_bit--){
        digitalWrite(_scl, LOW);  //Lower the clock
        //If the current bit is a 1, set the SDIO pin. If not, clear the SDIO pin
        if(value & (1<<value_bit))digitalWrite(_sda1, HIGH);
        else digitalWrite(_sda1, LOW);
		if(value & (1<<value_bit))digitalWrite(_sda2, HIGH);
        else digitalWrite(_sda2, LOW);
        delayMicroseconds(10);
        digitalWrite(_scl, HIGH);
        delayMicroseconds(10);
    }
}