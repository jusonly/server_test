
#ifndef __PROTOCOL_SERVER_H__
#define __PROTOCOL_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol.h"
#include "protocol_proc_abstruct.h"
#include <stdint.h>

void bind_server_send_handle(uint8_t *buf, uint8_t (*send_func)(uint8_t *buf, uint16_t len));
void on_server_recv_data(uint8_t *buf, uint16_t len);

int32_t construct_restart_frame(uint8_t *data);
int32_t construct_sync_time_frame(uint8_t *data);

void send_restart_frame(void);
void send_sync_time_frame(void);
void send_lamp_dimm_frame(int level);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __PROTOCOL_SERVER_H__ */