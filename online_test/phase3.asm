   0x0000000000401465 <+0>:     sub    $0x18,%rsp
   0x0000000000401469 <+4>:     lea    0x7(%rsp),%rcx
   0x000000000040146e <+9>:     lea    0xc(%rsp),%rdx
   0x0000000000401473 <+14>:    lea    0x8(%rsp),%r8
   0x0000000000401478 <+19>:    lea    0x1cb4(%rip),%rsi        # 0x403133
   0x000000000040147f <+26>:    mov    $0x0,%eax
   0x0000000000401484 <+31>:    callq  0x401160 <__isoc99_sscanf@plt>
   0x0000000000401489 <+36>:    cmp    $0x2,%eax
   0x000000000040148c <+39>:    jle    0x4014ad <phase_3+72>
   0x000000000040148e <+41>:    cmpl   $0x7,0xc(%rsp)
   0x0000000000401493 <+46>:    ja     0x4015a5 <phase_3+320>
   0x0000000000401499 <+52>:    mov    0xc(%rsp),%eax
   0x000000000040149d <+56>:    lea    0x1c9c(%rip),%rdx        # 0x403140
   0x00000000004014a4 <+63>:    movslq (%rdx,%rax,4),%rax
   0x00000000004014a8 <+67>:    add    %rdx,%rax
   0x00000000004014ab <+70>:    jmpq   *%rax
   0x00000000004014ad <+72>:    callq  0x401eeb <explode_bomb>
   0x00000000004014b2 <+77>:    jmp    0x40148e <phase_3+41>
   0x00000000004014b4 <+79>:    mov    $0x78,%eax
   0x00000000004014b9 <+84>:    cmpl   $0x125,0x8(%rsp)
   0x00000000004014c1 <+92>:    je     0x4015af <phase_3+330>
   0x00000000004014c7 <+98>:    callq  0x401eeb <explode_bomb>
   0x00000000004014cc <+103>:   mov    $0x78,%eax
   0x00000000004014d1 <+108>:   jmpq   0x4015af <phase_3+330>
   0x00000000004014d6 <+113>:   mov    $0x67,%eax
   0x00000000004014db <+118>:   cmpl   $0x314,0x8(%rsp)
   0x00000000004014e3 <+126>:   je     0x4015af <phase_3+330>
   0x00000000004014e9 <+132>:   callq  0x401eeb <explode_bomb>
   0x00000000004014ee <+137>:   mov    $0x67,%eax
   0x00000000004014f3 <+142>:   jmpq   0x4015af <phase_3+330>
   0x00000000004014f8 <+147>:   mov    $0x6c,%eax
   0x00000000004014fd <+152>:   cmpl   $0x1f1,0x8(%rsp)
   0x0000000000401505 <+160>:   je     0x4015af <phase_3+330>
   0x000000000040150b <+166>:   callq  0x401eeb <explode_bomb>
   0x0000000000401510 <+171>:   mov    $0x6c,%eax
   0x0000000000401515 <+176>:   jmpq   0x4015af <phase_3+330>
   0x000000000040151a <+181>:   mov    $0x74,%eax
   0x000000000040151f <+186>:   cmpl   $0x10c,0x8(%rsp)
   0x0000000000401527 <+194>:   je     0x4015af <phase_3+330>
   0x000000000040152d <+200>:   callq  0x401eeb <explode_bomb>
   0x0000000000401532 <+205>:   mov    $0x74,%eax
   0x0000000000401537 <+210>:   jmp    0x4015af <phase_3+330>
   0x0000000000401539 <+212>:   mov    $0x7a,%eax
   0x000000000040153e <+217>:   cmpl   $0x105,0x8(%rsp)
   0x0000000000401546 <+225>:   je     0x4015af <phase_3+330>
   0x0000000000401548 <+227>:   callq  0x401eeb <explode_bomb>
   0x000000000040154d <+232>:   mov    $0x7a,%eax
   0x0000000000401552 <+237>:   jmp    0x4015af <phase_3+330>
   0x0000000000401554 <+239>:   mov    $0x6e,%eax
   0x0000000000401559 <+244>:   cmpl   $0x226,0x8(%rsp)
   0x0000000000401561 <+252>:   je     0x4015af <phase_3+330>
   0x0000000000401563 <+254>:   callq  0x401eeb <explode_bomb>
   0x0000000000401568 <+259>:   mov    $0x6e,%eax
   0x000000000040156d <+264>:   jmp    0x4015af <phase_3+330>
   0x000000000040156f <+266>:   mov    $0x70,%eax
   0x0000000000401574 <+271>:   cmpl   $0x131,0x8(%rsp)
   0x000000000040157c <+279>:   je     0x4015af <phase_3+330>
   0x000000000040157e <+281>:   callq  0x401eeb <explode_bomb>
   0x0000000000401583 <+286>:   mov    $0x70,%eax
   0x0000000000401588 <+291>:   jmp    0x4015af <phase_3+330>
   0x000000000040158a <+293>:   mov    $0x79,%eax
   0x000000000040158f <+298>:   cmpl   $0x3b0,0x8(%rsp)
   0x0000000000401597 <+306>:   je     0x4015af <phase_3+330>
   0x0000000000401599 <+308>:   callq  0x401eeb <explode_bomb>
   0x000000000040159e <+313>:   mov    $0x79,%eax
   0x00000000004015a3 <+318>:   jmp    0x4015af <phase_3+330>
   0x00000000004015a5 <+320>:   callq  0x401eeb <explode_bomb>
   0x00000000004015aa <+325>:   mov    $0x67,%eax
   0x00000000004015af <+330>:   cmp    %al,0x7(%rsp)
   0x00000000004015b3 <+334>:   jne    0x4015ba <phase_3+341>
   0x00000000004015b5 <+336>:   add    $0x18,%rsp
   0x00000000004015b9 <+340>:   retq   
   0x00000000004015ba <+341>:   callq  0x401eeb <explode_bomb>
   0x00000000004015bf <+346>:   jmp    0x4015b5 <phase_3+336>