#include <string.h>
#include "protocol_proc_abstruct.h"
#include "mini_utils.h"
#include <stdio.h>
protocol_frame_profile_t g_profile;

void set_protocol_frame_profile(protocol_frame_profile_t profile)
{
    memcpy((void *)&g_profile, (void *)&profile, sizeof(protocol_frame_profile_t));
}

void get_device_address_flow(uint8_t *buf)
{
    memcpy(buf, g_profile.address, sizeof(g_profile.address));
}

void get_device_time_flow(uint8_t *buf)
{
    uint64_t time = 0;
    if (g_profile.get_time_fptr) g_profile.get_time_fptr(&time);
    big_endian_store_64(buf, time);
}

void get_realtime_data_flow(uint8_t *buf)
{
    realtime_data_t real_data;
    // realtime_data_t real_data;
    // real_data.voltage       = 220.0; //电压 V
    // real_data.current       = 1.0;   // 电流 A
    // real_data.power         = 220.0; // 功率 W
    // real_data.power_factor  = 1.0;   // 功率因数
    // real_data.energy        = 1.0;   // 电能 kwh
    // real_data.leakage       = 0.001; // 漏电流 mA
    // real_data.lamp_state    = 0x01;  // 灯状态 打开0x01| 关闭0x02| 异常0x03
    // real_data.dimming_level = 100;   // 调光等级
    memset(&real_data, 0, sizeof(realtime_data_t));
    if (g_profile.get_realtime_data_fptr) g_profile.get_realtime_data_fptr(&real_data);
    realtime_data_convert(&real_data, 1);
    memcpy(buf, &real_data, sizeof(realtime_data_t));
}

void get_device_alarm_data_flow(uint8_t *buf)
{
    alarm_data_t alarm_data;
    memset((uint8_t *)&alarm_data, 0, sizeof(alarm_data_t));
    if (g_profile.get_alarm_data_fptr) g_profile.get_alarm_data_fptr(&alarm_data);
    alarm_data_convert(&alarm_data, 1);
    memcpy(buf, &alarm_data, sizeof(alarm_data_t));
}

void modify_protocol_frame_time(protocol_frame_t *frame)
{
    if (g_profile.get_time_fptr)
    {
        /* 不要尝试对未对齐结构体的某个成员取地址来直接操作赋值(可能奇地址)
            错误的方法:
            g_profile.get_time_fptr(&frame->body.header.send_time);
        */
        uint64_t time;
        g_profile.get_time_fptr(&time);
        memcpy(&frame->body.header.send_time, &time, sizeof(uint64_t));
    }
}

void protocol_frame_profile_fill(protocol_frame_t *frame)
{
    /* Step1: 清空 */
    memset((void *)frame, 0, sizeof(protocol_frame_t)); // must set default value

    /* Step1: 配置值 */
    memcpy((void *)frame->header.area_id, (void *)g_profile.area_id, sizeof(g_profile.area_id));
    memcpy((void *)frame->header.factory_id, (void *)g_profile.factory_id, sizeof(g_profile.factory_id));
    memcpy((void *)frame->header.device_type_id, (void *)g_profile.device_type_id, sizeof(g_profile.device_type_id));

    frame->body.header.header1 = 0x68;
    memcpy((void *)frame->body.header.address, (void *)g_profile.address, sizeof(g_profile.address));
    frame->body.header.header2            = 0x68;
    frame->body.header.logic_protocol_ver = g_profile.logic_protocol_ver;
    frame->body.header.device_type        = g_profile.device_type;

    if (g_profile.get_time_fptr)
    {
        /* 不要尝试对未对齐结构体的某个成员取地址来直接操作赋值(可能奇地址)
            错误的方法:
            g_profile.get_time_fptr(&frame->body.header.send_time);
        */
        uint64_t time;
        g_profile.get_time_fptr(&time);
        memcpy(&frame->body.header.send_time, &time, sizeof(uint64_t));
    }
}

uint8_t protocol_frame_profile_compare(const protocol_frame_t *frame)
{
    uint8_t cmplen, rtvalue;
    int i, j;
    protocol_frame_t tmp_frame;
    protocol_frame_profile_fill(&tmp_frame);

    cmplen = 6; /* 6 equal the size of area_id+factory_id+device_type_id */
    i      = memcmp((void *)&tmp_frame.header, (void *)&frame->header, cmplen);
    cmplen = 11; /* 11 equal the size of header1+address+header2+logic_protocol_ver+device_type */
    j      = memcmp((void *)&tmp_frame.body.header, (void *)&frame->body.header, cmplen);

    rtvalue = 0;
    if (!i && !j)
    {
        rtvalue = 1;
    }
    return rtvalue;
}

void alarm_data_convert(alarm_data_t *data, int dir)
{
    if (dir == 0)
    {
        data->alarm_info.tilt_alarm_value = big_endian_read_32f((uint8_t *)&data->alarm_info.tilt_alarm_value);
        data->alarm_info.temp             = big_endian_read_32f((uint8_t *)&data->alarm_info.temp);
        data->lamp_info.voltage           = big_endian_read_32f((uint8_t *)&data->lamp_info.voltage);
        data->lamp_info.current           = big_endian_read_32f((uint8_t *)&data->lamp_info.current);
        data->lamp_info.power             = big_endian_read_32f((uint8_t *)&data->lamp_info.power);
        data->lamp_info.power_factor      = big_endian_read_32f((uint8_t *)&data->lamp_info.power_factor);
        data->lamp_info.energy            = big_endian_read_32f((uint8_t *)&data->lamp_info.energy);
    } else
    {
        big_endian_store_32f((uint8_t *)&data->alarm_info.tilt_alarm_value, data->alarm_info.tilt_alarm_value);
        big_endian_store_32f((uint8_t *)&data->alarm_info.temp, data->alarm_info.temp);
        big_endian_store_32f((uint8_t *)&data->lamp_info.voltage, data->lamp_info.voltage);
        big_endian_store_32f((uint8_t *)&data->lamp_info.current, data->lamp_info.current);
        big_endian_store_32f((uint8_t *)&data->lamp_info.power, data->lamp_info.power);
        big_endian_store_32f((uint8_t *)&data->lamp_info.power_factor, data->lamp_info.power_factor);
        big_endian_store_32f((uint8_t *)&data->lamp_info.energy, data->lamp_info.energy);
    }
}

void realtime_data_convert(realtime_data_t *data, int dir)
{
    if (dir == 0)
    {
        data->voltage      = big_endian_read_32f((uint8_t *)&data->voltage);
        data->current      = big_endian_read_32f((uint8_t *)&data->current);
        data->power        = big_endian_read_32f((uint8_t *)&data->power);
        data->power_factor = big_endian_read_32f((uint8_t *)&data->power_factor);
        data->energy       = big_endian_read_32f((uint8_t *)&data->energy);
        data->leakage      = big_endian_read_32f((uint8_t *)&data->leakage);
    } else
    {
        big_endian_store_32f((uint8_t *)&data->voltage, data->voltage);
        big_endian_store_32f((uint8_t *)&data->current, data->current);
        big_endian_store_32f((uint8_t *)&data->power, data->power);
        big_endian_store_32f((uint8_t *)&data->power_factor, data->power_factor);
        big_endian_store_32f((uint8_t *)&data->energy, data->energy);
        big_endian_store_32f((uint8_t *)&data->leakage, data->leakage);
    }
}