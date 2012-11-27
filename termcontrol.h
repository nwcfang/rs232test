/** termcontrol.h --- 
*
* Copyright (C) 2011 RTI im. Mintca
* Copyright (C) 2011 Egorov N.V.
*
* Author: Egorov N.V. <egorov@rti-mints.ru>
* $Id: termcontrol.h,v 01d5796cdd2d  2012/05/28 13:17:08  egorov@rti-mints.ru $
*

* Данная программа разработана в ОАО "РТИ имени Минтца".
* 
*/

#include <unistd.h>

#ifndef TERMCONTROL_H
#define TERMCONTROL_H 1

/** 
 * @brief Функция открытия последовательного порта.
 *
 * Функция открывает последовательный порт и выставляет параметры
 * скорости передачи данных после чего возвращает дискриптор открытого
 * файла устройства. В случае неуспешного открытия или установки
 * параметров возвращает значение -1.
 * 
 * @param path Путь к файлу устройства последовательного порта.
 * @param speed Скорость передачи данных через порт.
 */        
int
open_serial_port(const char *path, long speed);

/** 
 * @brief Функция запкрытия открытого файла устройства последовательно порта.
 * 
 * @param fd Открытый при помощи функции open_serial_port() дескриптор
 * фалова последовательного порта.
 * 
 * @return 0 при успешном завершении и -1 в случае ошибки.
 */
#define close_serial_port(fd)\
    close(fd)


#endif // TERMCONTROL_H
