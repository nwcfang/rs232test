/** main.c --- 
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Egorov N.V.
 *
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: main.c,v 158279b7769b  2011/10/14 13:22:37  egorov@rti-mints.ru $
 *

 * Данная программа разработана в ОАО "РТИ имени Минтца".
 * 
 */

#define _POSIX_SOURCES 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include <tio.h>

#include "client.h"
#include "server.h"
#include "config.h"
#include "termcontrol.h"
#include "debuging.h"


void termination_signal(int signum)
{
    int work = config.work;        
    config.work = 0;
    if (config.outputDevice != -1)
    {
        close_serial_port(config.outputDevice);
        config.outputDevice = -1;
    }
    if (work)
        kill(0, SIGTERM);
}

int main(int argc, const char* argv[])
{
    pid_t server_child = 0;
    int fd;
    int res = 0, status;
    struct sigaction sigchildAction;

    tio_param sParam [] = {
       {"D:", "DURATION", "Duration"},
       { "m:", "PORTSPEED", "Prot speed"},
       { "s:", "SENDPACKSLENGTH", "Send pack length"},
       { "d", "SERVERMODE", "Server mode" },
       { "l", "CLIENTMODE", "Client mode" },
       { "L", "CLIENTSERVERMODE", "Client/Server mode" }, 
       {NULL, NULL, NULL}
    };

    /*puts("Test started"); */


    sigchildAction.sa_handler = termination_signal;
    sigchildAction.sa_flags    = SA_NOCLDSTOP;
    sigemptyset(&(sigchildAction.sa_mask));
    sigaddset(&(sigchildAction.sa_mask),SIGTERM);

    if (sigaction(SIGTERM, &sigchildAction, NULL))
    {
        perror("Signal SIGTERM registration failed");
        return -1;
    }
    if (sigaction(SIGINT, &sigchildAction, NULL))
    {
        perror("Signal SIGINT registration failed");
        return -1;
    }

    // Инициализация tio и разбор входных параметров командной строки  
    tioInit( "alpha", "RS232 test", sParam, argc, argv); 
    
    if (write_configuration(&config))
    {
        fputs("Congiguration read error\n", stderr);
        return -1;
    }
    
    /*fd = open_serial_port(config.DeviceName, config.portSpeed);*/
    fd = open_serial_port( tio_argv[0], tioGetDefL( "PORTSPEED", 115200 ));
    if (fd < 0)
    {
        return -1;
    }
    config.outputDevice = fd;
    
    /*if (config.serverClientMode == CLIENTSERVERMODE)*/
    if ( tioGetL( "CLIENTSERVERMODE" ) > 0 )
    {
        server_child = fork();
    }
    /*if ((server_child == 0) && (config.serverClientMode & SERVERMODE))*/
    if ((server_child == 0) && (tioGetL( "CLIENTSERVERMODE" ) || tioGetL( "SERVERMODE" ) ) )
    {
        if (server_process(&config))
        {
            return -1;
        }
    }
    /*else if (config.serverClientMode & CLIENTMODE)*/
    else if ( tioGetL( "CLIENTSERVERMODE" ) || tioGetL( "CLIENTMODE" ))
        res = client_process(&config);
    else 
    {
        DEBUGMSG("Undefined target action");
        return -1;
    }

    if (server_child != 0)
        waitpid(server_child, &status, WNOHANG);

    // Завершение работы библиотеки tio  
    tioFinish(0);

    //return (int)(res || status);
    return 0;
}
        
    
