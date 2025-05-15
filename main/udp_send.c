/**
 * @file udp_send.c
 * @author Steins_Xin
 * @brief UDP 数据发送实现
 * @version 1.0
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "udp_send.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int udp_sock = -1;
static struct sockaddr_in udp_target_addr;

/**
 * @brief 初始化 UDP 目标地址和端口，创建 socket
 *
 * @param ip   目标 IP 地址
 * @param port 目标端口
 * @return     0=成功, -1=失败
 */
int udp_send_init(const char *ip, uint16_t port) {
    udp_send_close();  // 如果之前有打开的 socket，先关闭

    udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_sock < 0) {
        perror("UDP socket creation failed");
        return -1;
    }

    memset(&udp_target_addr, 0, sizeof(udp_target_addr));
    udp_target_addr.sin_family = AF_INET;
    udp_target_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &udp_target_addr.sin_addr) <= 0) {
        perror("Invalid UDP target IP");
        close(udp_sock);
        udp_sock = -1;
        return -1;
    }

    return 0;
}

/**
 * @brief 发送 UDP 数据
 *
 * @param message 要发送的字符串
 * @return        发送成功字节数，或 -1 失败
 */
int udp_send(const char *message) {
    if (udp_sock < 0) {
        printf("UDP socket not initialized\n");
        return -1;
    }

    int sent = sendto(udp_sock, message, strlen(message), 0,
                      (struct sockaddr *)&udp_target_addr, sizeof(udp_target_addr));
    if (sent < 0) {
        perror("UDP send failed");
    }

    return sent;
}

/**
 * @brief 关闭 UDP socket
 */
void udp_send_close(void) {
    if (udp_sock >= 0) {
        close(udp_sock);
        udp_sock = -1;
    }
}