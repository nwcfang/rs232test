/** config.c --- Основная настройка и расчет параметров программы
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Gusev M.S. 
 *
 * Author: Gusev M.S. <gpinok@gmail.com>
 * $Id: config.c,v 515d326184ef  2012/10/18 09:52:54  gpinok@gmail.com $
 *
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





Configuration config = {
    -1,         //device fd
    1000,       //minimu m transfered data count 
    1           //work mode
};

static int
calculate_configuration(Configuration *cfg)
{
    if (!cfg)
        return EINVAL;
    
    if( tioGetDefL( "DURATION", 0 ) )
        cfg->sendPacksLength = ( tioGetL( "DURATION" ), tioGetDefL( "PORTSPEED", 115200 ) / 8);
    return 0;
}


int
write_configuration(Configuration *cfg )
{
    cfg->sendPacksLength = tioGetDefL( "SENDPACKSLENGTH", 1000 );
    calculate_configuration(cfg);
    return 0;
}
            

                

    
    

    
