// 
//    FILE: dht11.h
// VERSION: 0.3.2
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
//     URL: http://arduino.cc/playground/Main/DHT11Lib
//
// HISTORY:
// George Hadjikyriacou - Original version
// see dht.cpp file
// 

#ifndef dht11_h
#define dht11_h

#include "Arduino.h"

#define DHT11LIB_VERSION "0.3.2"

class dht11
{
public:
	double dewPoint();
	double dewPoint(double celsius, double humidity);
	double dewPointFast();
	double dewPointFast(double celsius, double humidity);
	double fahrenheit();
	double fahrenheit(double celsius);
	double kelvin();
	double kelvin(double celsius);
    int8_t read(int8_t pin);
    int8_t humidity;
    int8_t temperature;
};
#endif
//
// END OF FILE
//
