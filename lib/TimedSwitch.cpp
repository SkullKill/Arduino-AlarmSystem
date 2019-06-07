/*
 * TimedSwitch.cpp
 *
 *  Created on: 21/10/2012
 *      Author: SkullKill
 */




#include <TimedSwitch.h>

/*
 * Create TimedSwitch class without any argument
 * please run TimedSwitch.setup after to set the arguments.
 */
TimedSwitch::TimedSwitch()
{
	//_switchPin = 0;
	//setupCommon();
}

/*
 * Create TimedSwitch class initializing it with
 * the structure TimedSwitchStruct
 */
TimedSwitch::TimedSwitch(TimedSwitchStruct_t *TimedSwitchStruct)
{

	_switchPin = TimedSwitchStruct->switchPin;

	setupCommon();
}

/*
 * Create TimedSwitch class initializing it with
 * all the arguments manually (not always maintained)
 */
TimedSwitch::TimedSwitch(uint8_t switchPin)
{
	_switchPin = switchPin;
	setupCommon();
}

/*
 * Setup TimedSwitch class after it has been created.
 * using structure TimedSwitchStruct
 */
void TimedSwitch::setup(TimedSwitchStruct_t *TimedSwitchStruct)
{
	_switchPin = TimedSwitchStruct->switchPin;
	setupCommon();
}

/*
 * SetupTimedSwitch class after it has been created.
 * using the arguments manually (not always maintained)
 */
void TimedSwitch::setup(uint8_t switchPin)
{
	_switchPin = switchPin;
	setupCommon();
}

/*
 * common argument that need to be initialized no mater what.
 */
void TimedSwitch::setupCommon()
{
	pinMode(_switchPin, OUTPUT);
	digitalWrite(_switchPin, LOW);
	_switchState = false;		// off
	_switchToggle = false;
	_switchToggle1st = false;
	_noInter = false;
	//Serial.println("switch tb initialise");
}

/*
 * Turn on switch, will reset any pending off delay if any. cannot reset noInterOff.
 * will not run if switch is already on
 */
void TimedSwitch::on()
{
	if (!_noInter)
	{
		if (!_switchState)
		{
			_SwitchOnForDuration_w.resetreset();
			_switchToggle = false;
			_switchToggle1st = false;
			digitalWrite(_switchPin, HIGH);
			_switchState = true;
		}
	}
	//Serial.println("switch tb on ");
}

/*
 * Turn on switch for duration define by argument, then turn off.
 * will not run if switch is already on.
 * will reset any pending off delay if any. cannot reset noInterOff.
 */
void TimedSwitch::on(uint32_t duration)
{
	if (!_noInter)
	{
		if (!_switchState)
		{
			if (!_switchToggle)
			{
				_SwitchOnForDuration_w.resetreset();
				_switchToggle = true;
				_duration = duration;
			}
		}
	}
	//Serial.println("switch on for");
}

/*
 * Turn on switch for duration define by argument, then turn off. it cannot be reset by any other calls.
 * will not run if switch is already on.
 * will reset any pending off delay if any. cannot reset noInterOff.
 */
void TimedSwitch::noInterOn(uint32_t duration)
{
	if (!_noInter)
	{
		if (!_switchState)
		{
			if (!_switchToggle)
			{
				_SwitchOnForDuration_w.resetreset();
				_switchToggle = true;
				_noInter = true;
				_duration = duration;
			}
		}
	}
	//Serial.println("switch on for, no int");
}

/*
 * Turn off switch, will reset any pending on delay if any. cannot reset noInterOn.
 * will not run if switch is already off
 */
void TimedSwitch::off()
{
	if (!_noInter)
	{
		if (_switchState)
		{
			_SwitchOnForDuration_w.resetreset();
			_switchToggle = false;
			_switchToggle1st = false;
			digitalWrite(_switchPin, LOW);
			_switchState = false;
		}
	}
	//Serial.println("switch tb off");
}

/*
 * Turn off switch for duration define by argument, then turn on.
 * will not run if switch is already off.
 * will reset any pending on delay if any. cannot reset noInterOn.
 */
void TimedSwitch::off(uint32_t duration)
{
	if (!_noInter)
	{
		if (_switchState)
		{
			if (!_switchToggle)
			{
				_SwitchOnForDuration_w.resetreset();
				_switchToggle = true;
				_duration = duration;
			}
		}
	}
	//Serial.println("switch tb off for");
}

/*
 * Turn off switch for duration define by argument, then turn on. it cannot be reset by any other calls.
 * will not run if switch is already off.
 * will reset any pending on delay if any. cannot reset noInterOn.
 */
void TimedSwitch::noInterOff(uint32_t duration)
{
	if (!_noInter)
	{
		if (_switchState)
		{
			if (!_switchToggle)
			{
				_SwitchOnForDuration_w.resetreset();
				_switchToggle = true;
				_noInter = true;
				_duration = duration;
			}
		}
	}
	//Serial.println("switch tb off for, no int");
}

/*
 * Check if timed passed the argument "duration" if to toggle 2nd time,
 * the main loop need to be doing this check in an endless loop, if the timed switched are to be used
 * not needed of only on , off are used without any delay
 */
void TimedSwitch::check()
{
	if (_switchToggle)
	{
		_SwitchOnForDuration_w.reset();
		if (!_switchToggle1st)
		{
			//Serial.println("ToogleSwitch1st");
			//Serial.println(millis());
			if (_switchState)
			{
				digitalWrite(_switchPin, LOW);
				_switchState = false;
			}
			else
			{
				digitalWrite(_switchPin, HIGH);
				_switchState = true;
			}
			_switchToggle1st = true;
		}
		if (_SwitchOnForDuration_w.wait(_duration))
		{
			//Serial.println("ToogleSwitch2nd");
			//Serial.println(millis());
			if (_switchState)
			{
				digitalWrite(_switchPin, LOW);
				_switchState = false;
			}
			else
			{
				digitalWrite(_switchPin, HIGH);
				_switchState = true;
			}
			_switchToggle = false;
			_switchToggle1st = false;
			_noInter = false;
		}
		else
		{
			//do nothing
			//Serial.println("Wait");
			//Serial.println(millis());
		}
	}
}

uint8_t TimedSwitch::status()
{
	if(_switchState)
	{
		return 1;		// on
	}
	else
	{
		return 0;		// off
	}
}
