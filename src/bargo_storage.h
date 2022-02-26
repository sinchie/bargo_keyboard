#ifndef BARGO_STORAGE_H
#define BARGO_STORAGE_H

#include "sdk_errors.h"

// 存储初始化
void bargo_storage_init();

// fds错误码转换文字
const char *fds_err_str(ret_code_t ret);


#endif