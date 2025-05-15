/**
 * @file udp_send.h
 * @author Steins_Xin
 * @brief UDP 数据发送模块头文件
 * @version 1.0
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MAIN_UDP_SEND_H
#define MAIN_UDP_SEND_H

#include <stdint.h>

/**
 * @brief 初始化 UDP 目标地址和端口，创建 socket
 *
 * @param ip   目标 IP 地址
 * @param port 目标端口
 * @return     0=成功, -1=失败
 */
int udp_send_init(const char *ip, uint16_t port);

/**
 * @brief 发送 UDP 数据
 *
 * @param message 要发送的字符串
 * @return        发送成功字节数，或 -1 失败
 */
int udp_send(const char *message);

/**
 * @brief 关闭 UDP socket
 */
void udp_send_close(void);


#endif //MAIN_UDP_SEND_H
