/** termcontrol.c --- подпрограммы управления последовательным портом
*
* Copyright (C) 2011 RTI im. Mintca
* Copyright (C) 2011 Egorov N.V.
*
* Author: Egorov N.V. <egorov@rti-mints.ru>
* $Id: termcontrol.c,v 01d5796cdd2d  2012/05/28 13:17:08  egorov@rti-mints.ru $
*

* Данная программа разработана в ОАО "РТИ имени Минтца".
* 
*/

#define _BSD_SOURCES

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>


#include "termcontrol.h"



/** 
 * @brief Функция перевода численного значения скорости в константу
 * скорости передачи данных по COM порту.
 *
 * Функция возвращает значение константы скорости для выбранного
 * скоростного режима. В случае если для последовательного порта такой
 * скоростной режим невозможно возвращается константа B0 а значение
 * errno устанавливается равным EINVAL.
 * 
 * @param speedValue - численное значение желаемой скороси
 */
static speed_t 
choose_speed(long speedValue)
{
    speed_t speed;
    switch (speedValue)
    {
    case 0:
        speed = B0;
        break;
    case 50:
        speed = B50;
        break;
    case 75:
        speed = B75;
        break;
    case 110:
        speed = B110;
        break;
    case 134:
        speed = B134;
        break;
    case 150:
        speed = B150;
        break;
    case 200:
        speed = B200;
        break;
    case 300:
        speed = B300;
        break;
    case 600:
        speed = B600;
        break;
    case 1200:
        speed = B1200;
        break;
    case 1800:
        speed = B1800;
        break;
    case 2400:
        speed = B2400;
        break;
    case 4800:
        speed = B4800;
        break;
    case 9600:
        speed = B9600;
        break;
    case 19200:
        speed = B19200;
        break;
    case 38400:
        speed = B38400;
        break;
    case 57600:
        speed = B57600;
        break;
    case 115200:
        speed = B115200;
        break;
    case 230400:
        speed = B230400;
        break;
    default:
        speed = B0;
        errno = EINVAL;
    }
    return speed;
}



int
open_serial_port(const char *path, long speed)
{
    int fd;
    speed_t speedValue;
    struct termios terminalStatus;

    fd = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (-1 == fd)
    {
        perror("Open serial device failed:");
        return -1;
    }
    
    if (tcgetattr(fd, &terminalStatus))
    {
        perror("Get status of serial port failed:");
        close(fd);
        return -2;
    }

    cfmakeraw(&terminalStatus);
    speedValue = choose_speed(speed);
    if (speedValue == B0 && errno == EINVAL)
    {
        perror("Wrong speed parametr selected:");
        close(fd);
        return -3;
    }
    cfsetspeed(&terminalStatus, speedValue);
    if (tcsetattr(fd, TCSANOW, &terminalStatus))
    {
        perror("Can not configure serial port");
        close(fd);
        return -4;
    }

    return fd;

}


