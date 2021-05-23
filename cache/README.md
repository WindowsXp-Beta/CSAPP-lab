# cache Lab note

> Author:Wei Xinpeng
>
> Date: 2021.4.30

## trace File

Format: [space] operation address,size

记录着内存访问

operation: `I`: instruction load `L`:data load `S`:data store `M`:data modify(data load followed by a data store)

Address: 64位16进制地址

size:访问的字节数

## Part A

需要修改的文件：csim.c 读入trace，输出hit，miss，和evicts的总数。

`./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>`

`<s>` set位的位数，意味着$2^s$个集合数。

`<E>` 每一个set有几个line

`<b>` block bits $2^b$个block size

一些注意：

- csim.c必须没有warning的编译

- 你的simulator必须适用任意的s, E, b，这意味着你需要使用malloc动态分配内存。

+ 我们只关心data cache，因此忽略instruction load。注意到`I`只出现在命令的第一列（没有前置空格）。

+ 在main函数最后应该调用函数`printSummary`。

  ```c
  printSummary(hit_count, miss_count, eviction_count);
  ```

+ 你应该假设内存访问是对齐的，因此单次内存访问不会跨越block，做了这个假设后，你可以忽略valgrind traces要求的size。

## Part B

- 每个transpose函数最多可以定义12个本地变量。
- 你最多定义12个本地变量。如果你使用了帮助函数，那么你定义的本地变量数量得小于12。
- 你不可以修改A数组。
- 你不可以在代码中使用任何数组或者`malloc`。
- 测试的cache的参数为 `s=5,E=1,b=5` 32个集合，每个集合1行，每个block32个字节。

## 求解过程记录

### partA

#### LRU

> LRU = least recently used
>
> [LRU原理和Redis实现——一个今日头条的面试题](https://zhuanlan.zhihu.com/p/34133067?utm_source=qq&utm_medium=social&utm_oi=987118977155428352)
>
> 高级数据结构课上的LimSet lab用哈希表和双链表实现了O(1)的LRU实现。但C里没有STL，而且我们这个模拟器也不追求高性能。

对于每个set，维护一个链表，链表的大小即为line的数量。

```c
struct Node {
  Node * next;
  int lineIndex;//存储cache line在cache[set][index]中的索引
};
```

每次hit，遍历链表，找到该项，将其转换到头结点。

miss，若未满，则将插入cache line置为头结点。

若已满，则删除链表尾对应的cache line并将其置于头结点。

### 解析命令行参数

使用`getopt`函数，在头文件`<unistd.h>`中。

```c
extern char *optarg;
extern int optind,opterr,optopt;
//optind下一次调用getopt时，从optind存储的位置处（argv[optind]）重新开始检查选项。
//opterr设置为0时，getopt不向stderr输出错误信息。
//当命令行选项字符不包括在optstring中或者选项缺少必要的参数时，该选项存储在optopt中，getopt返回'?'。
int getopt(int argc, char * const argv[], const char *optstring);
//返回值为解析到的选项的ascii码值，比如v就是118，注意？是63。
//当命令行选项参数再也检查不到optstring中包含的选项时，返回－1，同时optind储存第一个不包含选项的命令行参数。
//argc argv即为待解析的命令行参数，与main函数相同
//optstring可以含以下内容
//1.单个字母表示选项，即-v -h之类的。
//2.单个字符后接一个冒号：表示该选项后必须跟一个参数。参数 紧跟在选项后 或者 以空格 隔开。
//3.单个字符后跟两个冒号，表示该选项后必须跟一个参数。参数必须 紧跟在选项后 不能以空格 隔开。
//选项的参数字符串的指针赋给 optarg
//attention:默认情况下getopt会重新排列命令行参数的顺序，所以到最后所有 不包含选项的命令行参数（即前面没有    `-字母`的参数都会被排到最后） 都排到最后。即argv的顺序发生了改变
```

