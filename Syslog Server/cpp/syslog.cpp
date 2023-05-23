/*
 * IPWorks 2022 C++ Edition - Demo Application
 *
 * Copyright (c) 2023 /n software inc. - All rights reserved. - www.nsoftware.com
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/ipworks.h"
#define LINE_LEN 100

class MySyslog : public SysLog
{
public:

	virtual int FirePacketIn(SysLogPacketInEventParams *e)
	{
		printf("Host: %s\nFacility: %s\nSeverify: %s\nTime: %s\nMessage: %s\n\n", e->Hostname, e->Facility, e->Severity, e->Timestamp, e->Message);
		return 0;
	}

};

int main(int argc, char **argv)
{

	MySyslog syslog;
	SysLog syslogsender;
	int ret_code = 0;

	printf("Activating Syslog Listener.\n");
	printf("Sending test messages.\n");
	
	syslog.SetActive(true);

	syslogsender.SetLocalPort(0);//system chosen free port
	syslogsender.SetRemoteHost("localhost");
	syslogsender.SetRemotePort(syslog.GetLocalPort());

	ret_code = syslogsender.SetActive(true);
	if (ret_code) goto done;

	ret_code = syslogsender.SendPacket(1, 2, "Test 1");
	if (ret_code) goto done;
	syslogsender.SendPacket(1, 2, "Test 2");
	syslogsender.SendPacket(1, 2, "Test 3");
	syslogsender.SetActive(false);

	printf("Test messages sent. To stop listener, press Ctrl - C.\n\n");

	while(true)
	{
		syslog.DoEvents();
	}

done:
	if (ret_code)
		printf("Error: %s\n",syslogsender.GetLastError());

	syslog.SetActive(false);
	syslogsender.SetActive(false);

	fprintf(stderr, "\npress <return> to continue...\n");
	getchar();
	return 0;
}





