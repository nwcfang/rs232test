/** datapacker.h --- 
*
* Copyright (C) 2011 RTI im. Mintca
* Copyright (C) 2011 Egorov N.V.
*
* Author: Egorov N.V. <egorov@rti-mints.ru>
* $Id: datapacker.h,v 01d5796cdd2d  2012/05/28 13:17:08  egorov@rti-mints.ru $
*

* Данная программа разработана в ОАО "РТИ имени Минтца".
* 
*/

#include <stdint.h>

#ifndef DATAPACKER_H
#define DATAPACKER_H 1

/// Флаг ожидания запроса от отправителя
#define WAITMAGIC        0xA99AFF1F 
/// Флаг метка начала сообщения
#define SENDMAGIC        0xA99AFF2F
/// Флаг сообщение готовности получать данные
#define RECIVEREADYMAGIC 0xA99AFF4F

#define MESSAGELENGTH    64


/**
 * Структура данных для передачи по сети.
 * 
 */
typedef struct  {
    uint32_t magic;             /**< Код начала сообщения. */
    uint32_t messageLength;     /**< Длинна сообщения. */
    uint32_t dataTotalLength;   /**< Количесво передаваемых пакетов. */
    uint8_t  message[MESSAGELENGTH]; /**< Строка-сообщение */
} DataPack;

extern char messageString[MESSAGELENGTH];

/**
 * @brief Функция преобразования строки в пакет данных.
 * 
 * Функция упаковывает сообщение длинной не более MESSAGELENGTH в
 * структуру ::DataPack для передачии ее через последовательный порт.
 *
 * @param message Упаковываемая строка.
 * @param messageLength Длинна упаковываемой строки.
 * @param pack Указатель на неинициализированную структуру DataPack
 * 
 * @return 0
 */
int 
convert_message_to_datapack(char *message, size_t messageLength, DataPack *pack);

/**
 * @brief Функция преобразования пакета данных для передачи в сеть
 * 
 * Функция преобразует пакет данных в последовательность байт для
 * передачи по сети. Все числовые данные преобразуется в сетевой
 * порядок байт. Обратное преобразование дедлается функцией
 * convert_network_string_to_datapack().
 *
 * @param pack Пакет данных для преобразования.
 * @param buff Буффер длинной не менее sizeof(DataPack)
 * 
 * @return 0 при успешном завершении и код ошибки в противном случае.
 */
int 
convert_datapack_to_network_string(DataPack *pack, void* buff);


/**
 * @brief Функция сравнения двух пакетов данных
 * 
 * @param a 
 * @param b 
 * 
 * @return 
 */
int
compare_data_packages(DataPack* a, DataPack* b);

/**
 * Функция преобразования последовательности байт в структуру DataPack.
 * 
 * Функция распоковывает данные упакованные при помощи вызова
 * convert_datapack_to_network_string()
 *
 * @param string Срака данных полученных по сети
 * @param pack Пакет в который производится распаковка.
 * 
 * @return 0 при успешном завершении и -1 в противном случае.
 */
int
convert_network_string_to_datapack(char* string, DataPack *pack);

#endif // DATAPACKER_H
