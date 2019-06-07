/*
 * TimedSwitch.h
 *
 *  Created on: 21/10/2012
 *      Author: SkullKill
 */

#ifndef TIMEDSWITCH_H
#define TIMEDSWITCH_H


#include <Arduino.h>
#include <Waiter.h>

struct TimedSwitchStruct_t {uint8_t switchPin;};
typedef TimedSwitchStruct_t TiSS_t;

/*
 * Class for TimeSwitch
 * used to turn on or off pins.
 * if pin is already HIGH (on). it will not try to turn pin HIGH again. same for off.
 * can also be used with a delay argument. this will toggle switch, leave on this state for a period "duration" and then toogle back.
 * for example, if TimeSwitch.on(500) , will turn on for 500 ms, then turn off.
 */
class TimedSwitch
{
public:
	TimedSwitch();
	TimedSwitch(TimedSwitchStruct_t *TimedSwitchStruct);
	TimedSwitch(uint8_t switchPin);
	void setup(TimedSwitchStruct_t *TimedSwitchStruct);
	void setup(uint8_t switchPin);
	void on();
	void off();
	void on(uint32_t duration);
	void off(uint32_t duration);
	void noInterOn(uint32_t duration);
	void noInterOff(uint32_t duration);
	void check();
	uint8_t status();
private:
	uint8_t _switchPin;
	uint32_t _duration;
	bool _switchState;
	bool _switchToggle;
	bool _switchToggle1st;
	void setupCommon();
	bool _noInter;
	Waiter _SwitchOnForDuration_w;
};


#endif /* TIMEDSWITCH_H */
