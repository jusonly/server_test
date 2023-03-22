
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*

Fix 10 Bytes
| area_id | factory_id | device_type_id | body_len |
| ------- | ---------- | -------------- | -------- |
| 2B BCD  | 2B BCD     | 2B BCD         | uint64_t |

Fix 24 Bytes
| header1 | address | header2 | logic_protocol_ver | device_type | realtime_flag | send_time | prm_cmd | data_dir | business_data_len |
| ------- | ------- | ------- | ------------------ | ----------- | ------------- | --------- | ------- | -------- | ----------------- |
| 1B      | 6B BCD  | 1B      | uint16_t           | uint8_t     | uint8_t       | 8B        | uint8_t | uint8_t  | uint16_t          |

>=1 Bytes
| sub_cmd      | business_data | crc |
| ------------ | ------------- | --- |
| optional(1B) | optional(xxB) | 1B  |

*/

#include <stdint.h>

#define CFG_PROTOCOL_VERSION 0x9999   // make sure 2Byte
#define CFG_PROTOCOL_DEVICE_TYPE 0x02 // TERM DEVICE TYPE, make sure 1Byte,

/*
为了保证通信字节流的准确性，不要试图在数据结构中包含枚举类型变量
*/
#define MEMORY_PACKED __attribute__((packed)) // or __packed
/*------------------------------------------------------------------------*/
enum {
    FRAME_PARSE_ERROR = -1,
    FRAME_PARSE_OK    = 0,
    FRAME_PARSE_INCOMPLETE,
    FRAME_PARSE_OVERFLOW,
    FRAME_PARSE_UNDERFLOW,
    FRAME_PARSE_INVALID,
    FRAME_PARSE_INVALID_LENGTH,
    FRAME_PARSE_INVALID_CHECKSUM,
    FRAME_PARSE_INVALID_TYPE,
    FRAME_PARSE_INVALID_LENGTH_TYPE,
    FRAME_PARSE_INVALID_CHECKSUM_TYPE,
    FRAME_PARSE_INVALID_TYPE_LENGTH,
    FRAME_PARSE_INVALID_TYPE_CHECKSUM,
    FRAME_PARSE_INVALID_TYPE_LENGTH_CHECKSUM,
};

/*------------------------------------------------------------------------*/
/*
+--------------+-----+-----+-----+
| Abbreviation |     |     |     |
+--------------+-----+-----+-----+
| Meaning      |     |     |     |
+--------------+-----+-----+-----+
*/

enum {
    PRM_CMD_DEV_REQUSET_LOGIN        = 0x01,
    PRM_CMD_PLT_RESTART_DEVICE       = 0x02,
    PRM_CMD_PLT_SYNC_TIME            = 0x03,
    PRM_CMD_PLT_EN_REALTIME_REPORT   = 0x05,
    PRM_CMD_DEV_HEART_REPORT         = 0x06,
    PRM_CMD_PLT_SET_PLAN_TASK        = 0x07,
    PRM_CMD_PLT_QUERY_PLAN_TASK      = 0x08,
    PRM_CMD_PLT_REMOTE_CONTROL       = 0x09,
    PRM_CMD_DEV_CTRL_RESULT_REPORT   = 0x10,
    PRM_CMD_PLT_QUERY_LATEST_DATA    = 0x12,
    PRM_CMD_DEV_REALTIME_DATA_REPORT = 0x13,
    PRM_CMD_PLT_SET_RTU_UPGRADE      = 0x14,
    PRM_CMD_PLT_SET_TERM_UPGRADE     = 0x15,
    PRM_CMD_PLT_SET_ALARM_LIMIT      = 0x16,
    PRM_CMD_DEV_ALARM_REPORT         = 0x17,
    PRM_CMD_PLT_SET_KEY_PAIR         = 0x18,
    PRM_CMD_PLT_SET_ENCRYPT_TEXT     = 0x19,
    PRM_CMD_PLT_EN_ENCRYPTION        = 0x20,
    PRM_CMD_MAX
};

enum {
    SUB_CMD_TURN_LAMP_ON = 0x01,
    SUB_CMD_TURN_LAMP_OFF,
    SUB_CMD_DIMMING,
    SUB_CMD_EXECUTE_PLAN_TASK,
    SUB_CMD_CONTROL_MAX
};

enum {
    SUB_CMD_SINGLE_REPORT = 0x01,
    SUB_CMD_BATCH_REPORT,
    SUB_CMD_REPORT_MAX
};

typedef struct {
    uint8_t prm_cmd;
    uint8_t sub_cmd;
    uint8_t (*callback)(uint8_t *data, uint16_t len);
} frame_cmd_cb_table_t;

typedef enum {
    NORMAL_DATA_DIR_RTU_TO_PLT = 0x01,
    NORMAL_DATA_DIR_PLT_TO_RTU,
    NORMAL_DATA_DIR_TERM_TO_PLT,
    NORMAL_DATA_DIR_PLT_TO_TERM,
    ACK_DATA_DIR_PLT_TO_RTU = 0x11,
    ACK_DATA_DIR_RTU_TO_PLT,
    ACK_DATA_DIR_PLT_TO_TERM,
    ACK_DATA_DIR_TERM_TO_PLT
} data_dir_t;

/* fixed-length: 10 Bytes */
typedef struct MEMORY_PACKED {
    uint8_t area_id[2];        // 2 Byte BCD
    uint8_t factory_id[2];     // 2 Byte BCD
    uint8_t device_type_id[2]; // 2 Byte BCD
    uint32_t body_len;
} protocol_header_t;

/* fixed-length: 24 Bytes */
typedef struct MEMORY_PACKED {
    uint8_t header1;    // 0x68
    uint8_t address[6]; // 6 Byte BCD,0x202301010001,2023-0101-0001
    uint8_t header2;    // 0x68
    uint16_t logic_protocol_ver;
    uint8_t device_type;
    uint8_t realtime_flag;
    uint64_t send_time;
    uint8_t prm_cmd;
    uint8_t data_dir;
    uint16_t business_data_len;
} protocol_logic_header_t;

/* unfixed-length */
typedef struct {
    uint8_t sub_cmd;
    uint8_t is_has_sub_cmd;
    uint8_t *business_data; // depends on data_len in protocol_logic_header_t
} protocol_logic_body_t;

typedef struct {
    protocol_logic_header_t header;
    protocol_logic_body_t body;
    uint8_t crc;
} protocol_body_t;

typedef struct {
    protocol_header_t header;
    protocol_body_t body;
} protocol_frame_t;

typedef void (*recv_proc_fptr_t)(const protocol_frame_t *frame);

#define FRAME_BUSS_DATA_POS (sizeof(protocol_header_t) + sizeof(protocol_logic_header_t) - 1)
#define FRAME_BUSS_DATA_POS2 (sizeof(protocol_header_t) + sizeof(protocol_logic_header_t))

int32_t protocol_frame_search(uint8_t *data, uint16_t len, uint16_t *frame_len);
uint16_t calcu_logic_protocol_len(const protocol_frame_t *frame);
void deserialize_protocol_frame(protocol_frame_t *frame, const uint8_t *data);
void serialize_protocol_frame(const protocol_frame_t *frame, uint8_t *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__PROTOCOL_H__ */