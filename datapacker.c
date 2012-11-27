/** datapacker.c --- 
*
* Copyright (C) 2011 RTI im. Mintca
* Copyright (C) 2011 Egorov N.V.
*
* Author: Egorov N.V. <egorov@rti-mints.ru>
* $Id: datapacker.c,v 01d5796cdd2d  2012/05/28 13:17:08  egorov@rti-mints.ru $
*

* Данная программа разработана в ОАО "РТИ имени Минтца".
* 
*/

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "datapacker.h"

/**
 *  Тестовая строка представляющая собой sha256  строки 
 * "Test of RS-574 of RTI" 
 */

char messageString[] =  {0xb8, 0xf6, 0xa8, 0x47, 
                         0xfe, 0xc1, 0x58, 0x2a, 
                         0xd9, 0xf8, 0xa1, 0xef, 
                         0x9f, 0x39, 0xe3, 0xe2, 
                         0x15, 0x3e, 0x4b, 0xd4, 
                         0x94, 0xc5, 0x83, 0xd5, 
                         0xd0, 0x95, 0xd8, 0x43, 
                         0x06, 0x73, 0xdd, 0xb8};




int 
convert_message_to_datapack(char *message, size_t messageLength, DataPack *pack)
{
    if (messageLength > MESSAGELENGTH)
        return EINVAL;
    memcpy(pack->message, message, messageLength);
    pack->magic  =  SENDMAGIC;
    pack->messageLength = messageLength;
    pack->dataTotalLength = 0;
    return 0;

}



int 
convert_datapack_to_network_string(DataPack *pack, void* buff)
{
    DataPack internalPack;
    char *pointer = buff;

    if (!pack | !buff)
        return EINVAL;

    internalPack.magic           = htonl(pack->magic);
    internalPack.messageLength   = htonl(pack->messageLength);
    internalPack.dataTotalLength = htonl(pack->dataTotalLength);

    memcpy(pointer, &(internalPack.magic), sizeof(internalPack.magic));
    pointer += sizeof(internalPack.magic);
    memcpy(pointer, &(internalPack.messageLength), sizeof(internalPack.messageLength));
    pointer += sizeof(internalPack.messageLength);
    memcpy(pointer, &(internalPack.dataTotalLength), sizeof(internalPack.dataTotalLength));
    pointer += sizeof(internalPack.dataTotalLength);
    memcpy(pointer, pack->message, sizeof(pack->message));
    return 0;
}


int
convert_network_string_to_datapack(char* string, DataPack *pack)
{
    char* pointer = string;

    if (!string || !pack)
        return EINVAL;

    pack->magic = ntohl(*(uint32_t*)pointer);
    pointer += sizeof(pack->magic);
    pack->messageLength = ntohl(*(uint32_t*)pointer);
    pointer += sizeof(pack->messageLength);
    pack->dataTotalLength = ntohl(*(uint32_t*)pointer);
    pointer += sizeof(pack->dataTotalLength);
    memcpy(pack->message, pointer, sizeof(pack->message));
    return 0;
}
    

int
compare_data_packages(DataPack* a, DataPack* b)
{   
    if (a->messageLength != b->messageLength)
    {
        return (a->messageLength > b->messageLength) ? 1 : -1;
    }

    return memcmp(a->message, b->message, a->messageLength);
}



    
