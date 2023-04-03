#include "protocol.h"
#include "mini_utils.h"
#include <string.h>
#include "stdio.h"

#define _NULL_ (0)
/* 解析成功（指针复位，清除数据）
        返回成功
        返回错误
        返回内部参数错
        返回命令不支持
    解析失败，数据太短（指针不复位，保留数据）
    解析失败，校验不通过（但缓冲区未满，则追加到缓冲区，指针复位，清除数据）
*/
/*
    平台到设备数据传输时间 t1
    设备响应时间 t2(由解析时间... )
    数据解析的频率：来数据就解析/定时解析
*/

static uint8_t is_has_sub_cmd(uint8_t prm_cmd)
{
    uint8_t rtvalue = 0;
    if (prm_cmd == PRM_CMD_PLT_REMOTE_CONTROL || prm_cmd == PRM_CMD_DEV_CTRL_RESULT_REPORT || prm_cmd == PRM_CMD_DEV_REALTIME_DATA_REPORT)
        rtvalue = 1;
    return rtvalue;
}

/*---------------------------------------------------------------------------------------------------*/
uint16_t calcu_logic_protocol_len(const protocol_frame_t *frame)
{
    /* logic_header + logic_body + crc(1 Byte)*/
    uint32_t len = sizeof(protocol_logic_header_t) + frame->body.header.biz_data_len + 1;

    if (frame->body.body.is_has_sub_cmd)
    {
        len++;
    }
    return len;
}

/*---------------------------------------------------------------------------------------------------*/

/** 大小端转换
 * @brief  对协议非业务数据的部分做内存数据大小端转换
 * @param[in]  指向转换前的内存区域
 * @param[out] 指向转换后的内存区域
 * @param  dir 0:大端->小段| 1:小段转大端
 */
void protocol_frame_memory_convert(protocol_frame_t *frame, uint8_t dir)
{
    if (dir == 0)
    {
        frame->header.body_len                = big_endian_read_32((uint8_t *)&frame->header.body_len);
        frame->body.header.logic_protocol_ver = big_endian_read_16((uint8_t *)&frame->body.header.logic_protocol_ver);
        frame->body.header.biz_data_len  = big_endian_read_16((uint8_t *)&frame->body.header.biz_data_len);
        frame->body.header.send_time          = big_endian_read_64((uint8_t *)&frame->body.header.send_time);
    } else
    {
        big_endian_store_32((uint8_t *)&frame->header.body_len, frame->header.body_len);
        big_endian_store_16((uint8_t *)&frame->body.header.logic_protocol_ver, frame->body.header.logic_protocol_ver);
        big_endian_store_16((uint8_t *)&frame->body.header.biz_data_len, frame->body.header.biz_data_len);
        big_endian_store_64((uint8_t *)&frame->body.header.send_time, frame->body.header.send_time);
    }
}

void deserialize_protocol_frame(protocol_frame_t *frame, const uint8_t *data)
{
    uint16_t pos;
    /* Step1: memcpy(copy to memory struct from buffer) */
    memcpy((void *)&frame->header, data, sizeof(protocol_header_t));
    memcpy((void *)&frame->body, data + sizeof(protocol_header_t), sizeof(protocol_logic_header_t));
    pos = sizeof(protocol_header_t) + sizeof(protocol_logic_header_t);

    if (is_has_sub_cmd(frame->body.header.prm_cmd))
    {
        frame->body.body.is_has_sub_cmd = 1;
        frame->body.body.sub_cmd        = data[pos++];
    }
    else
    {
        frame->body.body.is_has_sub_cmd = 0;
    }
    if (frame->body.header.biz_data_len > 0)
    {
        frame->body.body.biz_data_ptr = (uint8_t *)(data + pos);
    }
    /* Step2: data convert(big endian to little endian) */
    protocol_frame_memory_convert(frame, 0);
}

void serialize_protocol_frame(const protocol_frame_t *frame, uint8_t *data)
{
    uint16_t pos;
    protocol_frame_t frame_tmp;
    protocol_frame_t *p_frame_tmp = &frame_tmp;
    /* Step1: copy tmp */
    memcpy((void *)&frame_tmp, frame, sizeof(protocol_frame_t));

    /* Step2: data convert(little endian to big endian), not involve the business data */
    protocol_frame_memory_convert(p_frame_tmp, 1);
    /* Step3: memcpy(copy to buffer from memory struct) */
    memcpy(data, (void *)&p_frame_tmp->header, sizeof(protocol_header_t));
    memcpy(data + sizeof(protocol_header_t), (void *)&p_frame_tmp->body, sizeof(protocol_logic_header_t));

    /* Step4: copy business data, use origin frame data(the part data memory convert by itself) */
    pos = sizeof(protocol_header_t) + sizeof(protocol_logic_header_t);

    if (frame->body.body.is_has_sub_cmd)
    {
        data[pos++] = frame->body.body.sub_cmd;
        if (frame->body.header.biz_data_len > 0)
        {
            /* have sub cmd */
            memcpy(data + pos, (void *)frame->body.body.biz_data_ptr, frame->body.header.biz_data_len - 1);
        }
        data[pos + frame->body.header.biz_data_len] = crc8(data + sizeof(protocol_header_t), frame->header.body_len - 2);
    }
    else
    {
        /* no sub cmd */
        if (frame->body.header.biz_data_len > 0)
        {
            memcpy(data + pos, (void *)frame->body.body.biz_data_ptr, frame->body.header.biz_data_len);
        }
        data[pos + frame->body.header.biz_data_len] = crc8(data + sizeof(protocol_header_t), frame->header.body_len - 1);
    }
}

int32_t protocol_frame_search(uint8_t *data, uint16_t len, uint16_t *frame_len)
{
    int32_t start_pos, incompleted_frame_start_pos;
    uint16_t id_char1_pos, id_char2_pos;
    uint16_t body_len_pos, logic_body_len_pos;
    uint32_t body_len, body_len_cmp;
    uint16_t logic_body_len;
    const uint16_t frame_min_len = sizeof(protocol_header_t) + sizeof(protocol_logic_header_t) + 1; /* 1 is the len of crc */

    incompleted_frame_start_pos = -1;
    if (len >= frame_min_len)
    {
        for (start_pos = 0; start_pos < len - frame_min_len + 1; start_pos++)
        {
            id_char1_pos = start_pos + sizeof(protocol_header_t);
            id_char2_pos = id_char1_pos + 7; /* 7 is the len of (header1+address[6]) in protocol_logic_header_t*/
            if (data[id_char1_pos] == 0x68 && data[id_char2_pos] == 0x68)
            {
                body_len_pos       = start_pos + 6;                 /* 6 is the len of (area_id+factory_id+device_type_id) in protocol_header_t*/
                logic_body_len_pos = start_pos + frame_min_len - 3; /* 3 is the len of (data_len) in protocol_logic_header_t and 1 Byte crc*/
                body_len           = big_endian_read_32(data + body_len_pos);
                logic_body_len     = big_endian_read_16(data + logic_body_len_pos);
                body_len_cmp = logic_body_len + sizeof(protocol_logic_header_t) + 1; /* crc occupy 1Byte */
                if (body_len == body_len_cmp || body_len == (body_len_cmp+1) ) /* note: subcmd occupy another 1Byte*/
                {
                    /* case1：找到完整帧，结束循环 */
                    //@todo: crc
                    *frame_len = body_len + sizeof(protocol_header_t);
                    return start_pos;
                }
            } else if (data[id_char1_pos] == 0x68)
            {
                /* case2：找到不完整帧，继续找 */
                incompleted_frame_start_pos = start_pos;
                *frame_len                  = 0;
            }
        }
    }
    /* case2：遍历结果：找不到数据帧 or 帧不完整 */
    *frame_len = 0;
    return incompleted_frame_start_pos;
}

/** 接收解析->结构化->逻辑处理
 * @brief  协议接收解析
 * @param  data 接收缓冲区
 * @param  len 接收的数据长度
 * @param  frame 结构化帧指针
 */
void protocol_frame_parse(uint8_t *data, uint16_t len)
{
    uint16_t frame_len = 0;
    int32_t start_pos  = 0;
    protocol_frame_t frame;
    while (start_pos >= 0)
    {
        start_pos = protocol_frame_search(data + frame_len, len - frame_len, &frame_len);
        if (frame_len > 0)
        {
            deserialize_protocol_frame(&frame, data + start_pos);
        } else
        {
            /* 不完整或者找不到 */
            break;
        }
    }
}