1. y64-ins 33个测试文件，每一个对应着一种Y86指令。
2. y64-err 8个测试文件，每一个对应着一种错误。
3. y64-app 20个测试文件，每一个是一个简单的汇编程序。
4. y64asm.c在扫描完第一遍汇编代码后会调用`relocate`函数填入相应的行号。



一些关键数据结构记录（方便查阅）

```c
typedef struct instr {
    char *name;
    int len;
    byte_t code; /* code for instruction+op */
    int bytes; /* the size of instr */
} instr_t;

typedef struct bin {
    int64_t addr;
    byte_t codes[10];
    int bytes;
} bin_t;

typedef struct line {
    type_t type; /* TYPE_COMM: no y64bin, TYPE_INS: both y64bin and y64asm */
    bin_t y64bin;
    char *y64asm;
    
    struct line *next;
} line_t;

typedef struct symbol {
    char *name;
    int64_t addr;
    struct symbol *next;
} symbol_t;

typedef struct reloc {
    bin_t *y64bin;
    char *name;
    struct reloc *next;
} reloc_t;
```



大致结构：

`assemble` 每次读入一行，让`parse_line()`（核心函数！！）处理成为`line_t`后加入`line_tail`末尾。考虑如何将一条命令变成`line_t`即种类，二进制码，字符串。

每一行有可能有四种情况：`comment` `instruction` ` error` 还有可能是label，对于后者，只需要记住它们的地址，没有对应的二进制码。注意**伪指令**包含在instruction中。伪指令的作用是让汇编器调整地址，以便在那儿产生代码或插入一些数据。

`comment`没有二进制码，不用在`bin`文件中体现，因此一行的默认形式就是comment。

`label`和`symbol`的区别：symbol是ins中出现的label，比如`call main`,`jmp Loop`。

some hints
1. 可以使用`./y64asm -v filename`然后再y64asm中写`printf`调试。
2. 一些debug中碰到的问题在`y64asm.c`中的注释中。