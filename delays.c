/** delays.c --- 
*
* Copyright (C) 2011 RTI im. Mintca
* Copyright (C) 2011 Egorov N.V.
*
* Author: Egorov N.V. <egorov@rti-mints.ru>
* $Id: delays.c,v 158279b7769b  2011/10/14 13:22:37  egorov@rti-mints.ru $
*

* Данная программа разработана в ОАО "РТИ имени Минтца".
* 
*/

#include <stdlib.h>
#include "delays.h"


#define positive_limited_rand(limit) \
    ((rand() * limit) / RAND_MAX)

long calculate_delay_in_usec(long baseDelay, long maxRand)
{
    return baseDelay + positive_limited_rand(maxRand);
}

