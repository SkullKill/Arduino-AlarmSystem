/*
 * Network.h
 *
 *  Created on: 25/10/2012
 *      Author: SkullKill
 */

#ifndef NETWORK_H
#define NETWORK_H


//#include <Arduino.h>
//#include <Waiter.h>
#include <Arduino.h>
//#include <MemoryFree.h>
#include <Waiter.h>

#include <Ethernet/Ethernet.h>
#include <SPI/SPI.h>
#include <Ethernet/EthernetUDP.h>
#include <Syslog/Syslog.h>

#include <AlarmSub.h>
#include <HomeAuto.h>

struct NetworkStruct_t
{
	uint8_t *mac;
	uint8_t *ip;
	uint8_t *dnsServer;
	uint8_t *gateway;
	uint8_t *subnet;
	uint8_t webPort;
	char *secret;
	uint8_t *smtpServer;
	uint8_t *logHost;
	uint8_t alarmSensorTotal;
	AlarmSub *alarmSub;
	//AlarmSub *alarmSub;
	HomeAuto *homeAuto;
};
typedef NetworkStruct_t NeS_t;

struct NetworkStruct2_t
{
	uint8_t *ip;
	uint8_t *gateway;
};
/*
 * Class for Remote Control
 * imports the AlarmSub class and HomeAuto class and interact with them directly
 * does not actually "import" but, use the address of the class to modify the same class created earlier.
 * does not copy the class, uses the same class previously created.
 */
class Network
{
public:
	Network();
	Network(NetworkStruct_t *NetworkStruct);
	Network(uint8_t startPin);
	void setup(NetworkStruct_t *NetworkStruct);
	void setup(uint8_t startPin);
	void check();
	uint32_t freeRam();
	void emailAlert();
	void emailCheck();
	void ipToStr(uint8_t *ipAddr, String *ipAddrStr);
	//friend class AlarmSub;
private:
	String _syslogSection;
	AlarmSub* _alarmSub;
	HomeAuto * _homeAuto;
	//RemoteStruct_t * RemoteStructLocal;
	char* _secret;
	uint8_t *_smtpServer;
	uint8_t _alarmSensorTotal;
	bool _auth;
	int16_t BUFSIZ;
	EthernetServer server;
	//EthernetClient client;
	//Client smtpClient;
	uint8_t _startPin;
	void setupCommon();
	void statusAll(EthernetClient client);
	bool _emailAlertSent;
	EthernetClient smtpClient;
};



#endif /* NETWORK_H */
