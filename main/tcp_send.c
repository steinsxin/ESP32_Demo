/**
 * @file tcp_send.c
 * @author Steins_Xin
 * @brief TCP 数据发送实现
 * @version 1.0
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "tcp_send.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int tcp_sock = -1;

/**
 * @brief 初始化 TCP 连接（目标地址+端口），并建立连接
 *
 * @param ip   目标 IP 地址
 * @param port 目标端口
 * @return     0=成功, -1=失败
 */
int tcp_send_init(const char *ip, uint16_t port) {
    tcp_send_close();  // 若已有 socket 先关闭

    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        perror("TCP socket creation failed");
        return -1;
    }

    struct sockaddr_in target_addr = {0};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &target_addr.sin_addr) <= 0) {
        perror("Invalid TCP target IP");
        close(tcp_sock);
        tcp_sock = -1;
        return -1;
    }

    if (connect(tcp_sock, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0) {
        perror("TCP connect failed");
        close(tcp_sock);
        tcp_sock = -1;
        return -1;
    }

    return 0;
}

/**
 * @brief 发送 TCP 数据（使用已连接的 socket）
 *
 * @param message 要发送的字符串
 * @return        发送成功字节数，或 -1 失败
 */
int tcp_send(const char *message) {
    if (tcp_sock < 0) {
        printf("TCP socket not initialized\n");
        return -1;
    }

    int sent = send(tcp_sock, message, strlen(message), 0);
    if (sent < 0) {
        perror("TCP send failed");
    }

    return sent;
}

/**
 * @brief 关闭 TCP 连接
 */
void tcp_send_close(void) {
    if (tcp_sock >= 0) {
        close(tcp_sock);
        tcp_sock = -1;
    }
}