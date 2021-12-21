## level1
```bash
disas touch1
```
```
0x0000000000401b6a <+0>:     sub    $0x8,%rsp
0x0000000000401b6e <+4>:     movl   $0x1,0x5984(%rip)        # 0x4074fc <vlevel>
0x0000000000401b78 <+14>:    lea    0x274c(%rip),%rdi        # 0x4042cb
0x0000000000401b7f <+21>:    callq  0x401080 <puts@plt>
0x0000000000401b84 <+26>:    mov    $0x1,%edi
0x0000000000401b89 <+31>:    callq  0x401fb6 <validate>
0x0000000000401b8e <+36>:    mov    $0x0,%edi
0x0000000000401b93 <+41>:    callq  0x4011f0 <exit@plt>
```
```bash
disas getbuf
```
```
0x0000000000401b54 <+0>:     sub    $0x28,%rsp
0x0000000000401b58 <+4>:     mov    %rsp,%rdi
0x0000000000401b5b <+7>:     callq  0x401da3 <Gets>
0x0000000000401b60 <+12>:    mov    $0x1,%eax
0x0000000000401b65 <+17>:    add    $0x28,%rsp
0x0000000000401b69 <+21>:    retq
```
## level2
aim: call touch2 with first argument is your cookie
```assembly
movq 0x2a328fb5,%rdi
pushq 0x401b98 # touch2 begins at 0x401b98
retq
```
> Above assembly is wrong because immediate needs to start with a `$`!!!
And assembly begins at 0x5566b658.
