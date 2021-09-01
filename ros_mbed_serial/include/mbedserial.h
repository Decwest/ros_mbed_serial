// mbedserial.h
// serial communication : mbed <-> ROS

#ifndef _MBEDSERIAL_
#define _MBEDSERIAL_

#include <mbed.h>
#include <stdio.h>
#include <string.h>

DigitalOut myled(LED1);

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


/***********************init***********************/
void _nullfunc() { ; };

Mbedserial::Mbedserial(BufferedSerial &pc) : rospc(pc)
{
    msg_buf = new char[256];
    bufsize = 0;
    startmsg = '\b';
    endmsg = '\n';
    floatarraysize = 0;
    intarraysize = 0;
    stringarraysize = 0;
    recv_data_size = 0;
    for (int i = 0; i < 3; i++)
    {
        pfunccb[i] = _nullfunc;
    }
}

/**********************receive**********************/

void Mbedserial::read_data()
{
    int recv_data = rospc.read(&buff[recv_data_size], sizeof(buff));
    int arraysize = 0;

    if (recv_data > 0)
    {
        recv_data_size += recv_data;
        if (recv_data_size >= 256)
        {
            recv_data_size = 0;
        }
        else if (buff[recv_data_size - 1] == endmsg)
        {
            arraysize = *(int *)(&buff[2]);
            //memcpy(&arraysize, &(buff[1]), 4);

            switch (buff[1])
            {
            case 'f':
                if (recv_data_size >= arraysize * 4 + 7)
                {
                    floatarraysize = arraysize;
                    for (int i = 0; i < arraysize; i++)
                    {
                        getfloat[i] = *(float *)(&buff[i * 4 + 6]);
                        //memcpy(&pub_float.data[i], &buf_pub[i * 4 + 5], 4);
                    }
                    pfunccb[0]();
                }
                break;
            case 'i':
                if (recv_data_size >= arraysize * 4 + 7)
                {
                    intarraysize = arraysize;
                    for (int i = 0; i < arraysize; i++)
                    {
                        getint[i] = *(int *)(&buff[i * 4 + 6]);
                        //memcpy(&pub_int.data[i], &buf_pub[i * 4 + 5], 4);
                    }
                    pfunccb[1]();
                }
                break;
            case 'c':
                if (recv_data_size >= arraysize + 7)
                {
                    stringarraysize = arraysize;
                    getstring = &buff[6];
                    getstring = getstring.substr(0, arraysize);
                }
                pfunccb[2]();
            }
            recv_data_size = 0;
        }
    }
}

/***********************send***********************/

void Mbedserial::float_write(const float *array, int arraysize)
{
    delete[] floattochar;
    floattochar = new char[arraysize * 4 + 7];
    floattochar[0] = startmsg;
    floattochar[1] = 'f';
    *(int *)(&floattochar[2]) = arraysize;
    //memcpy(&floattochar[1], &datasize, 4);
    for (int i = 0; i < arraysize; i++)
    {
        *(float *)(&floattochar[i * 4 + 6]) = array[i];
        //memcpy(&floattochar[i * 4 + 5], &serial_msg.data[i], 4);
    }
    floattochar[arraysize * 4 + 6] = endmsg;
    myled = !myled;
    rospc.write(floattochar, arraysize * 4 + 7);
}

void Mbedserial::int_write(const int *array, int arraysize)
{
    delete[] inttochar;
    inttochar = new char[arraysize * 4 + 7];
    inttochar[0] = startmsg;
    inttochar[1] = 'i';
    *(int *)(&inttochar[2]) = arraysize;
    //memcpy(&floattochar[1], &datasize, 4);
    for (int i = 0; i < arraysize; i++)
    {
        *(int *)(&inttochar[i * 4 + 6]) = array[i];
        //memcpy(&floattochar[i * 4 + 5], &serial_msg.data[i], 4);
    }
    inttochar[arraysize * 4 + 6] = endmsg;
    rospc.write(inttochar, arraysize * 4 + 7);
}

void Mbedserial::string_write(const std::string &str)
{
    delete[] stringtochar;
    int arraysize = str.size();
    stringtochar = new char[arraysize + 6];
    stringtochar[0] = startmsg;
    stringtochar[1] = 'c';
    *(int *)(&stringtochar[2]) = arraysize;
    //memcpy(&chartochar[1], &datasize, 4);
    const char *array = str.c_str();
    memcpy(&stringtochar[6], array, arraysize);
    stringtochar[arraysize + 6] = endmsg;
    rospc.write(stringtochar, arraysize + 7);
}

#endif