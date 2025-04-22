#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#define SOCKET int
#endif

typedef struct
{
    uint8_t data[2048];  // 数据
    uint8_t line_no[51]; // 51字节
    uint8_t direction;   // 1字节 方向
    uint8_t is_alert;    // 1字节 是否告警
    uint32_t sid;        // 4字节 签名ID
    uint32_t class_type; // 4字节 分类类型
    uint8_t emsg[128];   // 128字节 告警消息
    uint8_t delimiter;   // 1字节 分隔符
} PacketWrapper;

// 初始化套接字
void socket_client_init(void);

// 获取全局套接字
SOCKET get_global_sock(void);

// 创建套接字连接
SOCKET socket_client_create(const char *ip, int port);

// 发送数据
void socket_client_send(SOCKET sock, const uint8_t *data, size_t length);

// 清理套接字
void socket_client_cleanup();

// 修改发送函数声明
void socket_client_send_packet(SOCKET sock, const PacketWrapper *packet);

#endif