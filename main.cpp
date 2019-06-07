/*------------------------------------------------------
 *| main.cpp
 *|
 *|  Created on: Dec 02, 2011
 *|      Author: SkullKill
 *|
 *| * is to validate password
 *| # is to reset password attempt
 *------------------------------------------------------
 */
#include <Arduino.h>
#include <Password.h> //http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip
#include <AlarmSystem.h>


/*
 * TODO
 * read config from SDcard instead
 * time NTP
 * write log to SDcard
 * webserver to access Logs, to see alarm and sensor status, and to arm/unarm toogle garage.
 * send email in the even of alarm. with date and time.
 */

// IMPORTANT info about microSD card and Ethernet
// pin used for microSD = D4(select), D50, D51, D52
// pin used for Ethernet = D10(select), D50, D51, D52
// ping 53 must be left as output else microSD and Ethernet will not work.

// spare digital pins 2,3,5,6,7,8,(13),29
// spare analog pins 10,11,12,13,14,15

//Sensor List
// Use analog inputs 0 through 9 for sensor connections A through J (10 total)
//byte channelAInput = 0;
//byte channelBInput = 1;
//byte channelCInput = 2;
//byte channelDInput = 3;
//byte channelEInput = 4;
//byte channelFInput = 5;

// Use digital outputs 30 through 39 for status indicator LEDs A through J (10 total)
//byte channelALed = 30;
//byte channelBLed = 31;
//byte channelCLed = 32;
//byte channelDLed = 33;
//byte channelELed = 34;
//byte channelFLed = 35;

// sensor start ping will start at analog pin 0, and LED indicator will start at pin 30.
// cannot pass at an argument currently, but not hard to do
uint8_t alarmSensorTotal = 5;			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<rememter to change to 5
//uint8_t alarmSensorTotal = 1;			// DEV
String sensorNames[] = {"MasterBedroom" , "HomeTheatre  " , "Garage       " , "ServerRoom   " , "LivingDining "};

// Alarm Status LED and Buzzer
uint8_t armedPin = 11;
uint8_t powerPin = 12;
uint8_t buzzerPin = 9;
uint16_t buzzerFrequency = 261;		// 261, 523
uint16_t buzzerDuration = 100;

// Sirens and Output pins (40 - 43) (4 total)
uint8_t sirenPinOutside = 40;
uint8_t sirenDurationOutside = 2;		// in min , max 128 min ?
uint8_t sirenPinInside = 41;
uint8_t sirenDurationInside = 2;
uint8_t strobePin = 42;

// Garage (44 - 45)
uint8_t garagePin = 44;
uint8_t lightPin = 45;
uint8_t lightDuration = 5;

char pass[5] = "1234";					// need to test if works
Password password = Password( pass );
//Password password = Password( "1234" );

// Remote (46 - 49) (D3 - D0)
uint8_t startRemotePin = 46;



uint8_t ip[] = { 192, 168, XXX, XXX };   //Manual setup only
//uint8_t ip[] = { 192, 168, XXX, XXX };   //Manual setup only DEV

uint8_t dnsServer[] = { 192, 168, XXX, XXX };	//Manual setup only
uint8_t gateway[] = { 192, 168, XXX, XXX }; //Manual setup only
uint8_t subnet[] = { 255, 255, 255, 0 }; //Manual setup only

// if need to change the MAC address (Very Rare)
//uint8_t mac[] = { 0x00, 0x1D, 0xB3, 0x00, 0x21, 0xF7 };		// ProCurve Networking by HP
//uint8_t mac[] = { 0x00, 0x1D, 0xB3, 0x00, 0x21, 0xF6 };		// ProCurve Networking by HP DEV

uint8_t webPort= 80; //port 80
char secret[] = "SomeSecretPassowrd11";

uint8_t smtpServer[] = { 192, 168, XXX, XXX};		// smtp relay
//uint8_t smtpServer[] = { 192, 168, XXX, XXX};		// smtp disable (bogus ip)

uint8_t logHost[] = { 192, 168, XXX, XXX };		// Syslog server ip
//uint8_t logHost[] = { 192, 168, XXX, XXX };		// Syslog disable



//AlarmSubStruct_t ASStemp;
//HomeAutoStruct_t HAStemp;
//NetworkStruct_t networkStruct = {&mac, &ip, &gateway, &subnet, webPort, &secret, &ASStemp, &HAStemp};
NetworkStruct_t networkStruct = {mac, ip, dnsServer, gateway, subnet, webPort, secret, smtpServer, logHost, alarmSensorTotal};

//NetworkStruct2_t networkStruct = {ip, gateway};
//NetworkStruct2_t networkStruct = {{6 , 7}};

AlarmSubSensorStruct_t alarmSubSensorStruct;
AlarmSubStruct_t alarmSubStruct = {buzzerPin, buzzerDuration, alarmSensorTotal, sensorNames, &alarmSubSensorStruct, armedPin, sirenPinOutside, sirenDurationOutside, sirenPinInside, sirenDurationInside, strobePin};
// something is overwriting the memory space of homeAutoStruct, so put another one as buffer. need to fix properly later.
HomeAutoStruct_t homeAutoStruct2 = {0, garagePin, lightPin, lightDuration};
HomeAutoStruct_t homeAutoStruct = {0, garagePin, lightPin, lightDuration};




// Keypad (22 - 28)
const byte ROWS = 4; // Four rows
const byte COLS = 3; //  columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 25, 24, 23, 22 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 28, 27, 26 };

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//create AlarmSystem structure
//AlarmStruct_t AlarmStruct = {powerPin, buzzerPin, buzzerDuration, alarmSensorTotal, sensorNames, armedPin, sirenPinOutside, sirenDurationOutside, sirenPinInside, sirenDurationInside, strobePin, startRemotePin, garagePin, &networkStruct};
AlarmStruct_t AlarmStruct = {powerPin, &alarmSubStruct, startRemotePin, &homeAutoStruct, &networkStruct};
//create AlarmSystem
//AlarmSystem HomeAlarm = AlarmSystem(&AlarmStruct);
AlarmSystem HomeAlarm;

// need to move to AlarmSystem library
void checkPassword(){
  if (password.evaluate()){
    Serial.println("Success");
    //Add code to run if it works
    HomeAlarm.armingToggle();
    /*
    noTone(buzzerPin);
    tone(buzzerPin, buzzerFrequency, buzzerDuration);
    delay(buzzerDuration);
    tone(buzzerPin, 523, buzzerDuration);
    delay(buzzerDuration);
    tone(buzzerPin, buzzerFrequency, buzzerDuration);
    */
    password.reset();	// clear password memory
  }else{
    Serial.println("Wrong");
    //add code to run if it did not work
    noTone(buzzerPin);
    tone(buzzerPin, 800, buzzerDuration*5);
    password.reset();	// clear password memory
  }
}

//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
  	  Serial.print("Pressed: ");
	  Serial.println(eKey);
	  tone(buzzerPin, buzzerFrequency, buzzerDuration);
	  switch (eKey){
	    case '#': checkPassword(); break;
	    case '*': password.reset(); break;
	    default: password.append(eKey); break;
      }
	  break;
	case IDLE:
	  break;
	case RELEASED:
	  break;
	case HOLD:
	  break;
  }
}

// all setup done when creating AlarmSystem class
void setup()
{
	HomeAlarm.setup(&AlarmStruct);
	//Ethernet.begin(mac, ip, gateway, subnet);
	Serial.begin(9600);

	keypad.addEventListener(keypadEvent); //add an event listener for this keypad
	keypad.setHoldTime(1000);				// can be remove (redundant)
	keypad.setDebounceTime(50);			// can be remove (redundant)
	//delay(5000);							// 5 sec delay to let sensors turn on.
}

// main loop
void loop()
{
	keypad.getKey();
	HomeAlarm.checkRemote();				// check if any remote key has been press, and act accordingly
	HomeAlarm.alarmCheckAll();			// check all alarm sensors, if armed, ring siren etc
	HomeAlarm.checkHomeAuto();				// check if need to open/close garage
	HomeAlarm.networkCheckAll();
	//delay(1000);							// enable only to troubleshoot sensor
	//Serial.println(millis());
	// if (DEBUG){Serial.prinln("debug mode");}
}

