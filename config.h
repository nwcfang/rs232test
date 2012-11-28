/** config.h --- 
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Egorov N.V.
 *
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: config.h,v 158279b7769b  2011/10/14 13:22:37  egorov@rti-mints.ru $
 *
 
 * Данная программа разработана в ОАО "РТИ имени Минтца".
 * 
 */


#include <limits.h>

#ifndef CONFIG_H
#define CONFIG_H 1

#define SERVERMODE       0x1
#define CLIENTMODE       0x2
#define CLIENTSERVERMODE 0x3

typedef struct {
    long portSpeed;
    int  outputDevice;
    char DeviceName[PATH_MAX];
    long sendPacksLength;
    int  serverClientMode;
    long duration;
    int  work;
} Configuration;

extern Configuration config;

int
write_configuration(Configuration *cfg);


#endif // CONFIG_H
