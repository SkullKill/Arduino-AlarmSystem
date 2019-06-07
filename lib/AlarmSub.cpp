/*
 * AlarmSub.cpp
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#include <AlarmSub.h>
#include <Network.h>

/*
 * Create AlarmSub class without any argument
 * please run AlarmSub.setup after to set the arguments.
 */
AlarmSub::AlarmSub()
{
	_buzzerPin = 0;
	_buzzerDuration = 0;
	_armedPin = 0;
	//setupCommon();
}

/*
 * Create AlarmSub class initializing it with
 * the structure AlarmSubStruct
 */
AlarmSub::AlarmSub(AlarmSubStruct_t *AlarmSubStruct)
{
	//initialize
	_buzzerPin = AlarmSubStruct->buzzerPin;
	_buzzerDuration = AlarmSubStruct->buzzerDuration;
	_alarmSensorTotal = AlarmSubStruct->alarmSensorTotal;
	_alarmSubSensorStruct = AlarmSubStruct->alarmSubSensorStruct;
	_alarmSubSensorStruct->sensorName = AlarmSubStruct->sensorNames;
	_armedPin = AlarmSubStruct->armedPin;
	_sirenPin1 = AlarmSubStruct->sirenPin1;
	_sirenDuration1 = AlarmSubStruct->sirenDuration1;
	_sirenPin2 = AlarmSubStruct->sirenPin2;
	_sirenDuration2 = AlarmSubStruct->sirenDuration2;
	_strobePin = AlarmSubStruct->strobePin;
	//_network = &(*AlarmSubStruct->network);
	setupCommon();
}

/*
 * Create AlarmSub class initializing it with
 * all the arguments manually (not always maintained)
 * do not use, not maintained
 */
AlarmSub::AlarmSub(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin, uint8_t sirenPin)
{
	//initialize
	_buzzerPin = buzzerPin;
	_buzzerDuration = buzzerDuration;
	_armedPin = armedPin;
//	_sirenPin1 = sirenPin1;
//	_sirenPin2 = sirenPin2;
	setupCommon();
}

/*
 * Setup AlarmSub class after it has been created.
 * with the structure AlarmSubStruct
 */
void AlarmSub::setup(AlarmSubStruct_t *AlarmSubStruct)
{
	_buzzerPin = AlarmSubStruct->buzzerPin;
	_buzzerDuration = AlarmSubStruct->buzzerDuration;
	_alarmSensorTotal = AlarmSubStruct->alarmSensorTotal;
	_alarmSubSensorStruct = AlarmSubStruct->alarmSubSensorStruct;
	_alarmSubSensorStruct->sensorName = AlarmSubStruct->sensorNames;
	_armedPin = AlarmSubStruct->armedPin;
	_sirenPin1 = AlarmSubStruct->sirenPin1;
	_sirenDuration1 = AlarmSubStruct->sirenDuration1;
	_sirenPin2 = AlarmSubStruct->sirenPin2;
	_sirenDuration2 = AlarmSubStruct->sirenDuration2;
	_strobePin = AlarmSubStruct->strobePin;
	//_network = &(*AlarmSubStruct->network);

	Serial.print("buzzer Structpin");
	Serial.println(AlarmSubStruct->buzzerPin);
	Serial.print("Buzzer pin");
	Serial.println(_buzzerPin);
	Serial.print("strobe Structpin");
	Serial.println(AlarmSubStruct->strobePin);
	Serial.print("Strobe pin");
	Serial.println(_strobePin);

	setupCommon();
}

/*
 * Setup AlarmSub class after it has been created.
 * with all the arguments manually (not always maintained)
 * do not use, not maintain
 */
void AlarmSub::setup(uint8_t buzzerPin, uint8_t buzzerDuration, uint8_t armedPin, uint8_t sirenPin)
{
	_buzzerPin = buzzerPin;
	_buzzerDuration = buzzerDuration;
	_armedPin = armedPin;
//	_sirenPin = sirenPin;
	setupCommon();
}

/*
 * common argument that need to be initialized no mater what.
 */
void AlarmSub::setupCommon()
{
	_syslogSection = "AlarmSub";
	_syslogDelay = (5 * SEC);
	_syslog_sent = false;
	//_syslogU_sent = false;
	//_sylogA_w.resetreset();
	//_sylogU_w.resetreset();
	_sirenSwitch1.setup(_sirenPin1);
	_sirenSwitch2.setup(_sirenPin2);
	_strobeSwitch.setup(_strobePin);
	pinMode(_armedPin, OUTPUT);
	digitalWrite(_armedPin, LOW);
	_alarmSensorStartPin = 0;				// Statically Assign Sensor Start Pin
	_alarmSensorLEDStartPin = 30;			// Statically Assign Sensor LED Start Pin
	// Initialize all sensor LED pins
	uint8_t i = _alarmSensorLEDStartPin;
	uint8_t _alarmSensorLEDEndPin = _alarmSensorLEDStartPin + _alarmSensorTotal;
	while ((_alarmSensorLEDEndPin - i) > 0)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
		i++;
	}
	//pinMode(30, OUTPUT);
	//digitalWrite(30, LOW);
	//pinMode(A0, INPUT);		// this is if analog were to be used as digital pins?

	_armed = 0;
	_arming = 0;
	_alarmNow = 0;
	_alarmMultiple = 0;
	_buzzerFrequency = 261;

	//String sensorNames[] = {"MasterBedroom" , "2nd" , "3rd" , "4th" , "5th"};


	uint8_t iSensorStatus = 0;
	while ((_alarmSensorTotal - iSensorStatus) > 0)
	{
		_alarmSubSensorStruct->sensorStatus[iSensorStatus] = 0;
		iSensorStatus++;
	}
	_alert = false;
}

/*
 * Start the Arming procedure of the alarm
 */
void AlarmSub::arming()
{
	_arming = 1;
	noTone(_buzzerPin);
	tone(_buzzerPin, 300, _buzzerDuration);
	delay(_buzzerDuration);
	tone(_buzzerPin, 500, _buzzerDuration);
	delay(_buzzerDuration);
	tone(_buzzerPin, 700, _buzzerDuration);
	_strobeSwitch.noInterOn(15 * SEC);
	Syslog.logger(local7Slog, warnSlog, _syslogSection, "Arming");
}

/*
 * Start the disArming procedure of the alarm
 * done instantly compared to the Arming
 */
void AlarmSub::unArming()
{
	_armed = 0;
	digitalWrite(_armedPin, LOW);
	noTone(_buzzerPin);
	tone(_buzzerPin, 700, _buzzerDuration);
	delay(_buzzerDuration);
	tone(_buzzerPin, 500, _buzzerDuration);
	delay(_buzzerDuration);
	tone(_buzzerPin, 300, _buzzerDuration);
	// just in case it was arming
	_arming = 0 ;
	_alarmNow = 0 ;
	//_alarmMultiple = 0 ;
	_strobeSwitch.noInterOn(2 * SEC);
	Syslog.logger(local7Slog, warnSlog, _syslogSection, "UnArming");
}

/*
void AlarmSub::sirenOn()
{
	digitalWrite(_sirenPin, HIGH);
	//Serial.println(" Siren ON");
}

void AlarmSub::sirenOff()
{
	digitalWrite(_sirenPin, LOW);
}
*/

/*
 * in case panic button press, will instantly turn on sirens and strobes.
 * will also set alarm to armed state
 */
void AlarmSub::panic()
{
	//alarmArm(0);
	_armed = 1;
	//_sirenSwitch.on(_sirenDuration * MIN);
	_sirenSwitch1.on(uint32_t(_sirenDuration1) * uint32_t(MIN));
	_sirenSwitch2.on(uint32_t(_sirenDuration2) * uint32_t(MIN));
	_strobeSwitch.on();
	Syslog.logger(local7Slog, alertSlog, _syslogSection, "Panic Triggered");
}


/*
 * Check status of sensor, and turn on/off associaated led if needed.
 * if armed, will turn on siren if sensor not in normal state
 * if not armed, will only turn on siren if tamper switch detected.
 * normal = power to sensor, no movement detected, temper switch off.
 */
uint8_t AlarmSub::alarmCheck(uint8_t channelInput, uint8_t channelLed)
{
	uint8_t _sensorStatus;
	_sensorStatus = checkSensor(channelInput, channelLed);
	//String _syslogTmp;
	 //Serial.print("Sensor: ");
	 //Serial.print(channelInput);
	 //Serial.print(" : ");
	 //Serial.println(_sensorStatus);
	if (_arming)		// this could probably be moved to alarmCheckAll instead
	{
		alarmArm(17 * SEC);		// static delay before turning on alarm <<<<<<<<<<<<<<<<<<<<<<<<< need to be dynamic
	}
	if (_armed)
	{
		if (_sensorStatus != 1)			// if sensor status not normal
		{
			_sirenSwitch1.on(uint32_t(_sirenDuration1) * uint32_t(MIN));
			_sirenSwitch2.on(uint32_t(_sirenDuration2) * uint32_t(MIN));
			_strobeSwitch.on();
			//_network->emailAlert();
			//emailAlert();
			//Network::emailAlert();
			_alert = true;
			/*
			_sylogA_w.reset();				// only send one message every _syslogDelay
			if (!_syslogA_sent)
			{
				_syslogTmp = "Armed and ";
				_syslogTmp += *querySensorStatus(channelInput);							// <<<<<<<<<<<<<<<WORK PROVIDED THAT USING PIN 0 as START pin for sensor (BUG POTENTIAL)
				Syslog.logger(local7Slog, alertSlog, _syslogSection, _syslogTmp);
				_syslogA_sent = true;
			}
			if (_sylogA_w.wait(_syslogDelay))
			{
				_syslogA_sent = false;
			}
			*/
		}
	}
	else
	{
		switch (_sensorStatus)
		{
			case 0:							// tempering detected (shorted wire)
				_armed = 1 ;
				_sirenSwitch1.on(uint32_t(_sirenDuration1) * uint32_t(MIN));
				_sirenSwitch2.on(uint32_t(_sirenDuration2) * uint32_t(MIN));
				_strobeSwitch.on();
				_alert = true;
	    		break;
			case 1:							// if normal
				_sirenSwitch1.off();
				_sirenSwitch2.off();
				_strobeSwitch.off();
				_alert = false;
		    	break;
		    case 2:							// if movement
				_sirenSwitch1.off();
				_sirenSwitch2.off();
				_strobeSwitch.off();
				_alert = false;
				/*
				_sylogU_w.reset();			// only send one message every _syslogDelay
				if (!_syslogU_sent)
				{
					_syslogTmp = "NotArm ";
					_syslogTmp += *querySensorStatus(channelInput);							// <<<<<<<<<<<<<<<WORK PROVIDED THAT USING PIN 0 as START pin for sensor (BUG POTENTIAL)
					Syslog.logger(local7Slog, infoSlog, _syslogSection, _syslogTmp);
					_syslogU_sent = true;
				}
				if (_sylogU_w.wait(_syslogDelay))
				{
					_syslogU_sent = false;
				}
				*/
		    	break;
		    case 3:							// if tamper switch open, open circuit
		    	_armed = 1 ;
				_sirenSwitch1.on(uint32_t(_sirenDuration1) * uint32_t(MIN));
				_sirenSwitch2.on(uint32_t(_sirenDuration2) * uint32_t(MIN));
				_strobeSwitch.on();
				_alert = true;
		    	break;
		}
	}
/*	if (siren)
	{
		digitalWrite(sirenPin, HIGH);
	}
	else
	{
		digitalWrite(sirenPin, LOW);
	}
*/
	return (_sensorStatus);			// not used for anything at the moment

}

/*
 * Check all the sensors status, and turn on/off siren when needed
 * also check all siren and strobe TimedSwitch. for delay switching
 * the main loop need to be doing this check in an endless loop.
 */
void AlarmSub::alarmCheckAll()
{
	uint8_t i = _alarmSensorStartPin;
	uint8_t iLED = _alarmSensorLEDStartPin;
	uint8_t iSensorID = 0;
	String _syslogTmp;
	while (i < _alarmSensorTotal)
	{
		_alarmSubSensorStruct->sensorStatus[iSensorID] = alarmCheck(i, iLED);		// check alarm status, and store result in array.

		// send syslog if needed
		if ((_armed && (_alarmSubSensorStruct->sensorStatus[iSensorID] != 1)) || (_alarmSubSensorStruct->sensorStatus[iSensorID] == 2))	// if (armed and not normal) or movement
		{
			_sylog_w.reset();				// only send one message every _syslogDelay
			if (!_syslog_sent)
			{
				if (_armed)
				{
					_syslogTmp = "Armed and ";
					_syslogTmp += *querySensorStatus(iSensorID);
					Syslog.logger(local7Slog, alertSlog, _syslogSection, _syslogTmp);
					_syslog_sent = true;
				}
				else
				{
					_syslogTmp = "NotArm ";
					_syslogTmp += *querySensorStatus(iSensorID);
					Syslog.logger(local7Slog, infoSlog, _syslogSection, _syslogTmp);
					_syslog_sent = true;
				}
			}
		}
		if (_sylog_w.wait(_syslogDelay))
		{
			_syslog_sent = false;
		}

		i++;
		iLED++;
		iSensorID++;
	}
//	alarmCheck(0, 30);						// need to pass argument from upstream
//	alarmCheck(channelAInput, channelALed);
//	alarmCheck(channelBInput, channelBLed);
//	alarmCheck(channelCInput, channelCLed);
//	alarmCheck(channelDInput, channelDLed);
//	alarmCheck(channelEInput, channelELed);
//	alarmCheck(channelFInput, channelFLed);
	_sirenSwitch1.check();
	_sirenSwitch2.check();
	_strobeSwitch.check();
}


/**
 * Checks the state of a sensor and reports it to the connected host
 * state 0 = tempering detected (shorted wire)
 * state 1 = normal
 * state 2 = movement detected
 * state 3 = tamper switch triggered, open circuit, cut wire.
 * better to use TimedSwitch later ?? to prevent turn on pin when pin already on
 */
uint8_t AlarmSub::checkSensor(uint8_t sensorInput, uint8_t statusOutput)
{
  uint8_t state;
  int sensorReading = analogRead(sensorInput);
  if( sensorReading < 400 ) {
    state = 0;                        // Wire shorted. Possible tampering.
    digitalWrite(statusOutput, HIGH); // Turn the associated status LED on
  } else if ( sensorReading >= 400 && sensorReading < 590 ) {
    state = 1;                        // Normal state, sensor not triggered
    digitalWrite(statusOutput, LOW); // Turn the associated status LED off
  } else if ( sensorReading >= 590 && sensorReading < 800 ) {
    state = 2;                        // Sensor triggered.
    digitalWrite(statusOutput, HIGH); // turn the associated status LED on
  } else {
    state = 3;                        // Open circuit. Cut or tamper triggered.
    digitalWrite(statusOutput, HIGH); // Turn the associated status LED on
  }
  // Output the current reading to the host via the serial connection
  /*
  Serial.print(sensorInput, DEC);
  Serial.print(": ");
  Serial.print(sensorReading, DEC);
  Serial.print(" (");
  Serial.print(state, DEC);
  Serial.println(") ");
  */
  // Pass the current state back to the calling function
  return state;
}

bool AlarmSub::checkAlert()
{
	if (_alert)
	{
		return true;
	}
	else
	{
		return false;
	}
}

String *AlarmSub::queryArmedStatus()
{

	//uint8_t _sensorStatus = _alarmSubSensorStruct.sensorStatus[sensorID];
	//returnStatus = "Alarm: ";
	returnStatus = "Armed= ";
	returnStatus += String(_armed);
	returnStatus += (" , ");
	if (_armed)
	{
		returnStatus += ("Armed");
	}
	else
	{
		returnStatus += ("unArmed");
	}
	return(&returnStatus);
	//return("status");
}

String *AlarmSub::querySirenStatus()
{

	//uint8_t _sensorStatus = _alarmSubSensorStruct.sensorStatus[sensorID];
	//returnStatus = "Alarm: ";
	returnStatus = "Siren= ";
	uint8_t status = _sirenSwitch1.status();
	returnStatus += String(status);
	returnStatus += (" , ");
	if (status)
	{
		returnStatus += ("On");
	}
	else
	{
		returnStatus += ("Off");
	}
	return(&returnStatus);
	//return("status");
}

String *AlarmSub::queryStrobeStatus()
{

	//uint8_t _sensorStatus = _alarmSubSensorStruct.sensorStatus[sensorID];
	//returnStatus = "Alarm: ";
	returnStatus = "Strobe= ";
	uint8_t status = _strobeSwitch.status();
	returnStatus += String(status);
	returnStatus += (" , ");
	if (status)
	{
		returnStatus += ("On");
	}
	else
	{
		returnStatus += ("Off");
	}
	return(&returnStatus);
	//return("status");
}

String *AlarmSub::querySensorStatus(uint8_t sensorID)
{
	/*
	String status("  Sensor ");
	//status.operator +=("he");
	status += "hey";
	String ola = "569";
	status += ola;
	Serial.println(status);
	*/
	//Serial.println(_alarmSubSensorStruct.sensorName[0]);

	uint8_t _sensorStatus = _alarmSubSensorStruct->sensorStatus[sensorID];
	//Serial.print("Sensor id ");
	//Serial.print(sensorID);
	//Serial.print(" = ");
	//Serial.println(_sensorStatus);
	returnStatus = "Sensor: ";
	returnStatus += String(sensorID);
	returnStatus += (" , ");
	returnStatus += _alarmSubSensorStruct->sensorName[sensorID];
	returnStatus += (" = ");
	switch (_sensorStatus)
	{
		case 0:							// tempering detected (shorted wire)
			returnStatus += ("0 , Tempering detected, Shorted wire");
    		break;
		case 1:							// if normal
			returnStatus += ("1 , Normal");
	    	break;
	    case 2:							// if movement
	    	returnStatus += ("2 , Movement Detected");
	    	break;
	    case 3:							// if tamper switch open, open circuit
	    	returnStatus += ("3 , Tamper Switch Open, open circuit");
	    	break;
	}
	//String *mem = &returnStatus;
	//Serial.println(int(mem));
	//Serial.println(returnStatus);
	return(&returnStatus);
	//return("status");
}

/*
 * Delay before arming alarm, will beep at every second interval
 */
void AlarmSub::alarmArm(uint32_t delay)
{
	uint32_t _diff = 0 ;
	if (_alarmNow == 0)
	{
		Serial.println("ResetAlarmTime");
		_alarmNow = millis();
	}
	else
	{
		_diff = millis() - _alarmNow ;
		if ( _diff < delay )
		{
			if ((_diff > (1000 * _alarmMultiple)) && (_diff < ((1000 * _alarmMultiple) + 1000)))
			{
				//Serial.println("diff");
				Serial.println("arming");
				Serial.println("Beep");
				_alarmMultiple++ ;
		    	noTone(_buzzerPin);
		    	tone(_buzzerPin, _buzzerFrequency, _buzzerDuration);
			}
		}
		else
		{
			_armed = 1 ;
	    	digitalWrite(_armedPin, HIGH);
			_arming = 0 ;
			_alarmNow = 0 ;
			_alarmMultiple = 0 ;
		    noTone(_buzzerPin);
		    tone(_buzzerPin, 900, _buzzerDuration*10);
		}
	}
}

/*
 * toogle arming, if arming, (delay to arm, cancel and stay disArmed)
 * if arm, disarm,
 * if disarm, fo into arming mode (delay arm)
 */
void AlarmSub::armingToggle()
{
    if (_arming)
    {
/*    	arming = 0 ;
		alarmNow = 0 ;
		alarmMultiple = 0 ;
		Serial.println("Not arming");
		noTone(buzzerPin);
		tone(buzzerPin, 700, buzzerDuration);
		delay(buzzerDuration+200);
		tone(buzzerPin, 700, buzzerDuration);
		delay(buzzerDuration+200);
		tone(buzzerPin, 700, buzzerDuration);
*/
    	unArming();
    }
    else
    {
    	if (_armed)
    	{
    		unArming();
    	}
    	else
    	{
    		arming();
    	}
    }
}
