/** server.c --- server part of rs-232  (rs574) test
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Egorov N.V.
 * 
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: server.c,v 799dfa493d0a  2012/05/28 14:20:07  egorov@rti-mints.ru $
 *

 * Данная программа разработана в ОАО "РТИ имени Минтца".
 * 
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <arpa/inet.h>
#include <tio.h>

#include "datapacker.h"
#include "debuging.h"
#include "delays.h"
#include "config.h"

/** 
 * @brief Функция ожидания запроса на передачу данных.
 * 
 * Функция ожидающая получения входного сообщения с кодом для начала
 * передачи данных. Данная функция в цыкле размещает в COM порт
 * переданный в качестве параметра сообщение о готовности передачи
 * сообщения и ожидает подтверждения о готовности приема.
 *
 * @param ttyFd Фаловый дескриптор настроенного фала последовательного
 * порта.
 * 
 * @return 0 в случае успеного завершения и код ошибки в противном
 * случае.
 */
static int
wait_ready_state(int ttyFd)
{
    uint32_t waitMessage = htonl(WAITMAGIC);
    uint32_t reciver     = 0;
    struct   timeval selectDelay;                
    fd_set   ttyset;
    int      retval;

    FD_ZERO(&ttyset);
    FD_SET(ttyFd, &ttyset);
    
    while(config.work)
    {
        if (-1 == write(ttyFd,&waitMessage, sizeof(waitMessage)))
        {
            int err = errno;
            perror("Server: waitin failed on write to serial");
            return err;
        }
        usleep(calculate_delay_in_usec(WAITBASEDELAY, WAITBASEDELAY));
        selectDelay.tv_sec  = 0;
        selectDelay.tv_usec = BASEDELAY;
        retval = select(ttyFd+1, &ttyset, NULL, NULL, &selectDelay);
        if (-1 == retval)
        {
            perror("Server: polling file descriptor failed");
        }
        if (retval == 0)
        {
            DEBUGMSG("Waiting for answer failed new wait circle");
            continue;
        }
        if (-1 == read(ttyFd, &reciver, sizeof(uint32_t)))
        {
            int err = errno;
            if (errno == EAGAIN)
            {
                DEBUGMSG("TTY port is empty");
                continue;
            }
            perror("Server: read port failed");
            return err;
        }
        reciver = ntohl(reciver);
        DEBUGOUT2("Getting value %X", reciver);
        switch(reciver) 
        {
        case WAITMAGIC:
            continue;
        case RECIVEREADYMAGIC:
            return 0;
        default:
            DEBUGOUT("Starnge man state %X recived",reciver);
            break;
        }
    }
    DEBUGMSG("Process aborted before somthing happened");
    return -1;
}

/** 
 * @brief Осуществляет передачу данных через последовательный порт.
 *
 * Функция осуществляет передачу данных в количестве dataCount
 * стандартных пакетов.
 * 
 * @param ttyFd Дескриптор открытого файла порта
 * @param dataCount Количество передаваемых пакетов данных
 * 
 * @return 0 при удачном завершении и код ошибки в противном случае.
 */                
static int
send_data_to_client(int ttyFd, long dataCount)
{    
    DataPack sendData, encodedData;
    long dataSize = dataCount;

    if ((unsigned long)dataCount > (unsigned long)UINT32_MAX)
    {
        DEBUGOUT2("Requested %ld data but can\'t accept more then %lu", dataCount, 
                  (unsigned long)UINT32_MAX);
        errno = EINVAL;
        return EINVAL;
    }

    convert_message_to_datapack(messageString,sizeof(messageString), &sendData);

    dataSize = dataCount;

    do
    {
        int res;
        dataSize -= sizeof(DataPack);
        DEBUGPARAMETRS2("%ld", dataSize);
        sendData.dataTotalLength = dataSize;
        convert_datapack_to_network_string(&sendData, &encodedData);
        res = write(ttyFd, &encodedData, sizeof(DataPack));
        if (-1 == res)
        {
            perror("Server main circle - cant write data to port");
            return EIO;
        }
        usleep(10 * calculate_delay_from_speed_usec(tioGetDefL( "PORTSPEED", 115200 )));
    }
    while (dataSize > 0 && config.work);
    return 0;
}
    

int
server_process(const Configuration* config)
{
    int rc;

    DEBUGMSG("Starting server process");

    if( tioGetL( "CLIENTSERVERMODE" ) )
        daemon(0,0);

    if (0 != (rc = wait_ready_state(config->outputDevice)))
    {
        errno = rc;
        perror("Server fatal error: stoped");
        exit(-1);
    }
    DEBUGMSG("Conformation recived. Starting data transfare");
    if (0 != (rc = send_data_to_client(config->outputDevice, config->sendPacksLength)))
    {
        perror("Server sending fatal error: aborted");
        exit(-1);
    }
    DEBUGMSG("Server data transfere finished.");
    return 0;
}


        
