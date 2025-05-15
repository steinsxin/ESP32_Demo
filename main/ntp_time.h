/**
 * @file ntp_time.h
 * @author Steins_Xin
 * @brief NTP 时间同步模块头文件
 * @version 1.0
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MAIN_NTP_TIME_H
#define MAIN_NTP_TIME_H


#include <time.h>

/**
 * @brief 初始化 SNTP 服务
 */
void start_sntp(void);

/**
 * @brief 获取当前本地时间
 * @return struct tm 当前时间（本地时区）
 */
struct tm get_current_time(void);

#endif //MAIN_NTP_TIME_H
