### 研究rtmp 


### 参考
* [librtmp-example](https://github.com/Akagi201/librtmp-example/blob/master/send_flv_over_rtmp.c)
* [Publishing a stream using librtmp in C/C++](https://stackoverflow.com/questions/4524587/publishing-a-stream-using-librtmp-in-c-c/13803143#13803143)

```c
//Code
//Init RTMP code
RTMP *r;
char uri[]="rtmp://localhost:1935/live/desktop";
r= RTMP_Alloc();
RTMP_Init(r);
RTMP_SetupURL(r, (char*)uri);
RTMP_EnableWrite(r);
RTMP_Connect(r, NULL);
RTMP_ConnectStream(r, 0);

```
* [【流媒体协议】图解 FLV 协议 快速入门](https://zhuanlan.zhihu.com/p/495705490)
