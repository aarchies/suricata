#include "socket_client.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define PACKET_DELIMITER "\n"
#define DELIMITER_SIZE 1
static SOCKET global_sock;

void socket_client_init(void)
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

SOCKET get_global_sock(void)
{
    if (global_sock == NULL)
    {
        global_sock = socket_client_create("192.168.100.199", 50051);
    }

    return global_sock;
}

SOCKET socket_client_create(const char *ip, int port)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    return sock;
}

void socket_client_send(SOCKET sock, const uint8_t *data, size_t length)
{
    send(sock, (char *)data, length, 0);
}

void socket_client_send_packet(SOCKET sock, const PacketWrapper *packet)
{
    send(sock, (char *)packet, sizeof(PacketWrapper), 0);
}

void socket_client_cleanup()
{
#ifdef _WIN32
    closesocket(global_sock);
    WSACleanup();
#else
    close(global_sock);
#endif
}