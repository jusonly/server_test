#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <mutex>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "protocol/protocol_proc_abstruct.h"
#include "protocol/protocol_server_proc.h"
#include "protocol/protocol_client_proc.h"

uint8_t client_send_buffer[1024];
uint8_t client_recv_buffer[1024];

uint8_t server_send_buffer[1024];
uint8_t server_recv_buffer[1024];

int server_conn_fd;
int client_conn_fd;
int listenfd;

std::mutex keyboard_mutex;

char input_buf[255];
// std::this_thread::sleep_for(std::chrono::seconds(1));

void input_buf_clear(void)
{
    // keyboard_mutex.lock();
    memset(input_buf, 0, sizeof(input_buf));
    // keyboard_mutex.unlock();
}

void client_thread_entry()
{
    std::cout << "Client Thread Running..." << std::endl;
    /* socket */
    client_conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_conn_fd == -1)
    {
        std::cout << "Error: client create socket." << std::endl;
        return;
    }
    /* connect */
    struct sockaddr_in serverAddr;
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_port        = htons(8000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client_conn_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Error: client connect to server." << std::endl;
        return;
    }
    std::cout << "Info: client connect to server success." << std::endl;
    /*-------------------------------------------------------------*/
    fd_set client_fd_set;
    struct timeval tv;
    while (true)
    {
        tv.tv_sec  = 1; /* 设置检测超时 */
        tv.tv_usec = 0;
        FD_ZERO(&client_fd_set); /* 清空集合 */
        // FD_SET(STDIN_FILENO, &client_fd_set);                        /* 加入STDIN_FILENO 至集合 */
        FD_SET(client_conn_fd, &client_fd_set);                      /* 加入client_conn_fd 至集合*/
        select(client_conn_fd + 1, &client_fd_set, NULL, NULL, &tv); /* 检测集合中是否有数据(可读) */

        /* 判断具体的fd */
        // if (FD_ISSET(STDIN_FILENO, &client_fd_set))
        // {
        //     char tmp[255];
        //     memset(tmp, 0, sizeof(tmp));
        //     fgets(tmp, sizeof(tmp), stdin); /* STDIN_FILENO end with \n */
        //     if (strcmp(tmp, "login\n") == 0);
        // }
        if (FD_ISSET(client_conn_fd, &client_fd_set))
        {
            // client recv
            memset((void *)client_recv_buffer, 0, sizeof(client_recv_buffer));
            int recv_byte_len = recv(client_conn_fd, client_recv_buffer, sizeof(client_recv_buffer), 0);
            if (recv_byte_len > 0)
            {
                printf("client recv data:");
                for (int i = 0; i < recv_byte_len; i++)
                {
                    printf("%02x", client_recv_buffer[i]);
                }
                printf("\r\n");
                // client recv process
                on_client_recv_data(client_recv_buffer, recv_byte_len);
            } else if (recv_byte_len < 0)
            {
                std::cout << "Error: client recv data." << std::endl;
            } else
            {
                std::cout << "Error: server closed." << std::endl;
                close(client_conn_fd);
            }
        }
        /* input detect */
        if (strcmp(input_buf, "login") == 0)
        {
            input_buf_clear();
            send_login_frame();
        } else if (strcmp(input_buf, "realtime") == 0)
        {
            input_buf_clear();
            send_reltime_data_frame();
        } else if (strcmp(input_buf, "alarm") == 0)
        {
            input_buf_clear();
            send_alarm_frame();
        } else if (strcmp(input_buf, "cexit") == 0)
        {
            input_buf_clear();
            std::cout << "client disconnect" << std::endl;
            close(client_conn_fd);
            break;
        }
    }
    return;
}

void server_thread_entry()
{
    std::cout << "Server Thread Running..." << std::endl;
    // socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "Error: socket" << std::endl;
        return;
    }
    // bind
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cout << "Error: bind" << std::endl;
        return;
    }
    // listen
    if (listen(listenfd, 5) == -1)
    {
        std::cout << "Error: listen" << std::endl;
        return;
    }
    std::cout << "listen to clients success." << std::endl;
    // accept

    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (true)
    {
        std::cout << "server accepting..." << std::endl;
        server_conn_fd = accept(listenfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (server_conn_fd < 0)
        {
            std::cout << "Error: accept" << std::endl;
            continue;
        }
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "accepted from " << clientIP << ":" << ntohs(clientAddr.sin_port)
                  << std::endl;

        fd_set server_fd_set;
        struct timeval tv;
        while (true)
        {
            tv.tv_sec  = 1; /* 设置检测超时 */
            tv.tv_usec = 0;
            FD_ZERO(&server_fd_set); /* 清空集合 */
            // FD_SET(STDIN_FILENO, &server_fd_set);                        /* 加入STDIN_FILENO 至集合 */
            FD_SET(server_conn_fd, &server_fd_set);                      /* 加入server_conn_fd 至集合*/
            select(server_conn_fd + 1, &server_fd_set, NULL, NULL, &tv); /* 检测集合中是否有数据(可读) */

            /* 判断具体的fd */
            // if (FD_ISSET(STDIN_FILENO, &server_fd_set))
            // {
            //     char tmp[255];
            //     memset(tmp, 0, sizeof(tmp));
            //     fgets(tmp, sizeof(tmp), stdin); /* STDIN_FILENO end with \n */
            //     if (strcmp(tmp, "server exit\n") == 0);
            // }
            if (FD_ISSET(server_conn_fd, &server_fd_set))
            {
                // server recv
                memset(server_recv_buffer, 0, sizeof(server_recv_buffer));
                int32_t server_recv_len = recv(server_conn_fd, server_recv_buffer, sizeof(server_recv_buffer), 0);
                printf("server recv data:");
                for (int32_t i = 0; i < server_recv_len; i++)
                {
                    printf("%02x", server_recv_buffer[i]);
                }
                printf("\r\n");
                // server recv process
                on_server_recv_data(server_recv_buffer, server_recv_len);
            }
            /* input detect */
            if (strcmp(input_buf, "restart") == 0)
            {
                input_buf_clear();
                send_restart_frame();
            } else if (strcmp(input_buf, "sync") == 0)
            {
                input_buf_clear();
                send_sync_time_frame();
            } else if (strcmp(input_buf, "sexit") == 0)
            {
                input_buf_clear();
                std::cout << "...disconnect " << clientIP << ":"
                          << ntohs(clientAddr.sin_port) << std::endl;
                close(server_conn_fd);
                break;
            }
        }
        break;
    }
    close(listenfd);
}

// TODO: uint64_t 时间戳
void port_get_time(uint64_t *time)
{
    *time = 20220101095959012;
}

void protocol_init()
{
    protocol_frame_profile_t client_profile;
    memset(&client_profile, 0, sizeof(protocol_frame_profile_t));
    client_profile.area_id[0]         = 0x00;
    client_profile.area_id[1]         = 0x01;
    client_profile.factory_id[0]      = 0x00;
    client_profile.factory_id[1]      = 0x01;
    client_profile.device_type_id[0]  = 0x00;
    client_profile.device_type_id[1]  = 0x01;
    client_profile.address[0]         = 0x20;
    client_profile.address[1]         = 0x23;
    client_profile.address[2]         = 0x01;
    client_profile.address[3]         = 0x01;
    client_profile.address[4]         = 0x00;
    client_profile.address[5]         = 0x01;
    client_profile.logic_protocol_ver = CFG_PROTOCOL_VERSION;
    client_profile.device_type        = CFG_PROTOCOL_DEVICE_TYPE;
    client_profile.get_time_fptr      = port_get_time;
    set_protocol_frame_profile(client_profile);
}

uint8_t client_send_func_port(uint8_t *buf, uint16_t len)
{
    printf("client send data:");
    for (int32_t i = 0; i < len; i++)
    {
        printf("%02x", buf[i]);
    }
    printf("\n");
    if (send(client_conn_fd, buf, len, 0) < 0)
    {
        printf("client send error: %s(errno:%d)\n", strerror(errno), errno);
    }
    return 1;
}

uint8_t server_send_func_port(uint8_t *buf, uint16_t len)
{
    printf("server send data:");
    for (int32_t i = 0; i < len; i++)
    {
        printf("%02x", buf[i]);
    }
    printf("\n");
    if (send(server_conn_fd, buf, len, 0) < 0)
    {
        printf("server send error: %s(errno:%d)\n", strerror(errno), errno);
    }
    return 1;
}

void keyboard_entry(void)
{
    while (1)
    {
        std::cin >> input_buf;
    }
}

int main()
{
    protocol_init();
    bind_server_send_handle(server_send_buffer, server_send_func_port);
    bind_client_send_handle(client_send_buffer, client_send_func_port);

    std::thread server_thread(server_thread_entry);
    std::thread client_thread(client_thread_entry);
    std::thread keyboard_thread(keyboard_entry);

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
