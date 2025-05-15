/**
 * @file tcp_send.h
 * @author Steins_Xin
 * @brief TCP 数据发送模块头文件
 * @version 1.0
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MAIN_TCP_SEND_H
#define MAIN_TCP_SEND_H

#include <stdint.h>

/**
 * @brief 初始化TCP连接（目标地址+端口），并建立连接
 *
 * @param ip   目标IP地址
 * @param port 目标端口
 * @return     0=成功, -1=失败
 */
int tcp_send_init(const char *ip, uint16_t port);

/**
 * @brief 发送TCP数据（使用已连接的socket）
 *
 * @param message 要发送的字符串
 * @return        发送成功字节数，或 -1 失败
 */
int tcp_send(const char *message);

/**
 * @brief 关闭TCP连接
 */
void tcp_send_close(void);

#endif //MAIN_TCP_SEND_H
