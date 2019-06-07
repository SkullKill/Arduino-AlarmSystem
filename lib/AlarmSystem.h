/*
 * AlarmSystem.h
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#ifndef ALARMSYSTEM_H
#define ALARMSYSTEM_H

#include <AlarmSub.h>
#include <HomeAuto.h>
#include <Remote.h>
#include <Network.h>
//#include <Waiter.h>


struct AlarmStruct_t
{
	uint8_t powerPin;
	AlarmSubStruct_t* alarmSubStruct;
	uint8_t remoteStartPin;
//	uint8_t unarmRemote;
//	uint8_t garageRemote;
//	uint8_t panicRemote;
	//uint8_t garagePin;
	HomeAutoStruct_t* homeAutoStruct;
	NetworkStruct_t* networkStruct;
};
typedef AlarmStruct_t AlS_t;

/*
 * Main AlarmSystem Class, this is what you should be creating in your main program.
 * this will create and manage all the other sub classes. e.g AlarmSub, HomeAuto, Remote, etc...
 * ties everything together. your main class interact only to this class. this in turn interact with the required sub class.
 */
class AlarmSystem
{
public:
	AlarmSystem();
	AlarmSystem(AlarmStruct_t *AlarmStruct);
	AlarmSystem(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin, uint8_t sirenPin);
	void setup(AlarmStruct_t *AlarmStruct);
	void setupCommon();
	void dothing();  // example to be remove
	void checkRemote();
	void alarmCheckAll();
	void checkHomeAuto();
	void armingToggle();	// to be remove
	void networkCheckAll();
private:
	int _something;
	AlarmSub _alarmSub;
	AlarmSubStruct_t* _alarmSubStruct;
	uint8_t _powerPin;
	HomeAuto _homeAuto;
	HomeAutoStruct_t* _homeAutoStruct;
	RemoteStruct_t _remoteStruct;
	NetworkStruct_t* _networkStruct;
	Remote _remote;
	Network _network;
	//AlarmSubSensorStruct_t _alarmSubSensorStruct;
};


#endif /* ALARMSYSTEM_H */
