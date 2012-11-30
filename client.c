/** client.c --- 
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Egorov N.V.
 *
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: client.c,v ad3adcfc8418  2012/10/16 10:11:00  egorov@rti-mints.ru $
 *

 * Данная программа разработана в ОАО "РТИ имени Минтца".
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "debuging.h"
#include "datapacker.h"
#include "delays.h"
#include "config.h"

#include <tio.h>


static int
wait(int ttyFd)
{
    uint32_t readBuffer   = 0;
    
    struct timeval selectDelay;
    fd_set   set, *fdSet = &set;
    int      retval;

    time_t start = time(NULL);

    while(config.work)
    {
        time_t current = time(NULL);
        selectDelay.tv_sec  = 0;
        selectDelay.tv_usec = BASEDELAY;
        FD_ZERO(fdSet);
        FD_SET(ttyFd, fdSet);

        
        if ((current - start) > 20)
            break;
        DEBUGOUT("Current %ld: stat at %ld: elapsed: %ld", current, start, (current - start))

        retval = select(ttyFd+1, fdSet, NULL, NULL, &selectDelay);
        if (retval == -1)
        {
            perror("Client wait polling error: ");
            kill(0, SIGTERM);
            return EIO;
        }
        /*usleep(usec_floor(32 * config.portSpeed * 1e-6));*/
        usleep(usec_floor(32 * tioGetDefL( "PORTSPEED", 115200 ) * 1e-6));
        retval = read(ttyFd, &readBuffer, sizeof(readBuffer));
        if (retval == -1)
        {
            if(errno == EAGAIN)
            {
                DEBUGMSG("Port is empty - nothing read");
                continue;
            }
            perror("Client waiting read error: ");
            kill(0,SIGTERM);
            return EIO;
        }
        if(retval == sizeof(readBuffer))
        {
            DEBUGPARAMETRS("Found recive buffer: %X", readBuffer);
            if(htonl(readBuffer) == WAITMAGIC)
            {
                return 0;
            }
        }

    }
    DEBUGMSG("Waiting of server failed");
    return -1;
}

static int
read_and_compare(int ttyFd, DataPack *standartMessage)
{
    DataPack buffer, decodedBuffer;
    char*    buffPtr=(char*)&buffer;
    int      buffFill=0;

    int pass = 1;
    long iteration = 0;

    struct timeval selectDelay;
    fd_set   set, *fdSet = &set;
    int      retval;
    long lastLength = 1;

    *((uint32_t*)buffPtr) = htonl(RECIVEREADYMAGIC);

    while(config.work)
    {
        retval = write(ttyFd, &buffer, sizeof(uint32_t));
        if (-1 == retval)
        {
            perror("Client error write ready status: ");
            kill(0, SIGTERM);
            return EIO;
        }
        usleep(calculate_delay_in_usec(2 * WAITBASEDELAY, WAITBASEDELAY));

        selectDelay.tv_sec  = 0;
        selectDelay.tv_usec = BASEDELAY;
        FD_ZERO(fdSet);
        FD_SET(ttyFd, fdSet);
        select(ttyFd + 1, fdSet, NULL, NULL, &selectDelay);
        
        retval = read(ttyFd, &buffer, sizeof(buffer));
        if (retval == -1)
        {
            if (errno == EAGAIN)
                continue;
            perror("Client: read data error");
            kill(0, SIGTERM);
            return EIO;
        }
        
        buffer.magic = ntohl(buffer.magic);
        if (buffer.magic == RECIVEREADYMAGIC)
        {
            continue;
        }
        else if (buffer.magic == SENDMAGIC)
        {
            buffPtr = (char*)&buffer;
            buffFill = retval;
            break;
        }
        else if (buffer.magic == WAITMAGIC)
        {
            DEBUGOUT("Geting wait message - again");
            DEBUGPARAMETRS("Unexpected value in buffer: %X", (uint32_t)buffer.magic);
            kill(0, SIGTERM);
        }
        else 
        {
            DEBUGOUT("Unexpected value in buffer: %X", (uint32_t)buffer.magic);
            kill(0, SIGTERM);
            return EINVAL;
        }
    }
    DEBUGMSG("Ready to read status wrote");

    while(/*config.work && */ lastLength > 0)
    {
        if (buffFill == sizeof(buffer))
        {
            convert_network_string_to_datapack((char*)&buffer, &decodedBuffer);
            pass = pass && !compare_data_packages(standartMessage, &decodedBuffer);
            buffFill = 0;
            buffPtr  = (char*)&buffer;
            DEBUGOUT2("Message decoded: left space %d", decodedBuffer.dataTotalLength);
            lastLength = (int)decodedBuffer.dataTotalLength;
            memset(&buffer, 0, sizeof(buffer));
            ++iteration;
            continue;
        }

        selectDelay.tv_sec  = 0;
        /*selectDelay.tv_usec = 20 * calculate_delay_from_speed_usec(config.portSpeed);*/
        selectDelay.tv_usec = 20 * calculate_delay_from_speed_usec( tioGetDefL( "PORTSPEED", 115200 ) );
        FD_ZERO(fdSet);
        FD_SET(ttyFd, fdSet);

        retval = select(ttyFd + 1, fdSet, NULL, NULL,  &selectDelay);
        if (retval == -1)
        {
            if ( errno == EINTR )
                continue;
            perror("Client compare failed: ");
            kill(0, SIGTERM);
            return EIO;
        }
        else if (retval == 0)
        {
            if (lastLength < 0)
                break;
            DEBUGOUT("Too long nothing happening: Waiting more bytes %ld\n", lastLength);
            kill(0, SIGTERM);
            break;
        }
        retval = read(ttyFd, buffPtr, sizeof(buffer) - buffFill);
        if (retval == -1)
        {
            if(errno == EAGAIN)
            {
                continue;
            }
            perror("Client read port data failed: ");
            kill(0,SIGTERM);
            return EIO;
        }
        buffFill += retval;
        buffPtr  += retval;

    }
    DEBUGMSG("Client decode finished");
    return (pass && (iteration > 0)) ? 0 : 1;
}


int 
client_process(Configuration *config)
{
    DataPack standartMessage;
    
    if (convert_message_to_datapack(messageString,sizeof(messageString),&standartMessage))
    {
        fputs("Client: Failed to create standart message\n", stderr);
        config->work = 0;
        kill(0, SIGTERM);
        return -1;
    }
    DEBUGMSG("Starting server wait");
    if (wait(config->outputDevice))
    {
        config->work = 0;
        puts("Test FAILED");
        kill(0, SIGTERM);
        return -1;
    }
    DEBUGMSG("Starting transfere");
    if(read_and_compare(config->outputDevice, &standartMessage))
    {
        puts("Test COM -  FAILED");
        config->work = 0;
        kill(0, SIGTERM);
        return -1;
    }
    config->work  = 0;
    puts("client process - OK");
    return 0;
}



    
