/*
  Syslog.h - An Arduino library for a sending Syslog messages.
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


// example Syslog.logger(23,critSlog, _syslogSection ,"System Startup");
#ifndef Syslog_h
#define Syslog_h

#include <inttypes.h>

#define SYSLOG_DEFAULT_PORT 514
#define emergSlog 0
#define alertSlog 1
#define critSlog 2
#define errSlog 3
#define warnSlog 4
#define noticeSlog 5
#define infoSlog 6
#define debugSlog 7

#define local7Slog 23

class SyslogClass {
public:
    void setLoghost(uint8_t *);
    //void setOwnHostname(int n);
    void logger(uint8_t, uint8_t, const char[], const char[]);
    void logger(uint8_t, uint8_t, const char[], String&);
    void logger(uint8_t, uint8_t, String&, const char[]);
    void logger(uint8_t, uint8_t, String&, String&);
private:
    uint8_t * ip_syslogserver;
    //char my_own_hostname[256];
};

extern SyslogClass Syslog;

#endif
