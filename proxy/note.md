# proxy lab

## part A

实现一个顺序的proxy，监听argv[2] port上的请求。

> 使用方法：./proxy listen_port_num

receive a connection request => accept connection => parse http，detemine name of end server => open connection to end server => send it request => receive reply => forward to browser

Logging:你的proxy应当通过打印logging跟踪所有的request

`Date:browserIP URL size`

`browserIP`浏览器IP `URL`浏览器请求的URL，size：从connection开始到结束期间从end server收到的字节数。

## evaluation

1. 使用grade.py检测。
   1. basic proxy：35points，为了满分，你需要正确处理不同的请求。包括 abnormal 异常的。

## Hints

1. 从课本上基础的echo server开始，不断加入功能直到将其变成proxy。
2. 用telnet来debug，连接到一些http-only的网页。
3. 我们希望你专注于网络编程，因此我们提供了两个额外的帮助函数——parse-uri和format_log_entry
4. 当心内存泄漏，当处理一个http request出错了之后，你必须close所有的socket descriptor，释放所有内存。
5. 对于所有IO操作，使用RIO，不要使用standard IO，对于log file的读写，标准IO的fopen和fwrite是可以使用的。

## 踩坑

1. 关于response body，没有规定说一定要有换行符什么的，而且server发完了之后并不会发EOF这样的东西。因此我们需要使用header中的content-length来判断我们是否已经接受了全部的信息，可以close连接了。

2. Rio_readlineb_w这个函数，如果读不到换行符又没读到最大个数，就会一直等在read那，然后timeout了。<del>//不太对socket发完会发EOF的。</del> socket发完（指客户端已经把想说的话说完了，但还没close）并不会发EOF，只有socket被客户端关闭了才会发EOF。

   ```c
   //version1.0
   while(Rio_readlineb_w(&rio_server, rep_from_server, MAXLINE)) {//等不到\n又等不到MAXLINE就等die了。。
     Rio_writen_w(connfd, rep_from_server, strlen(rep_from_server));
   }
   //version2.0
   while(size > 0) {//虽然采用了size，但本质没变，还是会等不到\n又等不到MAXLINE就等die了。。
     size -= Rio_readlineb_w(&rio_server, rep_from_server, MAXLINE);
     Rio_writen_w(connfd, rep_from_server, strlen(rep_from_server));
   }
   //version3.0
   while(cont_length > 0) {//这样就可以，等到读完了cont_length就退出
       size_t rec_size = (cont_length < MAXLINE) ? cont_length : MAXLINE;
       ssize_t len = Rio_readlineb_w(&rio_server, rep_from_server, rec_size + 1);//加一的原因见下一条
       cont_length -= len;
       size += len;
       Rio_writen_w(connfd, rep_from_server, len);//为什么这里要用len而不是strlen，server当然可能发\0夹在一串文本里
   }
   ```

   

3. 设置最大个数也需要当心，它只能读到`MAXLINE-1`因此设置最大个数为content-length的话，它只能读content-length-1个字符然后退出来，再进循环。此时它要读一个，然而它只能读到1-1=0个，然后它就陷入无尽循环，又timeout了。。。

4. 不同的HTTP methods处理方法不同。GET 是没有request body的。其他method比如PUT，POST都是有request body的，因此request header中会有一项Content-length来表示body的长度，因此需要像处理response body一样处理这样的method。

5. part6请求头太长，超过8192，所以要先发一次，再继续sprintf。。

6. part4检验crash很好过，因为它只检查你的proxy有没有挂，没有挂就帮你重启，所以就算proxy死在那了也没事，但part7中只重启server和client，因此你的proxy必须得正确处理错误，得能及时退出。

7. grade.py是怎么模拟crash的：将一个HTTP请求分为三个部分，request就是请求行，也就是GET url HTTP/1.1，header就是请求头，body就是request body。而各个部分的crash就是随机截取一段（从头开始，结束在中间某个地方），发给proxy。解决这个问题的方法就是所有的`Rio_readlinb_w`全部检测是否返回0。

8. 关于打印完整的请求信息发现，client发来的请求是本地（127.0.0.1开头），但却往`www.baidu.com`等之类的地方发消息，这是因为它要控制server和client的crash，因此lab提供的server二进制文件其实也是个proxy。



