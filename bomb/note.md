## phase 2
```c
read_six_numbers(char *input, int *a){
    return sscanf(input, "%d %d %d %d %d %d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]);
}
```
## phase 3
-7320   -7286   -7252   -7218   -7187   -7160
0x401528	0x40154A	0x40156C
I use path of 0x401528. There are other paths.
```c
int main() {
	scanf(input, "%d %c %d", &a, &b, &c);
	//I choose path 1, so my a is 0
	if (0x394 == c) {
		if (b == 0x71 /* q */) {
			defuse();
		}
	}
}
```
## phase 4
```c
int func4(int a) {
	if (a <= 0) return 1;
	else {
		return 7 * func4(a - 1);
	}
}
int main() {
	int a;
	scanf(input, "%d", &a);
	if (0x31 == func4(a)) {
		defuse();
	}
}
```
## phase 5
target = "saints"
corpus = "isrveawhobpnutfg"
Low 4 bits of char should be 1 5 0 11 13 1
So you can choose any ascii word ending with above bits.
48 + 1 = '1'
48 + 5 = '5'
48 + 0 = '0'
48 + 11 = ';'
48 + 13 = '='
48 + 1 = '1'
## phase 6

41: jump 210
181 ~ 241
```c
int j = 1;
for (int i = 0; ; i++, j++){
    if (a[i] - 1 > 5) Bomb;
    if (j == 6) break;
    for (int k = j; k <= 5; k++) {
        if (a[k] == a[i]) Bomb;
    }
}
```

jump 56
```c
node *b[]; //begin at %rsp
struct node{
    int content;
    node *next;
};
node *head;//head is 0x405750
for (int i = 0; i < 6; i++) {
    int v = 1;
    node *tmp = head;
    if (a[i] > 1) {        
        do {
            v++;
            tmp = tmp -> next;
        } while (v != a[i]);
    } else {
	    tmp = head;
    }
    b[i] = tmp;
}
```
begin at 112
```c
rbx = b[0];
rax = b[1];
b[0]->next = b[1];
...
b[5]->next = null;
```

begin at 243
```c
ebp = 5;
rbx = b[0];
do {
	rbx = rbx->next;
	ebp--;
	if (ebp == 0) return;
	rax = rbx->next;
} while(*rbx >= *rax);
/*
node[1] > node[2]
node[2] > node[3]
node[3] > node[4]
node[4] > node[5]
 */
```

original node
0x405750 124 0x405760
0x405760 1ce 0x405770
0x405770 22a 0x405780
0x405780 23f 0x405790
0x405790 379 0x405240
0x405240 39d 0x000000
## secret phase
how to enter
disas phase_defused and you will find there's another sscanf and if you input nothing you will skip the secret since it will check return value of sscanf.

```assembly
0x00000000004022da <+0>:     sub    $0x68,%rsp
0x00000000004022de <+4>:     mov    $0x1,%edi
0x00000000004022e3 <+9>:     callq  0x401ba5 <send_msg>
0x00000000004022e8 <+14>:    cmpl   $0x6,0x34cd(%rip)        # 0x4057bc <num_input_strings>
0x00000000004022ef <+21>:    je     0x4022f6 <phase_defused+28>
0x00000000004022f1 <+23>:    add    $0x68,%rsp
0x00000000004022f5 <+27>:    retq   
0x00000000004022f6 <+28>:    lea    0x10(%rsp),%rcx
0x00000000004022fb <+33>:    lea    0xc(%rsp),%rdx
0x0000000000402300 <+38>:    lea    0x124f(%rip),%rsi        # 0x403556
0x0000000000402307 <+45>:    lea    0x35c2(%rip),%rdi        # 0x4058d0 <input_strings+240>
0x000000000040230e <+52>:    mov    $0x0,%eax
0x0000000000402313 <+57>:    callq  0x401170 <__isoc99_sscanf@plt>
0x0000000000402318 <+62>:    cmp    $0x2,%eax
0x000000000040231b <+65>:    je     0x402337 <phase_defused+93>
0x000000000040231d <+67>:    lea    0xf8c(%rip),%rdi        # 0x4032b0
0x0000000000402324 <+74>:    callq  0x401060 <puts@plt>
0x0000000000402329 <+79>:    lea    0xfb0(%rip),%rdi        # 0x4032e0
0x0000000000402330 <+86>:    callq  0x401060 <puts@plt>
0x0000000000402335 <+91>:    jmp    0x4022f1 <phase_defused+23>
0x0000000000402337 <+93>:    lea    0x10(%rsp),%rdi
0x000000000040233c <+98>:    lea    0x1219(%rip),%rsi        # 0x40355c
0x0000000000402343 <+105>:   callq  0x40194d <strings_not_equal>
0x0000000000402348 <+110>:   test   %eax,%eax
0x000000000040234a <+112>:   jne    0x40231d <phase_defused+67>
0x000000000040234c <+114>:   lea    0xefd(%rip),%rdi        # 0x403250
0x0000000000402353 <+121>:   callq  0x401060 <puts@plt>
0x0000000000402358 <+126>:   lea    0xf19(%rip),%rdi        # 0x403278
0x000000000040235f <+133>:   callq  0x401060 <puts@plt>
0x0000000000402364 <+138>:   mov    $0x0,%eax
0x0000000000402369 <+143>:   callq  0x40185e <secret_phase>
0x000000000040236e <+148>:   jmp    0x40231d <phase_defused+67>
```

0x34cd(%rip) 这种格式代表全局变量，`# 0x4057bc <num_input_strings>` 估计就是已经解决的phase的个数。

在 .data段。可以参考ICS链接那一章，code段和data段的相对位置时不变的。所以可以用rip访问。

+57行的sscanf很关键，查询%rdi发现是phase4输入的数据，%rsi变成了"%d %s" austinpower就在这里输入

```c
a = rsp + 12;
b = rsp + 16;
int num = scanf(2, "%d %s", &a, &b);
int strings_not_equal(char *b, "austinpowers") {
	if (length(b) != length(c)) return 1;
	if (b[0] == 0) return 0;

}
```