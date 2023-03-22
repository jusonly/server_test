#include "mini_utils.h"
#include <string.h>
#include <stdio.h>

uint16_t big_endian_read_16(const uint8_t *buffer)
{
    return (uint16_t)(((uint16_t)buffer[1]) | (((uint16_t)buffer[0]) << 8));
}

uint32_t big_endian_read_32(const uint8_t *buffer)
{
    return ((uint32_t)buffer[3]) | (((uint32_t)buffer[2]) << 8) | (((uint32_t)buffer[1]) << 16) | (((uint32_t)buffer[0]) << 24);
}

float big_endian_read_32f(const uint8_t *buffer)
{
    uint32_t value = ((uint32_t)buffer[3]) | (((uint32_t)buffer[2]) << 8) | (((uint32_t)buffer[1]) << 16) | (((uint32_t)buffer[0]) << 24);
    return *((float *)&value);
}

uint64_t big_endian_read_64(const uint8_t *buffer)
{
    return ((uint64_t)buffer[7]) | (((uint64_t)buffer[6]) << 8) | (((uint64_t)buffer[5]) << 16) | (((uint64_t)buffer[4]) << 24) | (((uint64_t)buffer[3]) << 32) | (((uint64_t)buffer[2]) << 40) | (((uint64_t)buffer[1]) << 48) | (((uint64_t)buffer[48]) << 56);
}

void big_endian_store_16(uint8_t *buffer, uint16_t value)
{
    buffer[0] = (uint8_t)(value >> 8);
    buffer[1] = (uint8_t)(value);
}

void big_endian_store_32(uint8_t *buffer, uint32_t value)
{
    buffer[0] = (uint8_t)(value >> 24);
    buffer[1] = (uint8_t)(value >> 16);
    buffer[2] = (uint8_t)(value >> 8);
    buffer[3] = (uint8_t)(value);
}

/* 浮点数不能直接%x打印输出 */
void big_endian_store_32f(uint8_t *buffer, float valuef)
{
    uint32_t value = *(uint32_t *)&valuef; /* 浮点数整形数二进制等价 */
    buffer[0]      = (uint8_t)(value >> 24);
    buffer[1]      = (uint8_t)(value >> 16);
    buffer[2]      = (uint8_t)(value >> 8);
    buffer[3]      = (uint8_t)(value);
}

void big_endian_store_64(uint8_t *buffer, uint64_t value)
{
    buffer[0] = (uint8_t)(value >> 56);
    buffer[1] = (uint8_t)(value >> 48);
    buffer[2] = (uint8_t)(value >> 40);
    buffer[3] = (uint8_t)(value >> 32);
    buffer[4] = (uint8_t)(value >> 24);
    buffer[5] = (uint8_t)(value >> 16);
    buffer[6] = (uint8_t)(value >> 8);
    buffer[7] = (uint8_t)(value);
}

/* https://github.com/whik/crc-lib-c */
uint8_t crc8(uint8_t *data, uint16_t length)
{
    uint8_t i;
    uint8_t crc = 0; // Initial value
    while (length--)
    {
        crc ^= *data++; // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}
uint16_t crc16_modbus(uint8_t *data, uint16_t length)
{
    uint8_t i;
    uint16_t crc = 0xffff; // Initial value
    while (length--)
    {
        crc ^= *data++; // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001; // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}
