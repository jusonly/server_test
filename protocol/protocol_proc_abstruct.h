
#ifndef __PROTOCOL_PROC_ABSTRUCT_H__
#define __PROTOCOL_PROC_ABSTRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol.h"
#include <stdint.h>

typedef struct MEMORY_PACKED {
    float voltage;         // 电压 V
    float current;         // 电流 A
    float power;           // 功率 W
    float power_factor;    // 功率因数
    float energy;          // 电能 kwh
    uint8_t lamp_state;    // 灯状态 打开0x01| 关闭0x02| 异常0x03
    uint8_t dimming_level; // 调光等级
} realtime_min_data_t;

typedef struct MEMORY_PACKED {
    float voltage;         // 电压 V
    float current;         // 电流 A
    float power;           // 功率 W
    float power_factor;    // 功率因数
    float energy;          // 电能 kwh
    float leakage;         // 漏电流 mA
    uint8_t lamp_state;    // 灯状态 打开0x01| 关闭0x02| 异常0x03
    uint8_t dimming_level; // 调光等级
} realtime_data_t;

typedef struct MEMORY_PACKED {
    uint8_t leak_alarm_flag;
    uint8_t leak_alarm_value; // mA
    uint8_t tilt_alarm_flag;
    float tilt_alarm_value;
    uint8_t repair_alarm_flag;
    uint8_t water_alaram_flag;
    uint8_t broken_alarm_flag;
    uint8_t high_temp_alarm_flag;
    float temp;
    uint8_t fuse_fault_alarm_flag;
    uint8_t lamp_fault_alarm_flag;
    uint8_t over_voltage_alarm_flag;
    uint8_t under_voltage_alarm_flag;
    uint8_t over_current_alarm_flag;
    uint8_t under_current_alarm_flag;
    uint8_t power_supply_alarm_flag;
} alarm_info_t;

typedef struct MEMORY_PACKED {
    alarm_info_t alarm_info;
    realtime_min_data_t lamp_info;
} alarm_data_t;

/*------------------------------------------------------------------------*/
typedef struct {
    uint8_t init_state;
    uint8_t dimming_level1;
    uint8_t start_time_hour;
    uint8_t start_time_min;
    uint8_t start_time_sec;
    uint32_t keep_time;
    uint8_t action;
    uint8_t dimming_level2;
} buss_data_set_plan_task_t;
/*----------------------------------------------------------------------*/
typedef struct {
    uint8_t area_id[2];        // 2 Byte BCD
    uint8_t factory_id[2];     // 2 Byte BCD
    uint8_t device_type_id[2]; // 2 Byte BCD
    uint8_t address[6];        // 6 Byte BCD
    uint16_t logic_protocol_ver;
    uint8_t device_type;
    uint32_t active_report_period;
    void (*get_time_fptr)(uint64_t *time); // 并不是直接include ，也不是在文件内部extern然后转述调用，而是预先定义好，然后初始化
    void (*get_alarm_data_fptr)(alarm_data_t *data);
    void (*get_realtime_data_fptr)(realtime_data_t *data);
} protocol_frame_profile_t;

extern protocol_frame_profile_t g_profile;
void set_protocol_frame_profile(protocol_frame_profile_t profile);
void protocol_frame_profile_fill(protocol_frame_t *frame);
uint8_t protocol_frame_profile_compare(const protocol_frame_t *frame);

void realtime_data_convert(realtime_data_t *data, int dir);
void alarm_data_convert(alarm_data_t *data, int dir);

void get_device_address_flow(uint8_t* buf);
void get_device_time_flow(uint8_t* buf);
void get_realtime_data_flow(uint8_t* buf);
void get_device_alarm_data_flow(uint8_t *buf);
void modify_protocol_frame_time(protocol_frame_t *frame);

typedef struct {
    uint8_t *send_buf;
    uint8_t (*send_func)(uint8_t *buf, uint16_t len);
} send_handle_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __PROTOCOL_PROC_ABSTRUCT_H__ */