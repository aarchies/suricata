#include "pipe_client.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PIPE_NAME "/tmp/suricata_pipe"
static int pipe_fd = -1;

int pipe_client_init(void)
{
    // 创建命名管道
    mkfifo(PIPE_NAME, 0666);

    // 打开管道
    pipe_fd = open(PIPE_NAME, O_WRONLY);
    if (pipe_fd < 0)
    {
        printf("Failed to open pipe\n");
        return -1;
    }
    return 0;
}

int pipe_client_send_packet(const uint8_t *data, uint32_t length, const uint8_t *tid, uint8_t direction)
{
    // 写入长度
    if (write(pipe_fd, &length, sizeof(length)) < 0)
    {
        return -1;
    }

    // 写入数据
    if (data && length > 0)
    {
        if (write(pipe_fd, data, length) < 0)
        {
            return -1;
        }
    }

    // 写入TID
    if (write(pipe_fd, tid, 32) < 0)
    {
        return -1;
    }

    // 写入方向
    if (write(pipe_fd, &direction, sizeof(direction)) < 0)
    {
        return -1;
    }

    return 0;
}

void pipe_client_cleanup(void)
{
    if (pipe_fd >= 0)
    {
        close(pipe_fd);
        pipe_fd = -1;
    }
    unlink(PIPE_NAME);
}