
#include <string.h>
#include <stdio.h>
#include "protocol_proc_abstruct.h"
#include "mini_utils.h"

#define _NULL_ (0)

#define MUX_REPORT_DATA_POS (FRAME_BUSS_DATA_POS + 8)
#define MUX_REPORT_DATA_POS2 (FRAME_BUSS_DATA_POS2 + 8)
#define TERM_CONSTRUCT_FRAME(cmd) construct_protocol_frame(s_send_handle.send_buf, cmd, NORMAL_DATA_DIR_TERM_TO_PLT)

static send_handle_t s_send_handle;

static int32_t construct_protocol_frame(uint8_t *data, uint8_t prmcmd, uint8_t cmddir);

static void calcu_frame_len_and_send(protocol_frame_t *frame)
{
    int32_t send_len;
    frame->header.body_len = calcu_logic_protocol_len(frame);
    send_len               = frame->header.body_len + sizeof(protocol_header_t);
    if (send_len > 0)
    {
        serialize_protocol_frame(frame, s_send_handle.send_buf);
        s_send_handle.send_func(s_send_handle.send_buf, send_len);
    }
}

static void copy_and_modify_time_dir(protocol_frame_t *frame_tmp, const protocol_frame_t *frame, uint8_t dir)
{
    memcpy(frame_tmp, frame, sizeof(protocol_frame_t));
    modify_protocol_frame_time(frame_tmp);
    frame_tmp->body.header.data_dir = dir;
}

uint8_t send_response_to_platform_request_frame(const protocol_frame_t *frame)
{
    protocol_frame_t frame_tmp;
    switch (frame->body.header.prm_cmd)
    {
    /* 终端重启回复 */
    case PRM_CMD_PLT_RESTART_DEVICE:
        copy_and_modify_time_dir(&frame_tmp, frame, NORMAL_DATA_DIR_TERM_TO_PLT);

        s_send_handle.send_buf[FRAME_BUSS_DATA_POS] = 0x01; // 确认重启
        frame_tmp.body.body.business_data           = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        frame_tmp.body.header.business_data_len     = 1;
        calcu_frame_len_and_send(&frame_tmp);
        // send_len = construct_protocol_frame(s_send_handle.send_buf, frame->body.header.prm_cmd, NORMAL_DATA_DIR_TERM_TO_PLT);
        // if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
        // todo: need to restart device
        break;
    /* 校时回复 */
    case PRM_CMD_PLT_SYNC_TIME:
        // todo: need to call time sync function
        copy_and_modify_time_dir(&frame_tmp, frame, NORMAL_DATA_DIR_TERM_TO_PLT);
        get_device_time_flow(s_send_handle.send_buf + FRAME_BUSS_DATA_POS);
        frame_tmp.body.body.business_data       = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        frame_tmp.body.header.business_data_len = 8;
        calcu_frame_len_and_send(&frame_tmp);
        // send_len = construct_protocol_frame(s_send_handle.send_buf, frame->body.header.prm_cmd, NORMAL_DATA_DIR_TERM_TO_PLT);
        // if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
        break;
    /* 设置主动上报周期 */
    case PRM_CMD_PLT_EN_REALTIME_REPORT:
        g_profile.active_report_period = big_endian_read_64(frame->body.body.business_data);
        copy_and_modify_time_dir(&frame_tmp, frame, NORMAL_DATA_DIR_TERM_TO_PLT);
        calcu_frame_len_and_send(&frame_tmp);
        printf("g_profile.active_report_period:%d\r\n", g_profile.active_report_period);
        // construct_protocol_frame(buffer, frame->body.header.prm_cmd, NORMAL_DATA_DIR_TERM_TO_PLT);
        break;
    case PRM_CMD_DEV_HEART_REPORT:
        if (frame->body.body.business_data[0] == 0x01)
        {
            // clear heart counter
        }
        break;
    case PRM_CMD_PLT_SET_PLAN_TASK:
        break;
    case PRM_CMD_PLT_QUERY_PLAN_TASK:
        break;
    case PRM_CMD_PLT_REMOTE_CONTROL:

        if (frame->body.body.business_data[0] == 0x00) //灯头标识
        {
            switch (frame->body.body.sub_cmd)
            {
            case SUB_CMD_TURN_LAMP_ON:
            case SUB_CMD_DIMMING:
                // adjust light
                // frame->body.body.business_data[1]
                break;
            case SUB_CMD_TURN_LAMP_OFF:
                if (frame->body.body.business_data[1] == 0)
                {
                }
                break;
            }
        }
        break;
    case PRM_CMD_PLT_QUERY_LATEST_DATA:
        break;
    case PRM_CMD_PLT_SET_RTU_UPGRADE:
        break;
    case PRM_CMD_PLT_SET_TERM_UPGRADE:
        break;
    case PRM_CMD_PLT_SET_ALARM_LIMIT:
        break;
    case PRM_CMD_PLT_SET_KEY_PAIR:
        break;
    case PRM_CMD_PLT_SET_ENCRYPT_TEXT:
        break;
    case PRM_CMD_PLT_EN_ENCRYPTION:
        break;
    default:
        break;
    }
    return 1;
}

uint8_t release_detect_to_platform_ack_frame(const protocol_frame_t *frame)
{
    uint8_t rtvalue = 1;
    switch (frame->body.header.prm_cmd)
    {
    case PRM_CMD_DEV_REQUSET_LOGIN:
        printf("client recv login ack\r\n");
        break;
    case PRM_CMD_DEV_HEART_REPORT:
        printf("client recv heart-report ack\r\n");
        break;
    case PRM_CMD_DEV_CTRL_RESULT_REPORT:
        break;
    case PRM_CMD_DEV_REALTIME_DATA_REPORT:
        printf("client recv reltime-data-report ack\r\n");
        break;
    default:
        rtvalue = 0;
        break;
    }
    return rtvalue;
}

static uint8_t client_recv_logic_proc(const protocol_frame_t *frame)
{
    uint8_t rtvalue = 0;
    if (protocol_frame_profile_compare(frame))
    {
        /* recv platform request type frame */
        if (frame->body.header.data_dir == NORMAL_DATA_DIR_PLT_TO_TERM)
        {
            rtvalue = send_response_to_platform_request_frame(frame);
        }
        /* recv platform ack type frame */
        else if (frame->body.header.data_dir == ACK_DATA_DIR_PLT_TO_TERM)
        {
            rtvalue = release_detect_to_platform_ack_frame(frame);
        }
    }
    return rtvalue;
}

/*-构建发送帧((填充profile->填充业务数据->序列化))------------------------------------------------------*/
static int32_t construct_protocol_frame(uint8_t *data, uint8_t prmcmd, uint8_t cmddir)
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
    case PRM_CMD_PLT_RESTART_DEVICE:
        s_send_handle.send_buf[FRAME_BUSS_DATA_POS] = 0x01;
        p_frame->body.body.business_data            = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        p_frame->body.header.business_data_len      = 1;
        break;
    case PRM_CMD_DEV_HEART_REPORT:
        // no business data
        break;
    case PRM_CMD_DEV_REALTIME_DATA_REPORT:
        p_frame->body.body.is_has_sub_cmd            = 1;
        p_frame->body.body.sub_cmd                   = SUB_CMD_SINGLE_REPORT;
        s_send_handle.send_buf[FRAME_BUSS_DATA_POS2] = 0x01;                        //灯数量
        get_device_address_flow(s_send_handle.send_buf + FRAME_BUSS_DATA_POS2 + 1); //灯6位编号地址
        s_send_handle.send_buf[FRAME_BUSS_DATA_POS2 + 7] = 0x00;                    //灯头标识
        get_realtime_data_flow(s_send_handle.send_buf + MUX_REPORT_DATA_POS2 + 8);
        p_frame->body.body.business_data       = s_send_handle.send_buf + FRAME_BUSS_DATA_POS2;
        p_frame->body.header.business_data_len = 8 + sizeof(realtime_data_t);
        break;
    case PRM_CMD_PLT_SYNC_TIME:
        get_device_time_flow(s_send_handle.send_buf + FRAME_BUSS_DATA_POS);
        p_frame->body.body.business_data       = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        p_frame->body.header.business_data_len = 8;
        break;
    case PRM_CMD_DEV_ALARM_REPORT:
        s_send_handle.send_buf[FRAME_BUSS_DATA_POS] = 0x01;                        //灯数量
        get_device_address_flow(s_send_handle.send_buf + FRAME_BUSS_DATA_POS + 1); //灯6位编号地址
        s_send_handle.send_buf[FRAME_BUSS_DATA_POS + 7] = 0x00;                    //灯头标识
        get_device_alarm_data_flow(s_send_handle.send_buf + MUX_REPORT_DATA_POS + 8);
        p_frame->body.body.business_data       = s_send_handle.send_buf + FRAME_BUSS_DATA_POS;
        p_frame->body.header.business_data_len = 8 + sizeof(alarm_data_t);
        break;
    // case PRM_CMD_PLT_EN_REALTIME_REPORT:
    //     memset((void *)&buffer, big_endian_read_32((uint8_t *)g_profile.active_report_period), 4);
    //     p_frame->body.body.business_data       = buffer;
    //     p_frame->body.header.business_data_len = 4;
    //     break;
    // case PRM_CMD_PLT_REMOTE_CONTROL:
    //     if (prmcmd < SUB_CMD_CONTROL_MAX)
    //     {
    //         p_frame->body.body.is_has_sub_cmd = 1;
    //     } else
    //     {
    //         param_error = 1;
    //     }
    //     break;
    default:
        break;
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

void send_login_frame(void)
{
    int32_t send_len = TERM_CONSTRUCT_FRAME(PRM_CMD_DEV_REQUSET_LOGIN);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}
void send_heart_frame(void)
{
    int32_t send_len = TERM_CONSTRUCT_FRAME(PRM_CMD_DEV_HEART_REPORT);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}
void send_reltime_data_frame(void)
{
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_DEV_REALTIME_DATA_REPORT, NORMAL_DATA_DIR_TERM_TO_PLT);
    // int32_t send_len = TERM_CONSTRUCT_FRAME(PRM_CMD_DEV_REALTIME_DATA_REPORT);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}
void send_alarm_frame(void)
{
    int32_t send_len = TERM_CONSTRUCT_FRAME(PRM_CMD_DEV_ALARM_REPORT);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}
/*-----------------------------------------------------------------------------*/
void bind_client_send_handle(uint8_t *buf, uint8_t (*send_func)(uint8_t *buf, uint16_t len))
{
    s_send_handle.send_func = send_func;
    s_send_handle.send_buf  = buf;
}

void on_client_recv_data(uint8_t *buf, uint16_t len)
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
            client_recv_logic_proc(&frame);
        } else
        {
            /* 不完整或者找不到 */
            break;
        }
    }
}