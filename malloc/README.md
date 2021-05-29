# Lab 10: Malloc
## Overall rules
1. 你只需要更改并提交 mm.c 文件。
2. mdriver.c是一个用来检测你的performance的驱动文件。使用方法：使用make编译后使用 `./mdriver -V`来检测它。
3. 不要修改mm.c中的接口。
4. 不要使用 malloc、calloc、free、realloc、sbrk、brk。
5. 不要定义任何全局或静态的复合数据结构，比如array、struct、trees、lists等。但可以使用全局的标量变量，比如int、float、指针。
6. 为了与libc中的malloc保持一致，你的动态内存分配器必须返回以8字节对齐的block。

## guideline
1. 你的动态内存分配器将由四个函数组成。它们在mm.h中声明，在mm.c中定义。我们已经提供了一个最简单的但仍然正确的malloc的package，将它作为一个开始。
   - `int mm_init(void)` 在调用其它三个函数之前，调用它来进行初始化来进行必要的初始化，比如分配初始的堆空间。返回-1如果遇到问题，0 otherwise。
   - `void *mm_malloc(size_t size)` 返回一个指向分配好至少size bytes空间的空间，整个分配好的block应该在堆空间并且不能和其它区域有交叠。
   - `void mm_free(void *ptr)` free ptr指向的block，这个函数只能在ptr之前被分配过，并没有free时work。
   - `void *mm_realloc(void *ptr, size_t size)`
     - 如果ptr是NULL，等价于malloc
     - 如果size是0，等价于free
     - 如果ptr不是NULL，它必须是malloc或realloc的返回值，realloc会改变ptr指向的block的size，并返回新block地址。注意：新block的地址可能与老的相同，也可能不同，这基于你的实现、老block的内部碎片，realloc要求的size等等。新block的内容与老block相同（相同的部分是新老size的较小者），everything else是未初始化的。
   
2. Heap Consistency Checker
   因为动态内存分配众所周知的难写。。。写一个heap checker来检测heap是很有必要的。
   一个heap checker应该检测：
   1. 是否所有在free list中的block都标记为free了。
   2. 是否所有的free block都在free list里。
   3. 有没有相邻的free block没有合并。
   4. free list中的指针，指向了一个valid的free block吗。
   5. 分配的block有没有重叠。
   6. heap block中的指针有没有指向一个valid的堆地址。
   在mm.c中实现`int mm_checker(void)`。**提交mm.c时应保证不调用checker函数**
   
3. `memlib.c`模拟了内存系统，你可以使用其中的函数。
   ```c
   void *mem_sbrk(int incr);
   //将heap扩大incr个bytes
   //返回指向新分配堆空间的first byte的pointer
   //与Unix sbrk函数完全相同，除了只接受正整数
   ```
   
   ```c
   void* mem_heap_lo(void);//返回指向heap中第一个byte的指针
   ```
   
   ```c
   void* mem_heap_hi(void);//返回指向heap最后一个byte的指针
   ```
   
   ```c
   size_t mem_heapsize(void);//返回heap的size
   ```
   
   ```c
   size_t mem_pagesize(void);//返回系统的page size，4k on linux systems
   ```

## evaluate

`mdriver.c`检测你的`mm.c` package的正确性、空间利用率、和吞吐。

- `-t <tracedir>` 使用。
- `-f <tracefile>`使用特定的tracefile来检测而不是默认的tracefiles集合。
- `-h` help
- `-l` 
- `-v` Verbose output。对于每一个tracefile打印性能分析表。
- `-V`更过的verbose。对于每一个trace file打印额外的诊断信息。useful in debugging 来找到哪个trace file导致了你的malloc package fail。