/*
 * AlarmSystem.cpp
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#include <AlarmSystem.h>

/*
 * Create AlarmSystem class without any argument
 * please run AlarmSystem.setup after to set the arguments.
 */
AlarmSystem::AlarmSystem()
{
	/*
	_something = 0;
	//initialise , setup etc
	// create an AlarmSub
	//_alarmSub.setup(buzzerPin, buzzerDuration, armedPin);
	_AlarmSubStruct.buzzerPin = 0;
	_AlarmSubStruct.buzzerDuration = 0;
	_AlarmSubStruct.armedPin = 0;
	//_AlarmSubStruct.sirenPin = 0;
	//_alarmSub.setup(&_AlarmSubStruct);
	//AlarmSub _alarmSub(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin);
//	_armRemote = 0;
//	_unarmRemote = 0;
//	_garageRemote = 0;
//	_panicRemote = 0;
	//_garagePin = 0;
	//_homeAuto.setup(&_homeAutoStruct);
	_remoteStruct.startPin = 0;
	//setupCommon();
	 */
}

/*
 * Create AlarmSystem class initializing it with
 * the structure AlarmSystemStruct
 */
AlarmSystem::AlarmSystem(AlarmStruct_t *AlarmStruct)
{
	//sSerial.begin(9600);
	_something = 0;
	//initialise , setup etc
	// create an AlarmSub
	//_alarmSub.setup(buzzerPin, buzzerDuration, armedPin);
	_powerPin= AlarmStruct->powerPin;
	_alarmSubStruct = &(*AlarmStruct->alarmSubStruct);
	//AlarmSub _alarmSub(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin);
//	_armRemote=AlarmStruct->armRemote;
//	_unarmRemote=AlarmStruct->unarmRemote;
//	_garageRemote=AlarmStruct->garageRemote;
//	_panicRemote=AlarmStruct->panicRemote;
	//_garagePin=AlarmStruct->garagePin;
	_homeAutoStruct = &(*AlarmStruct->homeAutoStruct);
	//_homeAutoStruct.garagePin = AlarmStruct->garagePin;
	_remoteStruct.startPin = AlarmStruct->remoteStartPin;
	_networkStruct = &(*AlarmStruct->networkStruct);
	//Serial.println("setup debug");
	setupCommon();
}

/*
 * Create AlarmSystem class initializing it with
 * all the arguments manually (not always maintained)
 * do not use, not maintained
 *
AlarmSystem::AlarmSystem(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin, uint8_t sirenPin)
{
	_something = 0;
	//initialise , setup etc
	// create an AlarmSub
	//_alarmSub.setup(buzzerPin, buzzerDuration, armedPin);
	_AlarmSubStruct.buzzerPin= buzzerPin;
	_AlarmSubStruct.buzzerDuration = buzzerDuration;
	_AlarmSubStruct.armedPin = armedPin;
	//_AlarmSubStruct.sirenPin = sirenPin;
	_alarmSub.setup(&_AlarmSubStruct);
	//AlarmSub _alarmSub(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin);
//	_armRemote=0;
//	_unarmRemote=0;
//	_garageRemote=0;
//	_panicRemote=0;
//	_panicRemote=0;
	//_garagePin = 0;
	setupCommon();
}
*/

/*
 * Setup AlarmSystem class after it has been created.
 * with the structure AlarmSystemStruct
 */
void AlarmSystem::setup(AlarmStruct_t *AlarmStruct)
{
	Serial.begin(9600);
	_something = 0;
	//initialise , setup etc
	// create an AlarmSub
	//_alarmSub.setup(buzzerPin, buzzerDuration, armedPin);
	_powerPin= AlarmStruct->powerPin;
	_alarmSubStruct = &(*AlarmStruct->alarmSubStruct);
	//AlarmSub _alarmSub(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin);
//	_armRemote=AlarmStruct->armRemote;
//	_unarmRemote=AlarmStruct->unarmRemote;
//	_garageRemote=AlarmStruct->garageRemote;
//	_panicRemote=AlarmStruct->panicRemote;
	//_garagePin=AlarmStruct->garagePin;
	_homeAutoStruct = &(*AlarmStruct->homeAutoStruct);
	//_homeAutoStruct.garagePin = AlarmStruct->garagePin;
	_remoteStruct.startPin = AlarmStruct->remoteStartPin;
	_networkStruct = &(*AlarmStruct->networkStruct);
	setupCommon();
}

/*
 * common argument that need to be initialized no mater what.
 */
void AlarmSystem::setupCommon()
{
	pinMode(_powerPin, OUTPUT);
	digitalWrite(_powerPin, HIGH);
	Serial.print("Garage StructBeforepin");
	Serial.println(_homeAutoStruct->garagePin);
	_alarmSub.setup(&(*_alarmSubStruct));
	_homeAuto.setup(&(*_homeAutoStruct));
	_remoteStruct.alarmSub = &_alarmSub;
	_remoteStruct.homeAuto = &_homeAuto;
	_remote.setup(&_remoteStruct);
	_networkStruct->alarmSub = &_alarmSub;
	_networkStruct->homeAuto = &_homeAuto;
	_network.setup(&(*_networkStruct));
	//_AlarmSubStruct.network = &_network;
}

/*
 * check description of subclass
 */
void AlarmSystem::checkRemote()
{
	_remote.check();
}

/*
 * check description of subclass
 */
void AlarmSystem::checkHomeAuto()
{
	_homeAuto.checkGarage();
	_homeAuto.checkDHT11();
}

/*
 * check description of subclass
 */
void AlarmSystem::armingToggle()
{
	_alarmSub.armingToggle();
}

/*
 * check description of subclass
 */
void AlarmSystem::alarmCheckAll()
{
	_alarmSub.alarmCheckAll();
}

/*
 * check description of subclass
 */
void AlarmSystem::networkCheckAll()
{
	_network.check();
	_network.emailCheck();
}
