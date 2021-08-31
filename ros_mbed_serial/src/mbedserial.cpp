// mbedserial.cpp
// serial communication : mbed <-> ROS

#include "mbedserial.h"

DigitalOut myled(LED1);

/***********************init***********************/
void _nullfunc() { ; };

Mbedserial::Mbedserial(BufferedSerial &pc) : rospc(pc)
{
    msg_buf = new char[256];
    bufsize = 0;
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
            arraysize = *(int *)(&buff[1]);
            //memcpy(&arraysize, &(buff[1]), 4);

            switch (buff[0])
            {
            case 'f':
                if (recv_data_size >= arraysize * 4 + 6)
                {
                    floatarraysize = arraysize;
                    for (int i = 0; i < arraysize; i++)
                    {
                        getfloat[i] = *(float *)(&buff[i * 4 + 5]);
                        //memcpy(&pub_float.data[i], &buf_pub[i * 4 + 5], 4);
                    }
                    pfunccb[0]();
                }
                break;
            case 'i':
                if (recv_data_size >= arraysize * 4 + 6)
                {
                    intarraysize = arraysize;
                    for (int i = 0; i < arraysize; i++)
                    {
                        getint[i] = *(int *)(&buff[i * 4 + 5]);
                        //memcpy(&pub_int.data[i], &buf_pub[i * 4 + 5], 4);
                    }
                    pfunccb[1]();
                }
                break;
            case 'c':
                if (recv_data_size >= arraysize + 6)
                {
                    stringarraysize = arraysize;
                    getstring = &buff[5];
                    getstring = getstring.substr(0, arraysize);
                }
                pfunccb[2]();
            }
            recv_data_size = 0;
        }
    }
}

/***********************send***********************/

int pub_wait_time = 8; //ms

void Mbedserial::float_write(const float *array, int arraysize)
{
    delete[] floattochar;
    floattochar = new char[arraysize * 4 + 6];
    floattochar[0] = 'f';
    *(int *)(&floattochar[1]) = arraysize;
    //memcpy(&floattochar[1], &datasize, 4);
    for (int i = 0; i < arraysize; i++)
    {
        *(float *)(&floattochar[i * 4 + 5]) = array[i];
        //memcpy(&floattochar[i * 4 + 5], &serial_msg.data[i], 4);
    }
    floattochar[arraysize * 4 + 5] = endmsg;
    floattochar[10] = 'f';
    rospc.write(floattochar, arraysize * 4 + 6);
}

void Mbedserial::int_write(const int *array, int arraysize)
{
    delete[] inttochar;
    inttochar = new char[arraysize * 4 + 6];
    inttochar[0] = 'i';
    *(int *)(&inttochar[1]) = arraysize;
    //memcpy(&floattochar[1], &datasize, 4);
    for (int i = 0; i < arraysize; i++)
    {
        *(int *)(&inttochar[i * 4 + 5]) = array[i];
        //memcpy(&floattochar[i * 4 + 5], &serial_msg.data[i], 4);
    }
    inttochar[arraysize * 4 + 5] = endmsg;
    rospc.write(inttochar, arraysize * 4 + 6);
}

void Mbedserial::string_write(std::string str, int arraysize)
{
    delete[] stringtochar;
    stringtochar = new char[arraysize + 6];
    stringtochar[0] = 'c';
    *(int *)(&stringtochar[1]) = arraysize;
    //memcpy(&chartochar[1], &datasize, 4);
    const char *array = str.c_str();
    memcpy(&stringtochar[5], array, arraysize);
    stringtochar[arraysize + 5] = endmsg;
    rospc.write(stringtochar, arraysize + 6);
}