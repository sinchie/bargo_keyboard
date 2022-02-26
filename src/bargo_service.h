#ifndef BARGO_SERVICE_H
#define BARGO_SERVICE_H

#include "app_timer.h"

// 初始化日志
void bargo_log_init(void);

// 初始化定时器
void bargo_timers_init(void);

// 初始化电源管理
void bargo_power_management_init(void);


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
void bargo_idle_state_handle(void);

#endif