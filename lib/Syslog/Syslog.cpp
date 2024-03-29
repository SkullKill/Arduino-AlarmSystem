/*
  Syslog.cpp - An Arduino library for a sending Syslog messages.
  Copyright (C) 2011 Markus Heller <heller@relix.de>
  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <Arduino.h>
#include "Syslog.h"
#include <Ethernet/Ethernet.h>
#include <Ethernet/EthernetUdp.h>

EthernetUDP SyslogUdp;
 
void SyslogClass::setLoghost(uint8_t * server_ip) {
    ip_syslogserver = server_ip;
    SyslogUdp.begin(8888);
}
 
 
/*
  The HOSTNAME field SHOULD contain the hostname and the domain name of
  the originator in the format specified in STD 13 [RFC1034].  This
  format is called a Fully Qualified Domain Name (FQDN) in this
  document.

  In practice, not all syslog applications are able to provide an FQDN.
  As such, other values MAY also be present in HOSTNAME.  This document
  makes provisions for using other values in such situations.  A syslog
  application SHOULD provide the most specific available value first.
  The order of preference for the contents of the HOSTNAME field is as
  follows:

  1.  FQDN
  2.  Static IP address
  3.  hostname
  4.  Dynamic IP address
  5.  the NILVALUE

  If an IPv4 address is used, it MUST be in the format of the dotted
  decimal notation as used in STD 13 [RFC1035].  If an IPv6 address is
  used, a valid textual representation as described in [RFC4291],
  Section 2.2, MUST be used.

  Syslog applications SHOULD consistently use the same value in the
  HOSTNAME field for as long as possible.

  The NILVALUE SHOULD only be used when the syslog application has no
  way to obtain its real hostname.  This situation is considered highly
  unlikely.


#define emerg 0
#define alert 1
#define crit    2
#define err 3
#define warn 4
#define notice 5
#define info 6
#define debug 7

*/
//void SyslogClass::setOwnHostname(int n) {
  //my_own_hostname = my_hostname;
//}
 
 
 
void SyslogClass::logger(uint8_t facility, uint8_t severity, const char tag[], const char message[]) {
	String Pri;

	Pri="<";
	Pri+=(8 * facility + severity);
	Pri+=">";
  
	char UDPBufferPri[Pri.length()+1];
	Pri.toCharArray(UDPBufferPri,Pri.length()+1);

	SyslogUdp.beginPacket(ip_syslogserver, SYSLOG_DEFAULT_PORT);

	SyslogUdp.write(UDPBufferPri);
	SyslogUdp.write(tag);
	SyslogUdp.write(" ");
	SyslogUdp.write(message);
	SyslogUdp.endPacket();
}

void SyslogClass::logger(uint8_t facility, uint8_t severity, const char tag[], String& message) {
  char __message[message.length()+1];
  message.toCharArray(__message,message.length()+1);  

  logger(facility, severity, tag, __message);
}

void SyslogClass::logger(uint8_t facility, uint8_t severity, String& tag, const char message[]) {

  char __tag[tag.length()+1];
  tag.toCharArray(__tag,tag.length()+1);

  logger(facility, severity, __tag, message);
}

void SyslogClass::logger(uint8_t facility, uint8_t severity, String& tag, String& message) {
  char __message[message.length()+1];
  message.toCharArray(__message,message.length()+1);

  char __tag[tag.length()+1];
  tag.toCharArray(__tag,tag.length()+1);

  logger(facility, severity, __tag, __message);
}


/* Create one global object */
SyslogClass Syslog;
