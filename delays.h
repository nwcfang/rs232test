/** random.h --- 
*
* Copyright (C) 2011 RTI im. Mintca
* Copyright (C) 2011 Egorov N.V.
*
* Author: Egorov N.V. <egorov@rti-mints.ru>
* $Id: delays.h,v 01d5796cdd2d  2012/05/28 13:17:08  egorov@rti-mints.ru $
*

* Данная программа разработана в ОАО "РТИ имени Минтца".
* 
*/

#include "datapacker.h"

#ifndef RANDOM_H
#define RANDOM_H 1

#define BASEDELAY     1e6

#define WAITBASEDELAY 5e5

/** 
 * Функция вычисления задержки в передачи сообщения
 * 
 * @param baseDelay Базовая задержка 
 * @param maxRand Допустимое отклонение от базовой задержки
 * 
 * @return случайное знаение в пределах допустимой погрешности от базовой задержки.
 */
long calculate_delay_in_usec(long baseDelay, long maxRand);

/** 
 * Фуннкция вычисления задержки исходя из скорости передачи сообщений.
 */
#define calculate_delay_from_speed_usec(speed) \
    calculate_delay_in_usec((sizeof(DataPack) * 8e6 / speed), (sizeof(DataPack) * 4e5 / speed))

/** 
 * Функция вычисление наименьшей ближайшей целой задержки по времени.
 */
#define usec_floor(val) \
    (1.0 * val <= 1.0 ? 1 : val)


#endif // RANDOM_H
