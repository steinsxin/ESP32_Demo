//
// Created by Steins_Xin on 25-5-11.
//

#ifndef MAIN_NTP_TIME_H
#define MAIN_NTP_TIME_H

#include "esp_sntp.h"

void sntp_task(void *param);
struct tm* get_time(void);

#endif //MAIN_NTP_TIME_H
