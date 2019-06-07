/*
 * AlarmSub.h
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#ifndef ALARMSUB_H
#define ALARMSUB_H

#include <Arduino.h>
#include <TimedSwitch.h>
//#include <Network.h>
#include <Syslog/Syslog.h>

struct AlarmSubSensorStruct_t {String *sensorName; uint8_t sensorStatus[];};

struct AlarmSubStruct_t
{
	uint8_t buzzerPin;
	uint8_t buzzerDuration;
	uint8_t alarmSensorTotal;
	String *sensorNames;
	AlarmSubSensorStruct_t *alarmSubSensorStruct;
	uint8_t armedPin;
	uint8_t sirenPin1;
	uint8_t sirenDuration1;
	uint8_t sirenPin2;
	uint8_t sirenDuration2;
	uint8_t strobePin;
	//Network *network;
};
typedef AlarmSubStruct_t AlSS_t;

/*
 * Class for Alarm SubSystem
 * manage all the alarm hardware and interaction. e.g manage, sensors, siren. armed state, etc
 */
class AlarmSub
{
public:
	AlarmSub();
	AlarmSub(AlarmSubStruct_t *AlarmSubStruct);
	AlarmSub(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin, uint8_t sirenPin);
	void setup(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin, uint8_t sirenPin);
	void setup(AlarmSubStruct_t *AlarmSubStruct);
	void setupCommon();
	void arming();
	void unArming();
	//void sirenOn();		// using TimedSwitch instead
	//void sirenOff();		// using TimedSwitch instead
	void panic();
	void alarmCheckAll();
	void armingToggle();
	bool checkAlert();
	//String *querySensorStatus(uint8_t sensorID);
	String *querySensorStatus(uint8_t sensorID);
	String *queryArmedStatus();
	String *querySirenStatus();
	String *queryStrobeStatus();
	String returnStatus;
	//friend class Network;
private:
	String _syslogSection;
	Waiter _sylog_w;
	//Waiter _sylogU_w;
	uint16_t _syslogDelay;
	bool _syslog_sent;
	//bool _syslogU_sent;
	bool _alert;
	//Network* _network;
	uint8_t _buzzerPin;
	uint8_t _buzzerDuration;
	uint8_t _alarmSensorTotal;
	uint8_t _alarmSensorStartPin;
	uint8_t _alarmSensorLEDStartPin;
	uint8_t _armedPin;
	bool _armed;
	bool _arming;
	uint32_t _alarmNow;
	uint8_t _sirenPin1;
	uint8_t _sirenDuration1;
	uint8_t _sirenPin2;
	uint8_t _sirenDuration2;
	uint8_t _strobePin;
	uint8_t alarmCheck(uint8_t channelInput, uint8_t channelLed);
	uint8_t checkSensor(uint8_t sensorInput, uint8_t statusOutput);
	void alarmArm(uint32_t delay);
	//Waiter _alarmDelay_w;
	uint32_t _alarmMultiple;
	uint16_t _buzzerFrequency;
	TimedSwitch _sirenSwitch1;
	TimedSwitch _sirenSwitch2;
	TimedSwitch _strobeSwitch;
	//uint8_t _sensorStatus[];
	//char* _sensorName[];
	AlarmSubSensorStruct_t* _alarmSubSensorStruct;

};



#endif /* ALARMSUB_H */
