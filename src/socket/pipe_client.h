#ifndef PIPE_CLIENT_H
#define PIPE_CLIENT_H

#include <stdint.h>

// 初始化管道
int pipe_client_init(void);

// 发送数据包
int pipe_client_send_packet(const uint8_t *data, uint32_t length, const uint8_t *tid, uint8_t direction);

// 清理管道
void pipe_client_cleanup(void);

#endif