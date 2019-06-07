/*
 * Network.cpp
 *
 *  Created on: 25/10/2012
 *      Author: SkullKill
 */

#include <Network.h>


//boolean reading = false;

////////////////////////////////////////////////////////////////////////
//CONFIGURE
////////////////////////////////////////////////////////////////////////
/*
  byte ip[] = { 192, 168, 33, 90 };   //Manual setup only
  byte gateway[] = { 192, 168, 33, 254 }; //Manual setup only
  byte subnet[] = { 255, 255, 255, 0 }; //Manual setup only

  // if need to change the MAC address (Very Rare)
  byte mac[] = { 0x00, 0x1D, 0xB3, 0x00, 0x21, 0xF7 };		// ProCurve Networking by HP

  EthernetServer server = EthernetServer(80); //port 80
*/

////////////////////////////////////////////////////////////////////////

/*
 * Create the Remote class without any argument
 * please run Remote.setup after to set the arguments.
 */
Network::Network()
{
	/*
	  byte ip[] = { 192, 168, 33, 90 };   //Manual setup only
	  byte gateway[] = { 192, 168, 33, 254 }; //Manual setup only
	  byte subnet[] = { 255, 255, 255, 0 }; //Manual setup only

	  // if need to change the MAC address (Very Rare)
	  byte mac[] = { 0x00, 0x1D, 0xB3, 0x00, 0x21, 0xF7 };		// ProCurve Networking by HP
	  uint8_t webPort= 80; //port 80
	  char secret[] = "secret";
	//_startPin = 0;
	//setupCommon();
	//Ethernet.begin(mac);
	Ethernet.begin(mac, ip, gateway, subnet); //for manual setup
	server.setup(80);
	//server.setup(webPort);
	//RemoteStructLocal = &(*RemoteStruct);
	//_startPin = NetworkStruct->startPin;
	//_alarmSub = &(*NetworkStruct->alarmSub);
	//_homeAuto = &(*NetworkStruct->homeAuto);
	_secret = (secret);
	setupCommon();
	*/
}

/*
 * Create Remote class initializing it with
 * the structure RemoteStruct
 */
Network::Network(NetworkStruct_t *NetworkStruct)
{
	//Ethernet.begin(mac);
	Ethernet.begin(NetworkStruct->mac, NetworkStruct->ip, NetworkStruct->dnsServer, NetworkStruct->gateway, NetworkStruct->subnet); //for manual setup
	//server.setup(80);
	server.setup(NetworkStruct->webPort);
	//RemoteStructLocal = &(*RemoteStruct);
	//_startPin = NetworkStruct->startPin;
	_smtpServer = NetworkStruct->smtpServer;
	Syslog.setLoghost(NetworkStruct->logHost);
	_alarmSub = &(*NetworkStruct->alarmSub);
	_homeAuto = &(*NetworkStruct->homeAuto);
	_secret = (NetworkStruct->secret);
	_alarmSensorTotal = NetworkStruct->alarmSensorTotal;
	setupCommon();
}

/*
 * Create Remote class initializing it with
 * all the arguments manually (not always maintained)
 */
Network::Network(uint8_t startPin)
{
	_startPin = startPin;
	setupCommon();
}

/*
 * Setup Remote class after it has been created.
 * using structure RemoteStruct
 */
void Network::setup(NetworkStruct_t *NetworkStruct)
{
	//Ethernet.begin(mac);
	Ethernet.begin(NetworkStruct->mac, NetworkStruct->ip, NetworkStruct->dnsServer, NetworkStruct->gateway, NetworkStruct->subnet); //for manual setup
	//server = EthernetServer(80);
	server.setup(NetworkStruct->webPort);
	//_startPin = NetworkStruct->startPin;
	_smtpServer = NetworkStruct->smtpServer;
	Syslog.setLoghost(NetworkStruct->logHost);
	_alarmSub = &(*NetworkStruct->alarmSub);
	_homeAuto = &(*NetworkStruct->homeAuto);
	_secret = (NetworkStruct->secret);
	_alarmSensorTotal = NetworkStruct->alarmSensorTotal;
	setupCommon();
}

/*
 * Setup Home Automation class after it has been created.
 * using all the arguments manually (not always maintained)
 */
void Network::setup(uint8_t startPin)
{
	_startPin = startPin;
	setupCommon();
}

/*
 * common argument that need to be initialized no mater what.
 * has been statically configured for a remote of 4 button.
 * look at code of sensor setup in AlarmSub to convert to dynamic
 */
void Network::setupCommon()
{
	pinMode(53, OUTPUT);					// set the SS pin as an output (necessary!)
	//digitalWrite(53, LOW);					// ? (not sure)
	pinMode(4, OUTPUT);						// SD select pin
	digitalWrite(4, HIGH);					// Explicitly disable SD
	pinMode(10, OUTPUT);						// Ethernet select pin
	digitalWrite(10, LOW);					// Explicitly enable Network
	server.begin();
	Serial.println(Ethernet.localIP());
	/*
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
	_switchStateDelay = 400;
	uint8_t i = 0;
	while (i > 4)
	{
		_switch[i] = false;
		_switchOn[i] = true;
		i++;		// start at 0, finish at 3, increment to 4, then stop
	}
	*/
	//#define BUFSIZ 100
	BUFSIZ = 100;
	//_secret = "secret"; // change this to your own, better, password.
	//char* lights[]={"red","yellow","green","beacon"}; // list of the "light" lables
	_auth= false;
	_emailAlertSent = false;
	_syslogSection = "Network";
	delay(5000);				// 5 sec delay to let sensors turn on. and network to initialise
	Syslog.logger(local7Slog, critSlog, _syslogSection, "System Startup");
	Serial.println("System Startup");
}



/*
 * Check if Remote button has been press. read from hardware.
 * check all 4 pins, one by one, and store info in array.
 */
void Network::check()
{
	//client.flush();
	//client = server.available();		// << in setup??
	EthernetClient client = server.available();		// << in setup??
	char clientline[BUFSIZ];
	int index = 0;
	uint8_t rip[] = {0, 0, 0, 0};
	String ripStr;
	if (client)
	{
		client.getRemoteIP(rip);
		ipToStr(rip, &ripStr);
		//uint32_t now = millis();
		uint32_t _timeout = 5000;
		Waiter _Timeout_w;
		_Timeout_w.resetreset();
		_Timeout_w.reset();
		// && !(_Timeout_w.wait(_timeout))
		//Serial.print("millis before web serve ");
		//Serial.println(millis());
		String _syslogTmp = "Connection From ";
		_syslogTmp += ripStr;
		Serial.println(_syslogTmp);
		Syslog.logger(local7Slog, noticeSlog, _syslogSection, _syslogTmp);
		while (client.connected() )
		{
			if ((_Timeout_w.wait(_timeout)))
			{
				//Serial.print("timeout web, break ");
				//Serial.println(millis());
				break;
			}
			if (client.available())
			{
				char c = client.read();

				// If it isn't a new line, add the character to the buffer
				if (c != '\n' && c != '\r')
				{
					clientline[index] = c;
					index++;
					// are we too big for the buffer? start tossing out data
					if (index >= BUFSIZ)
						index = BUFSIZ -1;

					// continue to read more data!
					continue;
				}

				// got a \n or \r new line, which means the string is done
				clientline[index] = 0;

				// Print it out for debugging
				Serial.println(clientline);

				// Look for substring such as a request to get the root file
				if (strstr(clientline, "GET / ") != 0)
				{
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println();
					//doform(client);
					client.print("Free Ram: ");
					client.println(freeRam());
				}
				else if (strstr(clientline, "GET /") != 0)
				{
					Serial.println("url have arg");
					// this time no space after the /, so a request!
					char *request;
					request = clientline + 5; // look after the "GET /" (5 chars)
					// a little trick, look for the " HTTP/1.1" string and
					// turn the first character of the substring into a 0 to clear it out.
					(strstr(clientline, " HTTP"))[0] = 0;
					// print the requested string
					Serial.println(request);
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					Serial.println("Sent 200 http");
					client.println();
					_auth= false;  // assume, until proven different, that the request is not authenticated
					/*
					for (int i = 0; i < numlights; i++)
					{ // loop through all the lights...
						states[i]=false;  // ...and assume they are off
					}
					*/
					if (strstr(request, (const char *)_secret) !=0)
					{
						client.println("<h2>Authenticated</h2>");
						_auth= true;
						//Serial.println("auth ok");
					}
					else
					{
						if (strstr(request, "favicon.ico") !=0)
						{
							Serial.println("favicon breaking");
							break;
						}
						client.println("<h2>Not Authenticated, so NOT...</h2>");
						//Serial.println("auth not ok");
					}

					if (_auth)
					{ // now that all the colors are set, change the lights
						/*  if "error" is sent as part of the request, ignore everything and start the error blink */
						if (strstr(request, "status") != 0)
						{
							if (strstr(request, "status=all") != 0)
							{ // show all status
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								//Serial.println("Calling Status All");
								statusAll(client);
								//Serial.println("FinishStatusAll");
							}
							if (strstr(request, "status=log") != 0)
							{ // show logs
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("Status log<br>");
							}
						}
						if (strstr(request, "alarm") != 0)
						{
							if (strstr(request, "alarm=on") != 0)
							{ // arm alarm
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("Alarm on<br>");
								_alarmSub->arming();
							}
							if (strstr(request, "alarm=off") != 0)
							{ // disarm alarm
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("Alarm off<br>");
								_alarmSub->unArming();
							}
						}
						if (strstr(request, "garage") != 0)
						{
							if (strstr(request, "garage=tog") != 0)
							{ // toogle garage door
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("Garage toogle<br>");
								_homeAuto->toggleGarage();
							}
							if (strstr(request, "garagelight=tim") != 0)
							{ // turn on garage light with timer
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("GarageLight timer<br>");
							}
							if (strstr(request, "garagelight=on") != 0)
							{ //turn on garage light
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("Garage Light On<br>");
							}
							if (strstr(request, "garagelight=off") != 0)
							{ // turn off garage light
								//blinkstates[1]=1; //yellow on <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
								client.println("Garage Light Off<br>");
							}
						}

					}

					// everything else is a 404
					//client.println("HTTP/1.1 404 Not Found");
					//client.println("Content-Type: text/html");
					//client.println();
					//client.println("<h2>HUH?</h2>");
					//Serial.println("after webserve, quit");
					//break;
					//getlights(); // get the current settings before...
					//doform(client); // ... drawing the form
				}
				else
				{
					// everything else is a 404
					client.println("HTTP/1.1 404 Not Found");
					client.println("Content-Type: text/html");
					client.println();
					client.println("<h2>HUH?</h2>");
				}
				break;
			}
		}

		delay(1); // give the web browser time to receive the data
		client.stop(); // close the connection:

	}
}

/*
 * check in array to see what key has been pressed, and do the corresponding action.
 */
uint32_t Network::freeRam()
{
	extern uint32_t __heap_start, *__brkval;
	uint32_t v;
	return (uint32_t) &v - (__brkval == 0 ? (uint32_t) &__heap_start : (uint32_t) __brkval);
}


void Network::statusAll(EthernetClient client)
{
	client.println("=======================================================");
	client.println("<br>");
	client.println("Status All");
	client.println("<br>");
	client.println("=======================================================");
	client.println("<br>");
	client.print("FreeRam: ");
	client.println(freeRam());
	Serial.println(freeRam());
	client.println("<br>");
	client.println(*_homeAuto->queryTempSensor());
	client.println("<br>");
	//String &temp = *_alarmSub->querySensorStatus(0);
	//String *mem = &temp;
	//Serial.println(int(mem));
	//&temp = &temp + 21;
	//Serial.println(temp);
	Serial.println(*_alarmSub->queryArmedStatus());
	client.println(*_alarmSub->queryArmedStatus());
	client.println("<br>");
	Serial.println(*_alarmSub->querySirenStatus());
	client.println(*_alarmSub->querySirenStatus());
	client.println("<br>");
	Serial.println(*_alarmSub->queryStrobeStatus());
	client.println(*_alarmSub->queryStrobeStatus());
	client.println("<br>");
	client.println("=======================================================");
	client.println("<br>");
	uint8_t i = 0;
	while ((_alarmSensorTotal - i ) > 0)
	{
		Serial.println(*_alarmSub->querySensorStatus(i));
		client.println(*_alarmSub->querySensorStatus(i));
		client.println("<br>");
		i++;
	}
	client.println("=======================================================");
	client.println("<br>");
	//client.println(temp);
	/*
	String temp2 = temp;
	String *mem2 = &temp;
	Serial.println(int(mem2));
	Serial.println(temp);
	client.println(temp);
	*/


}

void Network::emailCheck()
{
	if(_alarmSub->checkAlert())
	{
		if (!_emailAlertSent)
		{
			emailAlert();
			_emailAlertSent = true;
		}
	}
	else
	{
		_emailAlertSent = false;
	}
	if (smtpClient.available())
	{
		char c = smtpClient.read();
		Serial.print(c);
	}
	if (!smtpClient.connected())
	{
		//Serial.println();
		//Serial.println("disconnecting.");
		smtpClient.stop();
	}
}

void Network::emailAlert()
{
	//uint8_t smtpServer[] = { 203, 16, 214, 182};
	//uint8_t smtpServer[] = { 192, 168, 33, 3};		// local relay
	//uint8_t smtpServer[] = { 192, 168, 33, 50};		// disable
	//EthernetClient smtpClient;
	//delay(1000);
	if (smtpClient.connect(_smtpServer, 25))
	{
		Serial.println("connected");
		smtpClient.println("EHLO");
		smtpClient.println("MAIL FROM:<arduino@skaccess.com>");
		smtpClient.println("RCPT TO:<skullkill@skaccess.com>");

		smtpClient.println("DATA");
		smtpClient.println("From: ArduinoAlarm<arduino@skaccess.com>");
		smtpClient.println("To: Simon<skullkill@skaccess.com>");
		smtpClient.println("Subject: Home Alarm Alert");
		smtpClient.println("");
		smtpClient.println("=======================================================");
		smtpClient.println("Status......");
		smtpClient.println("=======================================================");
		smtpClient.print("FreeRam: ");
		smtpClient.println(freeRam());
		smtpClient.println(*_homeAuto->queryTempSensor());
		smtpClient.println(*_alarmSub->queryArmedStatus());
		smtpClient.println(*_alarmSub->querySirenStatus());
		smtpClient.println(*_alarmSub->queryStrobeStatus());
		smtpClient.println("=======================================================");
		uint8_t i = 0;
		while ((_alarmSensorTotal - i ) > 0)
		{
			smtpClient.println(*_alarmSub->querySensorStatus(i));
			i++;
		}
		smtpClient.println("=======================================================");
		smtpClient.println(".");
		//smtpClient.println("");
		smtpClient.println("QUIT");
		Syslog.logger(local7Slog, noticeSlog, _syslogSection, "Email Sent");
	}
	else
	{
	    Serial.println("connection failed");
	}
	//smtpClient.stop();
}

void Network::ipToStr(uint8_t *ipAddr, String *ipAddrStr)
{
	for (uint8_t n = 0; n<=3; n++)
	{
		*ipAddrStr += ipAddr[n];
		if (n !=3)
		{
			*ipAddrStr += ".";
		}
	}
}



