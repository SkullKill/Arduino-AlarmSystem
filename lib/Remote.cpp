/*
 * Remote.cpp
 *
 *  Created on: 21/10/2012
 *      Author: SkullKill
 */

#include <Remote.h>

/*
 * Create the Remote class without any argument
 * please run Remote.setup after to set the arguments.
 */
Remote::Remote()
{
	_startPin = 0;
	//setupCommon();
}

/*
 * Create Remote class initializing it with
 * the structure RemoteStruct
 */
Remote::Remote(RemoteStruct_t *RemoteStruct)
{
	//RemoteStructLocal = &(*RemoteStruct);
	_startPin = RemoteStruct->startPin;
	_alarmSub = &(*RemoteStruct->alarmSub);
	_homeAuto = &(*RemoteStruct->homeAuto);
	setupCommon();
}

/*
 * Create Remote class initializing it with
 * all the arguments manually (not always maintained)
 */
Remote::Remote(uint8_t startPin)
{
	_startPin = startPin;
	setupCommon();
}

/*
 * Setup Remote class after it has been created.
 * using structure RemoteStruct
 */
void Remote::setup(RemoteStruct_t *RemoteStruct)
{
	_startPin = RemoteStruct->startPin;
	_alarmSub = &(*RemoteStruct->alarmSub);
	_homeAuto = &(*RemoteStruct->homeAuto);
	setupCommon();
}

/*
 * Setup Home Automation class after it has been created.
 * using all the arguments manually (not always maintained)
 */
void Remote::setup(uint8_t startPin)
{
	_startPin = startPin;
	setupCommon();
}

/*
 * common argument that need to be initialized no mater what.
 * has been statically configured for a remote of 4 button.
 * look at code of sensor setup in AlarmSub to convert to dynamic
 */
void Remote::setupCommon()
{
	_syslogSection = "Remote";
	uint8_t _i = _startPin;
	uint8_t _endPin = (_startPin + 4);
	while((_endPin - _i) > 0 )
	{
		pinMode(_i, INPUT);
		digitalWrite(_i, LOW);
		_i++;
	}
	_armRemote = _endPin;
	_unarmRemote = (_endPin - 1);
	_garageRemote = (_endPin - 2);
	_panicRemote = (_endPin - 3);
	_switchStateDelay = 1000;			// 600 a bit too low
	uint8_t i = 0;
	while (i < 4)
	{
		_switch[i] = false;
		_switchOn[i] = true;
		i++;		// start at 0, finish at 3, increment to 4, then stop
	}
}

/*
 * Check if Remote button has been press. read from hardware.
 * check all 4 pins, one by one, and store info in array.
 */
void Remote::check()
{
	uint8_t i = 0;
	uint8_t iRem = _startPin;
	while (i < 4)
	{
		if (digitalRead(iRem))
		{
			//Serial.println(" Remote key ");
			//Serial.println(" swithOn ");
			//Serial.println(_switchOn[i]);
			if (_switchOn[i])
			{
				//Serial.println("set remote key");
				_switch[i] = true;
			}
		}
		i++;
		iRem++;
	}
	switching();
}

/*
 * check in array to see what key has been pressed, and do the corresponding action.
 */
void Remote::switching()
{
	uint8_t i = 0;
	while (i < 4)
	{
		if (_switch[i])			// if switch pressed
		{
			_switchStateDelay_w[i].reset();
			if (_switchOn[i])	// if already trigged, will not trigger again, until switchStateDelay expired
			{
				switch (i)
				{
				case 0:		// Remote Arm
					Serial.println("Remote Arm");
					Syslog.logger(local7Slog, noticeSlog, _syslogSection, "Arm");
					//RemoteStructLocal->alarmSub->arming();
					_alarmSub->arming();
					break;
				case 1:		// Remote unArm
					Serial.println("Remote UnArm");
					Syslog.logger(local7Slog, noticeSlog, _syslogSection, "UnArm");
					//RemoteStructLocal->alarmSub->unArming();
					_alarmSub->unArming();
					break;
				case 2:		// Garage door
					Syslog.logger(local7Slog, noticeSlog, _syslogSection, "Garage");
					Serial.println("Remote Garage");
					_homeAuto->toggleGarage();
					break;
				case 3:		// panic
					Syslog.logger(local7Slog, noticeSlog, _syslogSection, "Panic");
					Serial.println("Remote Panic");
					_alarmSub->panic();
					break;
				default:
					break;
				}
				_switchOn[i] = false;
			}
			if (_switchStateDelay_w[i].wait(_switchStateDelay))		// after delay, reset flag, to allow function to be called again.
			{
				_switch[i] = false;
				_switchOn[i] = true;
			}
		}
		i++;	// check next button.
	}
}

