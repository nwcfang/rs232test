/** server.h --- 
 *
 * Copyright (C) 2011 RTI im. Mintca
 * Copyright (C) 2011 Egorov N.V.
 *
 * Author: Egorov N.V. <egorov@rti-mints.ru>
 * $Id: server.h,v 01d5796cdd2d  2012/05/28 13:17:08  egorov@rti-mints.ru $
 *

 * Данная программа разработана в ОАО "РТИ имени Минтца".
 * 
 */

#include "config.h"


#ifndef SERVER_H
#define SERVER_H 1
/** 
 * @brief Функция запуска для процесса "сервера"
 * 
 * @param config Настройки приложения.
 * 
 * @return 0 в случае успеха и код ошибки в случае ошибки.
 */
int
server_process(const Configuration *config);

#endif // SERVER_H
