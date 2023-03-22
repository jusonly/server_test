#ifndef __MINI_UTILS_H__
#define __MINI_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint16_t big_endian_read_16(const uint8_t* buffer);
uint32_t big_endian_read_32(const uint8_t* buffer);
float big_endian_read_32f(const uint8_t *buffer);

void big_endian_store_16(uint8_t* buffer, uint16_t value);
void big_endian_store_32(uint8_t* buffer, uint32_t value);
void big_endian_store_32f(uint8_t *buffer, float valuef);

uint64_t big_endian_read_64(const uint8_t *buffer);
void big_endian_store_64(uint8_t *buffer, uint64_t value);

uint8_t crc8(uint8_t *data, uint16_t length);
uint16_t crc16_modbus(uint8_t *data, uint16_t length);

#ifdef __cplusplus  
}
#endif  /* __cplusplus */

#endif  /* __MINI_UTILS_H__ */

