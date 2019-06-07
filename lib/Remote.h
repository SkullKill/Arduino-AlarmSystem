/*
 * Remote.h
 *
 *  Created on: 21/10/2012
 *      Author: SkullKill
 */

#ifndef REMOTE_H
#define REMOTE_H


#include <Arduino.h>
#include <Waiter.h>
#include <AlarmSub.h>
#include <HomeAuto.h>
#include <Syslog/Syslog.h>

struct RemoteStruct_t {uint8_t startPin; AlarmSub *alarmSub; HomeAuto *homeAuto;};
typedef RemoteStruct_t ReS_t;

/*
 * Class for Remote Control
 * imports the AlarmSub class and HomeAuto class and interact with them directly
 * does not actually "import" but, use the address of the class to modify the same class created earlier.
 * does not copy the class, uses the same class previously created.
 */
class Remote
{
public:
	Remote();
	Remote(RemoteStruct_t *RemoteStruct);
	Remote(uint8_t startPin);
	void setup(RemoteStruct_t *RemoteStruct);
	void setup(uint8_t startPin);
	void check();
private:
	String _syslogSection;
	AlarmSub * _alarmSub;
	HomeAuto * _homeAuto;
	//RemoteStruct_t * RemoteStructLocal;
	uint8_t _startPin;
	uint8_t _armRemote;
	uint8_t _unarmRemote;
	uint8_t _garageRemote;
	uint8_t _panicRemote;
	void setupCommon();
//	Waiter debounceDelay;
	uint16_t _switchStateDelay;
	bool _switch[4];
	Waiter _switchStateDelay_w[4];
	bool _switchOn[4];
	void switching();
};



#endif /* REMOTE_H */
