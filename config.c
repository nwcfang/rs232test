/** config.c --- Основная настройка и расчет параметров программы
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Egorov N.V.
 *
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: config.c,v 515d326184ef  2012/10/18 09:52:54  egorov@rti-mints.ru $
 *

 * Данная программа разработана в ОАО "РТИ имени Минтца".
 * 
 */


#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include <tio.h>

#include "config.h"




/*Configuration config = {
    115200, // speed
    -1,     // device fd
    "/dev/ttyUSB0", // device path
    1000,       // minimu m transfered data count 
    CLIENTMODE, // mode of test
    0, // duration (nowtime is unused)
    1 // work mode
};*/

Configuration config = {
    /*115200,*/
    -1,     //device fd
    /*"/dev/ttyS0",   //device path*/
    1000,      //minimu m transfered data count 
    /*CLIENTSERVERMODE,      //mode of test*/
    /*0,      //duration (nowtime is unused)*/
    1       //work mode
};

static int
calculate_configuration(Configuration *cfg)
{
    if (!cfg)
        return EINVAL;
    
    /*if (cfg->duration)*/
    if( tioGetDefL( "DURATION", 0 ) )
    {
        /*cfg->sendPacksLength = (cfg->duration * cfg->portSpeed / 8);*/
        cfg->sendPacksLength = ( tioGetL( "DURATION" ), tioGetDefL( "PORTSPEED", 115200 ) / 8);
        
        /*cfg->duration = 0;*/
    }
    return 0;

}


int
/*write_configuration(Configuration *cfg, const char *argv[], int argc)*/
write_configuration(Configuration *cfg )
{
    /*int opt;
    int already_typed = 0;

    if (!cfg || !argv ||  argc < 1)
        return EINVAL;*/

    //Получение параметров командной строки
    /*cfg->duration = tioGetDefL( "DURATION", 0 );*/
    /*cfg->portSpeed = tioGetDefL( "PORTSPEED", 115200 );*/
    cfg->sendPacksLength = tioGetDefL( "SENDPACKSLENGTH", 1000 );
    /*if( tioGetL( "SERVERMODE" ) > 0 )
        cfg->serverClientMode = SERVERMODE;
    else if( tioGetL( "CLIENTSERVERMODE" ) > 0 )
        cfg->serverClientMode = CLIENTSERVERMODE;
    else // По умолчанию тест заботает в режиме клиента 
        cfg->serverClientMode = CLIENTMODE;*/

    /*while (-1 != (opt = getopt(argc, argv, "D:m:s:dlLh")))
    {
        switch(opt)
        {
        case 'D':
            cfg->duration = atol(optarg);
            if (cfg->duration <= 0)
                return EAGAIN;
            break;
        case 'm':
            cfg->portSpeed = atol(optarg);
            if (cfg->portSpeed <= 0)
                return EAGAIN;
            break;
        case 's':
            cfg->sendPacksLength = atol(optarg);
            if (cfg->sendPacksLength <= 0)
                return EAGAIN;
            break;
        case 'd':
            if (already_typed)
                return EAGAIN;
            cfg->serverClientMode = SERVERMODE;
            already_typed = 1;
            break;
        case 'l':
            if (already_typed)
                return EAGAIN;
            cfg->serverClientMode = CLIENTMODE;
            already_typed = 1;
            break;
        case 'L':
            if (already_typed)
                return EAGAIN;
            cfg->serverClientMode = CLIENTSERVERMODE;
            already_typed = 1;
            break;
        default:
            return EAGAIN;
        }
    }*/
    

    calculate_configuration(cfg);
    
    /*if (optind < argc)
        strcpy(config.DeviceName, argv[optind]);*/
   /*strcpy( config.DeviceName, tio_argv[0] ); */
   /*puts( "debug:" );*/
   /*puts( config.DeviceName );*/

    return 0;
}
            

                

    
    

    
