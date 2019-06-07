/*
 * HomeAuto.cpp
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#include <HomeAuto.h>

/*
 * Create the Home Automation class without any argument
 * please run HomeAuto.setup after to set the arguments.
 */
HomeAuto::HomeAuto()
{
	//_garagePin = 0;
	//setupCommon();
}

/*
 * Create Home Automation class initializing it with
 * the structure HomeAutoStruct
 */
HomeAuto::HomeAuto(HomeAutoStruct_t *HomeAutoStruct)
{
	_garagePin = HomeAutoStruct->garagePin;
	_lightPin = HomeAutoStruct->lightPin;
	_lightDuration = HomeAutoStruct->lightDuration;
	setupCommon();
}

/*
 * Create Home Automation class initializing it with
 * all the arguments manually (not always maintained)
 */
HomeAuto::HomeAuto(uint8_t garagePin)
{
	_garagePin = garagePin;
	setupCommon();
}

/*
 * Setup Home Automation class after it has been created.
 * with the structure HomeAutoStruct
 */
void HomeAuto::setup(HomeAutoStruct_t *homeAutoStruct)
{
	_garagePin = homeAutoStruct->garagePin;
	_lightPin = homeAutoStruct->lightPin;
	_lightDuration = homeAutoStruct->lightDuration;
	Serial.print("Garage Structpin");
	Serial.println(homeAutoStruct->garagePin);
	Serial.print("Garage pin");
	Serial.println(_garagePin);
	setupCommon();
}

/*
 * Setup Home Automation class after it has been created.
 * with all the arguments manually (not always maintained)
 */
void HomeAuto::setup(uint8_t garagePin)
{
	_garagePin = garagePin;
	setupCommon();
}

/*
 * common argument that need to be initialized no mater what.
 */
void HomeAuto::setupCommon()
{
	_syslogSection = "HomeAuto";
	pinMode(_garagePin, OUTPUT);
	digitalWrite(_garagePin, LOW);
	_togGarage = false;
	_switchStateDelay = 500;			// relay stay on for "delay"
	_switchOn = true;
	_lightSwitch.setup(_lightPin);
	tempSensorStr = "Temp ";
	_DHT11firstRun = false;
	_DHTreadDelay = ((uint32_t)15 * (uint32_t)MIN);
}

/*
 * to open or close the garage door, call this function
 */
void HomeAuto::toggleGarage()
{
	_togGarage = true;
	//Serial.println("Toggle Garage Called");
	//_switchStateDelay_w.resetreset();
	Syslog.logger(local7Slog, noticeSlog, _syslogSection, "Toogle Garage");
}

/*
 * Check if Garage key has been press,
 * if it has, will turn on relay for X ms. 500ms is usually a good value to start with.
 * the main loop need to be doing this check in an endless loop.
 * p.s could have been done using TimedSwitch
 */
void HomeAuto::checkGarage()
{
	if (_togGarage)
	{
		_switchStateDelay_w.reset();
		if (_switchOn)
		{
			_lightSwitch.on(uint32_t(_lightDuration) * uint32_t(MIN));
			digitalWrite(_garagePin, HIGH);
			Serial.println("Garage RelayOn");
			//Serial.print("Garage pin");
			//Serial.println(_garagePin);
			//Serial.print("light pin");
			//Serial.println(_lightPin);
			Syslog.logger(local7Slog, debugSlog, _syslogSection, "Garage RelayOn");
			_switchOn = false;
		}
		if (_switchStateDelay_w.wait(_switchStateDelay))
		{
			Serial.println("Garage Relay Off");
			Syslog.logger(local7Slog, debugSlog, _syslogSection, "Garage RelayOff");
			digitalWrite(_garagePin, LOW);
			_togGarage = false;
			_switchOn = true;
		}
	}
	_lightSwitch.check();
}

void HomeAuto::checkDHT11()
{
	uint8_t dht11Pin = 29;											// <<<<need to be dynamic
	_DHT11Delay_w.reset();
	if ((_DHT11Delay_w.wait(_DHTreadDelay)) || (!_DHT11firstRun))
	{
		//Serial.println("\n");
		//Serial.println(millis());
		//Syslog.logger(local7Slog, debugSlog, _syslogSection, "Before Temp Read");
		int8_t chk = DHT11.read(dht11Pin);
		//Syslog.logger(local7Slog, debugSlog, _syslogSection, "After Temp Read");
		//Serial.print("Read sensor: ");

		switch (chk)
		{
		case 0:
			//Syslog.logger(local7Slog, debugSlog, _syslogSection, "Temp return ok");
			tempSensorStr = "Temp ";
			tempSensorStr += DHT11.temperature;
			tempSensorStr += " oC, Humidity ";
			tempSensorStr += DHT11.humidity;
			tempSensorStr += " %, Dew Point ";
			//Syslog.logger(local7Slog, debugSlog, _syslogSection, "Before Temp duePoint convertion");
			//dewPoint = "      ";
			//char dewPoint[10];
			dtostrf(DHT11.dewPoint(), 1, 2, dewPoint);
			tempSensorStr += dewPoint;
			tempSensorStr += " oC";
			Serial.println(tempSensorStr);
			Syslog.logger(local7Slog, noticeSlog, _syslogSection, tempSensorStr);
			/*
		    	Serial.println("OK");
		    	Serial.print("Humidity (%): ");
		    	Serial.println((float)DHT11.humidity, 2);

		    	Serial.print("Temperature (oC): ");
		    	Serial.println((float)DHT11.temperature, 2);
		    	Serial.println(millis());

		    	Serial.print("Temperature (oF): ");
		    	Serial.println(DHT11.fahrenheit(), 2);

		    	Serial.print("Temperature (K): ");
		    	Serial.println(DHT11.kelvin(), 2);

		    	Serial.print("Dew Point (oC): ");
		    	Serial.println(DHT11.dewPoint());

		    	Serial.print("Dew PointFast (oC): ");
		    	Serial.println(DHT11.dewPointFast());
		    	Serial.println(millis());
		    	*/
	    	break;
		case -1:
			Serial.println("Checksum error");
			break;
		case -2:
			Serial.println("Time out error");
			break;
		default:
			Serial.println("Unknown error");
			break;
		}
		_DHT11firstRun = true;
	}
}

String *HomeAuto::queryTempSensor()
{
	return &tempSensorStr;
}
