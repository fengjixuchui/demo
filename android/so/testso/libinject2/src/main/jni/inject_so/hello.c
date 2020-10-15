/*
 * hello.c
 *
 *  Created on: 2015年6月26日
 *      Author: Administrator
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <elf.h>
#include <fcntl.h>

#define LOG_TAG "DEBUG"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

int hook_entry(char * a){
    LOGD("Hook success, pid = %d\n", getpid());
    LOGD("Hello %s\n", a);
    printf("I Have Inject so success\n");
    fflush(stdout);
    return 0;
}
