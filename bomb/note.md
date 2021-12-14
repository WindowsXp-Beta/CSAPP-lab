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
disas phase_defused and you will find there's another sscanf and if you input nothing you will skip the secret since it will check return value of scanf.

```c
a = rsp + 12;
b = rsp + 16;
int num = scanf(2, "%d %s", &a, &b);
int strings_not_equal(char *b, "austinpowers") {
	if (length(b) != length(c)) return 1;
	if (b[0] == 0) return 0;

}
```