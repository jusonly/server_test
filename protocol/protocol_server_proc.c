#include <string.h>
#include "protocol_proc_abstruct.h"
#include "protocol_server_proc.h"
#include "mini_utils.h"

#define _NULL_ (0)
static send_handle_t s_send_handle;

static int32_t construct_protocol_frame(uint8_t *data, uint8_t prmcmd, uint8_t cmddir, uint8_t subcmd);

/*-Server 接收处理--------------------------------------------------------------*/
static uint8_t protocol_frame_recv_proc(const protocol_frame_t *frame)
{
    uint8_t rtvalue = 0;
    if (protocol_frame_profile_compare(frame))
    {
        /* recv term request type frame */
        if (frame->body.header.data_dir == NORMAL_DATA_DIR_TERM_TO_PLT)
        {
            int32_t send_len;
            switch (frame->body.header.prm_cmd)
            {
            case PRM_CMD_DEV_REQUSET_LOGIN:
                // todo :优化实现 用copy 然后修改的方式
                send_len = construct_protocol_frame(s_send_handle.send_buf, frame->body.header.prm_cmd, ACK_DATA_DIR_PLT_TO_TERM, _NULL_);
                if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
                break;
            case PRM_CMD_DEV_HEART_REPORT:
                send_len = construct_protocol_frame(s_send_handle.send_buf, frame->body.header.prm_cmd, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
                if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
                break;
            case PRM_CMD_DEV_CTRL_RESULT_REPORT:
                break;
            case PRM_CMD_DEV_REALTIME_DATA_REPORT:
                break;
            default:
                break;
            }
        }
        /* recv term ack type frame */
        else if (frame->body.header.data_dir == ACK_DATA_DIR_TERM_TO_PLT)
        {
        }
    }
    return rtvalue;
}

/*-构建发送帧((填充profile->填充业务数据->序列化))-------------------------------*/
static int32_t construct_protocol_frame(uint8_t *data, uint8_t prmcmd, uint8_t cmddir, uint8_t subcmd)
{
    uint8_t param_error = 0;
    int32_t len;
    protocol_frame_t frame;
    protocol_frame_t *p_frame = &frame;

    protocol_frame_profile_fill(p_frame);
    p_frame->body.header.realtime_flag = 0x01; // 默认均为实时
    p_frame->body.header.prm_cmd       = prmcmd;
    p_frame->body.header.data_dir      = cmddir;

    /* set default value */
    p_frame->body.body.is_has_sub_cmd      = 0;
    p_frame->body.body.business_data       = _NULL_;
    p_frame->body.header.business_data_len = 0;
    /* set specific value according to different cmd */
    switch (prmcmd)
    {
    case PRM_CMD_DEV_REQUSET_LOGIN:
        p_frame->body.body.business_data       = g_profile.address;
        p_frame->body.header.business_data_len = 6;
        break;
    case PRM_CMD_PLT_SYNC_TIME:
        get_device_time_flow(s_send_handle.send_buf + FRAME_BUSS_DATA_POS);
        p_frame->body.body.business_data       = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        p_frame->body.header.business_data_len = 8;
        break;
    case PRM_CMD_PLT_EN_REALTIME_REPORT:
        g_profile.active_report_period = 100;
        big_endian_store_64(s_send_handle.send_buf + FRAME_BUSS_DATA_POS, g_profile.active_report_period);
        p_frame->body.body.business_data = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        break;
    case PRM_CMD_DEV_HEART_REPORT:
        s_send_handle.send_buf[FRAME_BUSS_DATA_POS] = 0x01;
        p_frame->body.body.business_data            = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        p_frame->body.header.business_data_len      = 1;
        break;
    case PRM_CMD_PLT_QUERY_LATEST_DATA:

        break;
    case PRM_CMD_PLT_REMOTE_CONTROL:
        
        break;
    default:
        break;
    }
    if (p_frame->body.body.is_has_sub_cmd)
    {
        switch (subcmd)
        {
        default:
            break;
        }
    }
    if (!param_error)
    {
        p_frame->header.body_len = calcu_logic_protocol_len(p_frame);
        len                      = p_frame->header.body_len + sizeof(protocol_header_t);
        serialize_protocol_frame(p_frame, data);
    } else
    {
        len = -1;
    }
    return len;
}

/*-----------------------------------------------------------------------------*/

void send_restart_frame(void)
{
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_RESTART_DEVICE, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_sync_time_frame(void)
{
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_SYNC_TIME, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_realtime_report(void)
{
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_EN_REALTIME_REPORT, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_query_frame(void)
{
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_QUERY_LATEST_DATA, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_lamp_dimm_frame(int level)
{

}

void bind_server_send_handle(uint8_t *buf, uint8_t (*send_func)(uint8_t *buf, uint16_t len))
{
    s_send_handle.send_func = send_func;
    s_send_handle.send_buf  = buf;
}

void on_server_recv_data(uint8_t *buf, uint16_t len)
{
    uint16_t frame_len = 0;
    int32_t start_pos  = 0;
    protocol_frame_t frame;
    memset(&frame, 0, sizeof(protocol_frame_t));
    while (start_pos >= 0)
    {
        start_pos = protocol_frame_search(buf + frame_len, len - frame_len, &frame_len);
        if (frame_len > 0)
        {
            deserialize_protocol_frame(&frame, buf + start_pos);
            protocol_frame_recv_proc(&frame);
        } else
        {
            /* 不完整或者找不到 */
            break;
        }
    }
}