
#include <string.h>
#include "protocol_proc_abstruct.h"
#include "protocol_server_proc.h"
#include "mini_utils.h"
#include <stdio.h>

#define _NULL_ (0)
static send_handle_t s_send_handle;

static int32_t construct_protocol_frame(uint8_t *data, uint8_t prmcmd, uint8_t cmddir, uint8_t subcmd);

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

/*-Server 接收处理--------------------------------------------------------------*/
static uint8_t protocol_frame_recv_proc(const protocol_frame_t *frame)
{
    protocol_frame_t frame_tmp;
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
                copy_and_modify_time_dir(&frame_tmp, frame, ACK_DATA_DIR_PLT_TO_TERM);
                calcu_frame_len_and_send(&frame_tmp);
                break;
            case PRM_CMD_DEV_HEART_REPORT:
                send_len = construct_protocol_frame(s_send_handle.send_buf, frame->body.header.prm_cmd, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
                if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
                break;
            case PRM_CMD_DEV_CTRL_RESULT_REPORT:
                break;
            case PRM_CMD_DEV_REALTIME_DATA_REPORT:
                break;
            case PRM_CMD_PLT_SET_TERM_UPGRADE:
                copy_and_modify_time_dir(&frame_tmp, frame, NORMAL_DATA_DIR_PLT_TO_TERM);
                if (frame_tmp.body.body.biz_data_ptr[2] == 0x03)
                {
                    update_data_t update;
                    update.version    = 0x8888;
                    update.cmd        = 0x04;
                    update.file_size  = 0x10;
                    update.packet_num = 0x02;
                    update.idx        = 0;
                    update.len        = 0x0a;
                    update.data       = s_send_handle.send_buf + BIZ_DATA_POS + 9;
                    fill_update_data(s_send_handle.send_buf + BIZ_DATA_POS, &update);
                    frame_tmp.body.body.biz_data_ptr   = s_send_handle.send_buf + BIZ_DATA_POS;
                    frame_tmp.body.header.biz_data_len = 9 + update.len;
                    calcu_frame_len_and_send(&frame_tmp);

                    update.version    = 0x8888;
                    update.cmd        = 0x04;
                    update.file_size  = 0x10;
                    update.packet_num = 0x02;
                    update.idx        = 1;
                    update.len        = 0x06;
                    update.data       = s_send_handle.send_buf + BIZ_DATA_POS + 9;
                    fill_update_data(s_send_handle.send_buf+ BIZ_DATA_POS, &update);
                    frame_tmp.body.body.biz_data_ptr   = s_send_handle.send_buf + BIZ_DATA_POS;
                    frame_tmp.body.header.biz_data_len = 9 + update.len;
                    calcu_frame_len_and_send(&frame_tmp);
                }
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
    p_frame->body.body.is_has_sub_cmd = 0;
    p_frame->body.body.biz_data_ptr   = _NULL_;
    p_frame->body.header.biz_data_len = 0;
    /* set specific value according to different cmd */
    switch (prmcmd)
    {
    case PRM_CMD_PLT_RESTART_DEVICE:
        // no business data
        break;
    case PRM_CMD_PLT_SYNC_TIME:
        // todo: fill sys time
        get_device_time_flow(s_send_handle.send_buf + BIZ_DATA_POS);
        p_frame->body.body.biz_data_ptr   = s_send_handle.send_buf + BIZ_DATA_POS;
        p_frame->body.header.biz_data_len = 8;
        break;
    case PRM_CMD_PLT_SET_REALTIME_REPORT:
        g_profile.active_report_period = 100;
        big_endian_store_32(s_send_handle.send_buf + BIZ_DATA_POS, g_profile.active_report_period);
        p_frame->body.body.biz_data_ptr   = s_send_handle.send_buf + BIZ_DATA_POS;
        p_frame->body.header.biz_data_len = 4;
        break;
    case PRM_CMD_DEV_HEART_REPORT:
        s_send_handle.send_buf[BIZ_DATA_POS] = 0x01;
        p_frame->body.body.biz_data_ptr      = s_send_handle.send_buf + BIZ_DATA_POS;
        p_frame->body.header.biz_data_len    = 1;
        break;
    case PRM_CMD_PLT_QUERY_LATEST_DATA:
        // no business data
        break;
    case PRM_CMD_PLT_REMOTE_CONTROL:
        s_send_handle.send_buf[BIZ_DATA_POS2] = 0x00; //灯头标识
        p_frame->body.body.is_has_sub_cmd     = 1;
        p_frame->body.body.sub_cmd            = subcmd;
        switch (subcmd)
        {
        case SUB_CMD_TURN_LAMP_ON:
            s_send_handle.send_buf[BIZ_DATA_POS2 + 1] = 100;
            break;
        case SUB_CMD_TURN_LAMP_OFF:
            s_send_handle.send_buf[BIZ_DATA_POS2 + 1] = 0;
            break;
        case SUB_CMD_DIMMING:
            s_send_handle.send_buf[BIZ_DATA_POS2 + 1] = 50;
            break;
        }
        p_frame->body.body.biz_data_ptr   = s_send_handle.send_buf + BIZ_DATA_POS2;
        p_frame->body.header.biz_data_len = 2;
        break;
    case PRM_CMD_PLT_SET_TERM_UPGRADE:
        s_send_handle.send_buf[BIZ_DATA_POS]     = 0x66;
        s_send_handle.send_buf[BIZ_DATA_POS + 1] = 0x77;
        p_frame->body.body.biz_data_ptr          = s_send_handle.send_buf + BIZ_DATA_POS;
        p_frame->body.header.biz_data_len        = 2;
        break;
    case PRM_CMD_PLT_SET_PLAN_TASK:
        plan_task_manage_t plan_tasks;
        if (g_profile.get_plan_task_data_fptr) g_profile.get_plan_task_data_fptr(&plan_tasks);
        p_frame->body.body.biz_data_ptr   = s_send_handle.send_buf + BIZ_DATA_POS;
        p_frame->body.header.biz_data_len = fill_plan_task_stream(s_send_handle.send_buf + BIZ_DATA_POS, &plan_tasks);

        break;
    case PRM_CMD_PLT_QUERY_PLAN_TASK:
        // no business data
        break;
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
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_SET_REALTIME_REPORT, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_query_frame(void)
{
    int32_t send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_QUERY_LATEST_DATA, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_lamp_dimm_frame(void)
{
    int32_t send_len;

    send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_REMOTE_CONTROL, NORMAL_DATA_DIR_PLT_TO_TERM, SUB_CMD_TURN_LAMP_ON);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);

    // send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_REMOTE_CONTROL, NORMAL_DATA_DIR_PLT_TO_TERM, SUB_CMD_TURN_LAMP_OFF);
    // if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);

    // send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_REMOTE_CONTROL, NORMAL_DATA_DIR_PLT_TO_TERM, SUB_CMD_DIMMING);
    // if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_update_frame(void)
{
    int32_t send_len;
    send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_SET_TERM_UPGRADE, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_query_latest_data_frame(void)
{
    int32_t send_len;
    send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_QUERY_LATEST_DATA, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_set_term_plan_task(void)
{
    int32_t send_len;
    send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_SET_PLAN_TASK, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
}

void send_query_term_plan_task(void)
{
    int32_t send_len;
    send_len = construct_protocol_frame(s_send_handle.send_buf, PRM_CMD_PLT_QUERY_PLAN_TASK, NORMAL_DATA_DIR_PLT_TO_TERM, _NULL_);
    if (send_len > 0) s_send_handle.send_func(s_send_handle.send_buf, send_len);
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