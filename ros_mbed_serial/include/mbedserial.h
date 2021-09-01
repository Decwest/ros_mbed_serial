// mbedserial.h
// serial communication : mbed <-> ROS

#ifndef _MBEDSERIAL_
#define _MBEDSERIAL_

#include <mbed.h>
#include <stdio.h>
#include <string.h>

// mbedserial class
class Mbedserial
{
private:
	char *msg_buf;
	int bufsize;
	char startmsg;
	char endmsg;
	char *floattochar;
	char *inttochar;
	char *stringtochar;
	int recv_data_size;

	char buff[256] = {0};

	BufferedSerial &rospc;
	void (*pfunccb[3])();
	void read();

public:
	float getfloat[32];
	int getint[32];
	std::string getstring;
	int floatarraysize;
	int intarraysize;
	int stringarraysize;

	Mbedserial(BufferedSerial &);
	void read_data();
	void float_write(const float *array, int arraysize);
	void int_write(const int *array, int arraysize);
	void string_write(const std::string &str);
	void float_attach(void (*pfunc)()) { pfunccb[0] = pfunc; };
	void int_attach(void (*pfunc)()) { pfunccb[1] = pfunc; };
	void string_attach(void (*pfunc)()) { pfunccb[2] = pfunc; };
};

#endif