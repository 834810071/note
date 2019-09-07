UNIX 网络编程卷1：套接字联网API
# 第1章 简介
## 1.1 概述
>>传输控制协议（英语：Transmission Control Protocol，缩写：TCP）  用户数据报协议（英语：User Datagram Protocol，縮寫：UDP；又稱用户数据包协议)   IP(Internet Protocol, 网际协议)

## 1.10 Unix标准
>> POSIX(可移植操作系统接口，Portable Operating System Interface)    

# 第2章 传输层：TCP、UDP和SCTP
>>SCTP（Stream Control Transmission Protocol，流控制传输协议）
## 2.3 用户数据报协议(UDP)
>>应用进程往一个UDP套接字写入一个消息，该消息随后被封装到一个UDP数据包，该UDP数据报进而又被封装到一个IP数据报，然后发送到目的地。UDP不保证UDP数据报回到达最终目的地，不保证各个数据报的先后顺序跨网络后保持不变，也不保证每个数据报只到达一次。

>> 如果想要确保一个数据报到达其目的地，可以往应用程序中添置一大堆特性：来自对端的确认、本端的超时与重传等。

>> 每个UDP数据报都有一个长度。如果一个数据报正确地到达其目的地，那么该数据报的长度将随数据一道传给应用进程。TCP是一个字节流(byte-stream)协议，没有任何记录边界。

## 2.4 传输控制协议(TCP   )
