/*
 * HomeAuto.h
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#ifndef HOMEAUTO_H
#define HOMEAUTO_H

#include <Arduino.h>
#include <TimedSwitch.h>
//#include <Waiter.h>
#include <Syslog/Syslog.h>
#include <DHT11/dht11.h>

struct HomeAutoStruct_t {uint8_t buff; uint8_t garagePin; uint8_t lightPin; uint8_t lightDuration;};
typedef HomeAutoStruct_t HoAS_t;

/*
 * Class for Home Automation
 * anything that deals with Home Automation will be here.
 * e.g door/windows control, environment/home theater control
 */
class HomeAuto
{
public:
	HomeAuto();
	HomeAuto(HomeAutoStruct_t *HomeAutoStruct);
	HomeAuto(uint8_t garagePin);
	void setup(HomeAutoStruct_t *HomeAutoStruct);
	void setup(uint8_t garagePin);
	void toggleGarage();
	void checkGarage();
	void checkDHT11();
	dht11 DHT11;
	String *queryTempSensor();
	String tempSensorStr;
	char dewPoint[16];
private:
	String _syslogSection;
	uint8_t _garagePin;
	uint8_t _lightPin;
	bool _togGarage;
	void setupCommon();
//	Waiter debounceDelay;
	Waiter _switchStateDelay_w;
	Waiter _DHT11Delay_w;
	bool _DHT11firstRun;
	uint32_t _DHTreadDelay;
	uint16_t _switchStateDelay;
	bool _switchOn;
	uint8_t _lightDuration;
	TimedSwitch _lightSwitch;
};



#endif /* HOMEAUTO_H */
