#include <stdio.h>

#include "librtmp/rtmp.h"

void test_rtmp();

int main() {
    test_rtmp();
    return 0;
}

void test_rtmp() {
    puts("test rtmp.");
    RTMP *rtmp = NULL;
    RTMP_Init(rtmp);

}
