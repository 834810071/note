[**UNIX 网络编程卷1：套接字联网API**](https://github.com/834810071/note/blob/master/UNIXNetworkProgramming/README.md)
=================================
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

## 2.4 传输控制协议(TCP)
>> 首先，TCP提供客户与服务器之间的连接(connection)。TCP客户先与某个给定服务器建立一个连接，再跨该连接与那个服务器交换数据，然后终止这个连接。

>> 可靠性(reliability):当TCP向另一端发送数据时，它要求对端返回一个确认。如果没有收到确认，TCP就自动重传数据并等待更长时间。在数次重传失败后，TCP才放弃。

>> TCP含有用于动态估算客户与服务器之间的往返时间(round-trip, RTT)的算法，以便让它知道等待一个确认需要多少时间。

>> TCP通过给其中每个字节关联一个序列号对所发送的数据进行排序(sequencing)。

>> TCP提供流量控制(flow control)。TCP总是告知对端在任何时刻它一次能够从对端接收多少字节数据，这称为通告窗口(advertised window)。

>> TCP是全双工的(full-duplex)。 [UDP可以是全双工]

## 2.5 流控制传输协议(SCTP)
>> SCTP在客户与服务器之间提供关联(association)，并像TCP那样给应用提供可靠性、排序、流量控制以及全双工数据传送。  

>> SCTP是面向消息的[message-oriented]。它提供了各个记录的按序递送服务。与UDP一样，由发送端写入每条记录的长度随数据一道传递给接收端应用。
   
>> SCTP能够在所连接的端点之间提供多个流，每个流各自可靠地按序递送消息。一个流上某个消息的丢失不会阻塞同一关联其他流上消息的投递。TCP而言，在单一字节流中任何位置的字节丢失都将阻塞该连接上其后所有数据的递送，直到该连接被修复为止。
   
>> SCTP提供多宿特性，使得单个SCTP端点能够支持多个IP地址。

## 2.6 TCP连接的建立和终止
### 2.6.1 三路握手
>> 建立一个TCP连接时会发生下述情形。    
>> (1) 服务器必须准备好接收外来的连接。这通常通过调用socket、bind和listen这3个函数来完成，我们称之为被动打开(passive open)。     
>> (2) 客户端通过调用connect发起主动打开(active open)。这导致客户TCP发送一个SYN(同步)分节，它告诉服务器客户将在(待建立的)连接中发送的数据的初始序列号。通常SYN分节不携带任何数据，其所在的IP数据报只含有一个IP首部、一个TCP首部及可能的TCP选项。    
>> (3) 服务器必须确认(ACK)客户的SYN，同时自己也得发送一个SYN分节，它含有服务器将在同一连接中发送的数据的初始序列号。服务器在单个分节中发送SYN和对客户SYN的ACK(确认)。    
>> (4) 客户必须确认服务器的SYN。

### 2.6.3 TCP连接的终止
>> TCP建立一个连接需要3个分节，终止一个连接需要4个分节。      
>> (1) 某个应用进程首先调用close，我们称该端执行主动关闭(active close)。该端的TCP于是发送一个FIN分节，表示数据发送完毕。     
>> (2) 接收到这个FIN的对端执行被动关闭(passive close)。这个FIN由TCP确认。它的接收也作为一个文件结束符(end-of-file)传递给接收端应用进程（放在已排队等候该应用进程的接收的任何其他数据之后），因为FIN的接收意味着接收端应用进程在相应连接上再无额外数据可接收。     
>> (3) 一段时间后，接收到这个文件结束符的应用进程将调用close关闭它的套接字，这导致它的TCP也发送给一个FIN。    
>> (4) 接收这个最终FIN的原发送端TCP(即主动关闭的那一端)确认这个FIN。    

## 2.7 TIME_WAIT状态
>>执行主动关闭的那端经历了这个状态。该端点停留在这个状态的持续时间是最长分节生命期(maximum segment lifetime, MSL)的两倍，有时候称之为2MSL。   
    
>> TIME_WAIT状态有两个存在的理由：    
>> (1) 可靠地实现TCP全双工连接的终止。[假设最后一个ACK丢失，服务器将重新发送它的最终哪个FIN，因此客户必须维护状态信息，以允许它重新发送最终那个ACK]           
>> (2) 允许老的重复分节在网络中消逝。[不给处在TIME_WAIT状态的连接发起新的化身，保证每成功建立一个TCP连接时，来自该连接先前化身的老的重复分组都已在网络中消逝了]        

## 2.11 缓冲区大小及限制
>>MTU(maximum transmission unit，最大传输单元)   
MSS(maximum segment size，最大分节大小)[用于向对端TCP通告对端在每个分节中发送的最大TCP数据量]    
MSS <= MTU - 40[ipv4]/60[ipv6]，TCP首部20字节，ipv4首部20字节，ipv6首部40字节。   
### 2.11.1 TCP输出
>> 从一个TCP套接字的write调用成功返回仅仅表示可以重新使用原来的应用进程缓冲区，并不表明对端的TCP或应用进程已接收到数据。

# 第3章 套接字编程简介
## 3.3 值-结果参数
>> (1) 从进程到内核传递套接字地址结构的函数有3个：bind、connect和sendto。     
>> (2) 从内核到进程传递套接字地址结构的函数有4个：accept、recvfrom、getsockname和getpeername。     
## 3.4 字节排序函数
>> 内村中存储两个字节有h两种方法:一种将低序字节存储在起始地址，这称为小端(little-endian)字节序；另一种方法是将高序字节存储在起始地址，这称为大端(big-endian)字节序。    

>> 把某个给定系统所使用的字节序称为主机字节序(host byte order)。   

```cpp

#include <netinet/in.h> 
uint16_t htons(uint16_t host16bitvalue);
uint32_t htonl(uint32_t host32bitvalue);    // 均返回网络字节序的值
uint16_t ntohs(uint16_t net16bitvalue);
uint32_t ntohl(uint32_t net32bitvalue);    // 均返回主机字节序的值
```

## 3.5 字节操作函数
```cpp
#include <strings.h>
void bzero(void *dest, size_t nbytes);
void bcopy(const void *src, void *dest, size_t nbytes);
int bcmp(const void *ptr1, const void *ptr2, size_t nbytes);
```

## 3.6 inet_aton、inet_addr和inet_nota函数
```cpp
#include <arpa/inet.h>
// 将strptr所指的C字符串转换成一个32位的网络字节序二进制值，并通过指针addrptr来存储。
int inet_aton(const char *strptr, struct in_addr *addrptr);
// 返回32位的网络字节序二进制[废弃]
in_addr_t inet_addr(const char *strptr);
// 返回一个点分十进制数串的指针
char *inet_ntoa(struct in_addr inaddr);
```
## 3.7 inet_pton和inet_ntop函数
>> p -> presentation n -> numeric
```cpp
#include <arpa/inet.h>
// 转换有strptr指针所指的字符串，并通过addrptr指针存放二进制结果
int inet_pton(int family, const char* strptr, void *addrptr);
// 从数值格式(addrptr)到表达式格式(strptr)
const char* inet_ntop(int family, const void *addrptr, char* strptr, size_t len);
```

# 第4章 基本TCP套接字编程
## 4.2 socket函数
>> 为了执行网络的I/O，一个进程必须做的第一件事情就是调用socket函数，指定期望的通信协议类型。
```cpp
#include <sys/socket.h>
int socket(int family, int type, int protocol);
```

## 4.3 connect函数
>> TCP客户调用connect函数来建立与TCP服务器的连接。
```cpp
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
```

## 4.4 bind函数
>> bind函数把一个本地协议地址赋予一个套接字。
```cpp
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* myaddr, socklen_t addrlen);
```
## 4.5 listen函数
```cpp
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```

>> 内核给任何一个给定的监听套接字维护两个队列：   
>> (1) 未完成连接队列(incomplete connection queue)，每个这样的SYN分节对应其中一项：已由某个客户发出并到达服务器，而服务器正在等待完成相应的TCP三路握手过程。这些套接字处于SYN_RCVD状态。   
>> (2) 已完成连接队列(completed connection queue)，每个已完成TCP三路握手过程的客户对应其中一项。    

>> 当进程调用accept时，已完成连接队列中的队头项将返回给进程，或者如果该队列为空，那么进程将被投入睡眠，直到TCP在该队列中放入一项才唤醒它。   

>> 注意点：  
>> * 在三路握手正常完成的前提下（也就是说分节没有丢失，从而没有重传），未完成连接队列中的任何一项在其中的存留时间就是一个RTT，而RTT的时间取决于特定的客户与服务器。   
>> * 当一个客户SYN到达时，若这些队列是满的，TCP就忽略该分节，也就是不发送RST。这么做是因为：这种情况是暂时的，客户TCP将重发SYN，期望不久就能在这些队列中找到可用空间。要是响应RST，客户无法区别RST意味着该端口没有服务器在监听还是有服务器监听，但是队列满了。

## 4.6 accept函数
```cpp
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
``` 
connfd[已连接套接字[connected socket]]   
sockfd[监听套接字[listening socket]]

## 4.7 fork和exec函数
```cpp
#include <unistd.h>
pid_t fork(void);
```

>> fork调用一次，返回两次。它在调用进程（称为父进程）中返回一次，返回值是新派生进程（称为子进程）的进程ID号，在子进程又返回一次，返回值为0。子进程可以通过调用getppid取得父进程的进程ID。     

>> 父进程fork之前打开的所有描述符在fork返回之后由子进程分享。

>> fork的两个典型用法：   
>> (1) 一个进程创建一个自身副本，这样每个副本都可以在另一个副本执行其他任务的同时处理各自的操作。   
>> (2) 一个进程想要执行另一个程序。副本调用exec函数把自身替换成新的程序[从main开始执行]。   

```cpp
#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ...
               /* (char  *) NULL */);
int execlp(const char *file, const char *arg, ...
               /* (char  *) NULL */);
int execle(const char *path, const char *arg, ...
               /*, (char *) NULL, char * const envp[] */);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[],
               char *const envp[])
```

>> 每个文件或套接字都有一个引用计数。引用计数在文件表项中维护，它是当前打开着的引用该文件或套接字的描述符个数。   
## 4.9 close函数
>> close一个TCP套接字的默认行为是把该套接字标记成已关闭，然后立即返回到调用进程。然而TCP将尝试发送已排队等待发送到对端的任何数据，发送完毕后发生的是正常的TCP连接终止序列。
## 4.10 getsockname和getpeername函数
>> 这两个函数或者返回与某个套接字关联的本地协议地址(getsockname)，或者返回与某个套接字关联的外地址协议(getpeername)。
```cpp
#include <sys/socket.h>
int getsockname(int sockfd, struct sockaddr* localaddr, socklen_t* addrlen);
int getpeername(int sockfd, struct sockaddr* peeraddr, socklen_t* addrlen);
```

# 第5章 TCP客户/服务器程序示例
## 5.8 POSIX信号处理
>> 信号(signal)就是告知某个进程发生了某个事件的通知，有时也称为软件中断(software interrupt)。

>> 信号可以：   
>> * 由一个进程发给另一个进程（或自身）；
>> * 由内核发给某个进程。  

>> 每个信号都有一个与之关联的处置，也称为行为。我们通过调用sigaction函数来设定一个信号的处置，并有三种选择。   
>> (1) 信号处理函数(signal handle)：提供一个函数，只要有特定信号发送它就被调用。这种行为称为捕获(catching)信号。函数原型

```cpp
void handler(int signo);
```

>> (2) 将某个信号的处置设置为SIG_IGN来忽略它。   
>> (3) 将某个信号的处置设定为SIG_DFL来启用它的默认处置。

## 5.10 wait和waitpid函数
```cpp
#include <sys/wait.h>
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
```
>> 函数wait和waitpid均返回两个值：已终止子进程进程ID号，以及通过statloc指针返回的子进程终止状态。

## 5.12 [服务器进程终止](https://blog.csdn.net/tiankong_/article/details/75116174)

## 5.13 SIGPIPE信号
>> 当一个进程向某个已收到RST的套接字执行写操作时，内核向该进程发送一个SIGPIPE信号。该信号的默认行为是终止进程，因此进程必须捕获它以免不情愿地被终止。不论该进程是捕获了该信号并从其信号处理函数返回，还是简单地忽略该信号，写操作都将返回EPIPE错误。

# 第6章 I/O复用：select和poll函数

## 6.2 I/O模型
>> UNIX下可用的五种I/O模型：   
>> * 阻塞式I/O；
>> * 非阻塞式I/O；
>> * I/O复用(select和poll)； 
>> * 信号驱动式I/O(SIGIO)；
>> * 异步I/O(POSIX的aio_列函数)。  

>> 对于一个套接字上的输入操作，第一步通常涉及等待数据从网络中到达。当所等待分组到达时，它被复制到内核中的某个缓冲区。第二步就是把数据从内核缓冲区复制到应用进程缓冲区。  

### 6.2.2 非阻塞式I/O模型
>> 进程把一个套接字设置成非阻塞是在通知内核：当所请求的I/O操作非得把本进程投入睡眠才能完成时，不要把本进程投入睡眠，而是返回一个错误。  

### 6.2.5 异步I/O模型
>>信号驱动式I/O是有内核通知我们何时可以启动一个I/O操作，而异步I/O模型是由内核通知我们I/O操作何时完成。   
## 6.3 select函数
>> 该函数允许进程指示内核等待多个事件中的任何一个发生，并只在有一个或多个事件发生或经历一段指定的时间后才可以唤醒它。   
```cpp
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);
```

### 6.3.1 [描述符就绪条件](https://www.abcode.club/archives/346)

## 6.6 shutdown函数
>> close有两个限制:  
>> (1) close把描述符的引用计数减1，仅在该计数变为0时才关闭套接字。   
>> (2) close终止读和写两个方向的数据传送。

## 6.10 poll函数
```cpp
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd {
               int   fd;         /* file descriptor */
               short events;     /* requested events */
               short revents;    /* returned events */
              };
```

>> poll识别三类数据：普通(normal)、优先级带(priority band)和高优先级(high priority)。

>>POSIX在其poll的定义中留了许多空洞（也就是说有多种方法可返回相同的条件）。     
  （1）所有正规TCP数据和所有UDP数据都被认为是普通数据。     
  （2）TCP的带外数据被认为是优先级数据。     
  （3）当TCP连接的读这一半关闭时（例如收到了一个来自对端的FIN），也被认为是普通数据，随后的读操作将返回0.    
  （4）TCP连接存在错误既可认为是普通数据，也可认为是错误（POLLERR），无论哪种情况，随后的读操作将返回-1，并把errno设置成合适的值。这可用于处理诸如接收到RST或超时发生等条件。    
  （5）在监听套接口上有新的连接可用既可认为是普通数据，也可认为是优先级数据。大多数实现视之为普通数据。     
  （6）非阻塞式connect的完成被认为使得相应套接口可写。   
  
# 第7章 套接字选项
## 7.2 getsockopt和setsockopt函数
```cpp
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

后面感觉看了也记不住，就不看了。。。

# 第8章 基本UDP套接字编程
## 8.2 recvfrom和sendto函数
```cpp
#inlude<sys/socket.h>
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
```
## 8.9 服务器进程未运行
>> 一个基本规则是：对于一个UDP套接字，由它引发的异步错误并不返回给它，除非它已连接。   
>> 仅在进程已将其UDP套接字连接到恰恰一个对端后，这些异步错误才返回给进程。 
## 8.11 UDP的connect函数
>> 对于已连接UDP套接字，与默认的未连接UDP套接字相比，发生了三个变化。   
>> (1) 再也不能给输出操作指定目的的IP地址和端口号。    
>> (2) 不必使用recvfrom以获悉数据报的发送者，而改用read、recv或recvmsg。    
>> (3) 由已连接UDP套接字引发的错误会返回给它们所在的进程，而未连接的UDP套接字不接收任何异步错误。    

>> UDP客户进程或服务器进程只在使用自己的UDP套接字与确定的唯一对端进行通信时，才可以调用connect。   

### 8.11.1 给一个UDP套接字多次调用connect
>> 拥有一个已连接UDP套接字的进程可出于以下两个目的再次调用connect:
>> * 指定新的IP地址和端口号；
>> * 断开套接字。

### 8.11.2 性能
>> 在一个未连接的UDP套接字上给两个数据报调用sendto函数于是涉及内核执行下列6个步骤： 
>> * 连接套接字；
>> * 输出第一个数据报；
>> * 断开套接字连接；
>> * 连接套接字；
>> * 输出第二个数据报；
>> * 断开套接字连接。

>> 调用connect后：
>> * 连接套接字；
>> * 输出第一个套接字；
>> * 输出第二个套接字。

# 第11章 名字与地址转换
## 11.2 域名系统
>> 域名系统(Domain Name System，DNS)主要用于主机名字与IP地址之间的映射。

## 11.3 gethostbyname函数
>> 查找主机名[IPv4]。
```cpp
#include <netdb.h>

struct hostent *gethostbyname(const char *name);

struct hostent {
    char  *h_name;            /* official name of host */
    char **h_aliases;         /* alias list */
    int    h_addrtype;        /* host address type */
    int    h_length;          /* length of address */
    char **h_addr_list;       /* list of addresses */
}

```

后面感觉看了也记不住，就不看了。。。

# 第14章 高级I/O函数
## 14.2 套接字超时
>>在涉及套接字的I/O操作上设置超时的方法有以下3种。   
>> (1) 调用alarm，它在指定超时期满式时产生SIGALARM信号。   
>> (2) 早select中阻塞等待I/O，以此代替直接阻塞在read或write调用上。   
>> (3) 使用较新的SO_RCVTIMEO和SO_SNDTIMEO套接字选项。   

