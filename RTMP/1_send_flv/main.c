#include <stdio.h>

#include "librtmp/rtmp.h"

void test_rtmp();

int main() {
    test_rtmp();
    return 0;
}

void test_rtmp() {
    puts("test rtmp.");
    RTMP *rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    if (rtmp == NULL) {
        puts("rtmp 初始化失败.");
        return;
    }

}
