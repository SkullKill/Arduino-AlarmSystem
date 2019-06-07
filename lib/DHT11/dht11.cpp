//
//    FILE: dht11.cpp
// VERSION: 0.3.2
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
// HISTORY:
// George Hadjikyriacou - Original version (??)
// Mod by SimKard - Version 0.2 (24/11/2010)
// Mod by Rob Tillaart - Version 0.3 (28/03/2011)
// + added comments
// + removed all non DHT11 specific code
// + added references
//

#include "dht11.h"

double dht11::dewPoint()
{
	return (dewPoint((double) temperature, (double) humidity));
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
double dht11::dewPoint(double celsius, double humidity)
{
	double A0= 373.15/(273.15 + celsius);
	double SUM = -7.90298 * (A0-1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
	SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM-3) * humidity;
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558-T);
}

double dht11::dewPointFast()
{
	return (dewPointFast((double) temperature, (double) humidity));
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dht11::dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
}

double dht11::fahrenheit()
{
	return (fahrenheit((double) temperature));
}

/*-----( Declare User-written Functions )-----*/
//
//Celsius to Fahrenheit conversion
double dht11::fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

double dht11::kelvin()
{
	return (kelvin((double) temperature));
}

//Celsius to Kelvin conversion
double dht11::kelvin(double celsius)
{
	return celsius + 273.15;
}

// returnvalues:
//  0 : OK
// -1 : checksum error
// -2 : timeout
int8_t dht11::read(int8_t pin)
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (uint8_t i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// ACKNOWLEDGE or TIMEOUT
	uint16_t loopCnt = 10000;
	while(digitalRead(pin) == LOW)
		if (loopCnt-- == 0) return -2;

	loopCnt = 10000;
	while(digitalRead(pin) == HIGH)
		if (loopCnt-- == 0) return -2;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (uint8_t i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return -2;

		uint32_t t = micros();

		loopCnt = 10000;
		while(digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return -2;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
        // as bits[1] and bits[3] are allways zero they are omitted in formulas.
	humidity    = bits[0]; 
	temperature = bits[2]; 
	/*
	Serial.print("RH ");
	Serial.println((int)bits[0]);
	Serial.print("RH decimal ");
	Serial.println((int)bits[1]);
	Serial.print("Temp decimal ");
	Serial.println((int)bits[1]);
	*/
	uint8_t sum = bits[0] + bits[2];  

	if (bits[4] != sum) return -1;
	return 0;
}
//
// END OF FILE
//
