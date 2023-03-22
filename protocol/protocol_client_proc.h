
#ifndef __PROTOCOL_CLIENT_H__
#define __PROTOCOL_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "protocol.h"
#include <stdint.h>

int32_t construct_login_frame(uint8_t *data);

void bind_client_send_handle(uint8_t* buf, uint8_t (*send_func)(uint8_t* buf, uint16_t len));
void on_client_recv_data(uint8_t* buf, uint16_t len);
void send_login_frame(void);
void send_heart_frame(void);
void send_reltime_data_frame(void);
void send_alarm_frame(void);

#ifdef __cplusplus  
}
#endif /* __cplusplus */
#endif /* __PROTOCOL_CLIENT_H__ */