
farm.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <start_farm>:
   0:	f3 0f 1e fa          	endbr64 
   4:	b8 01 00 00 00       	mov    $0x1,%eax
   9:	c3                   	retq   

000000000000000a <getval_440>:
   a:	f3 0f 1e fa          	endbr64 
   e:	b8 48 89 c7 c3       	mov    $0xc3c78948,%eax
  13:	c3                   	retq   

0000000000000014 <setval_206>:
  14:	f3 0f 1e fa          	endbr64 
  18:	c7 07 c8 89 c7 c3    	movl   $0xc3c789c8,(%rdi)
  1e:	c3                   	retq   

000000000000001f <addval_234>:
  1f:	f3 0f 1e fa          	endbr64 
  23:	8d 87 d8 c3 da f9    	lea    -0x6253c28(%rdi),%eax
  29:	c3                   	retq   

000000000000002a <setval_310>:
  2a:	f3 0f 1e fa          	endbr64 
  2e:	c7 07 78 90 90 c3    	movl   $0xc3909078,(%rdi)
  34:	c3                   	retq   

0000000000000035 <addval_412>:
  35:	f3 0f 1e fa          	endbr64 
  39:	8d 87 48 89 c7 90    	lea    -0x6f3876b8(%rdi),%eax
  3f:	c3                   	retq   

0000000000000040 <addval_370>:
  40:	f3 0f 1e fa          	endbr64 
  44:	8d 87 58 90 90 c3    	lea    -0x3c6f6fa8(%rdi),%eax
  4a:	c3                   	retq   

000000000000004b <getval_498>:
  4b:	f3 0f 1e fa          	endbr64 
  4f:	b8 48 89 c7 91       	mov    $0x91c78948,%eax
  54:	c3                   	retq   

0000000000000055 <getval_462>:
  55:	f3 0f 1e fa          	endbr64 
  59:	b8 58 90 90 c3       	mov    $0xc3909058,%eax
  5e:	c3                   	retq   

000000000000005f <mid_farm>:
  5f:	f3 0f 1e fa          	endbr64 
  63:	b8 01 00 00 00       	mov    $0x1,%eax
  68:	c3                   	retq   

0000000000000069 <add_xy>:
  69:	f3 0f 1e fa          	endbr64 
  6d:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
  71:	c3                   	retq   

0000000000000072 <getval_421>:
  72:	f3 0f 1e fa          	endbr64 
  76:	b8 89 ce 60 d2       	mov    $0xd260ce89,%eax
  7b:	c3                   	retq   

000000000000007c <setval_454>:
  7c:	f3 0f 1e fa          	endbr64 
  80:	c7 07 48 89 e0 94    	movl   $0x94e08948,(%rdi)
  86:	c3                   	retq   

0000000000000087 <getval_455>:
  87:	f3 0f 1e fa          	endbr64 
  8b:	b8 8b d1 38 c9       	mov    $0xc938d18b,%eax
  90:	c3                   	retq   

0000000000000091 <setval_263>:
  91:	f3 0f 1e fa          	endbr64 
  95:	c7 07 a9 c2 84 d2    	movl   $0xd284c2a9,(%rdi)
  9b:	c3                   	retq   

000000000000009c <addval_122>:
  9c:	f3 0f 1e fa          	endbr64 
  a0:	8d 87 21 89 ce c3    	lea    -0x3c3176df(%rdi),%eax
  a6:	c3                   	retq   

00000000000000a7 <addval_244>:
  a7:	f3 0f 1e fa          	endbr64 
  ab:	8d 87 89 d1 08 c9    	lea    -0x36f72e77(%rdi),%eax
  b1:	c3                   	retq   

00000000000000b2 <addval_383>:
  b2:	f3 0f 1e fa          	endbr64 
  b6:	8d 87 89 d1 28 db    	lea    -0x24d72e77(%rdi),%eax
  bc:	c3                   	retq   

00000000000000bd <getval_194>:
  bd:	f3 0f 1e fa          	endbr64 
  c1:	b8 89 c2 60 db       	mov    $0xdb60c289,%eax
  c6:	c3                   	retq   

00000000000000c7 <addval_458>:
  c7:	f3 0f 1e fa          	endbr64 
  cb:	8d 87 c9 ce 90 90    	lea    -0x6f6f3137(%rdi),%eax
  d1:	c3                   	retq   

00000000000000d2 <addval_395>:
  d2:	f3 0f 1e fa          	endbr64 
  d6:	8d 87 88 c2 84 c0    	lea    -0x3f7b3d78(%rdi),%eax
  dc:	c3                   	retq   

00000000000000dd <setval_142>:
  dd:	f3 0f 1e fa          	endbr64 
  e1:	c7 07 89 c2 30 db    	movl   $0xdb30c289,(%rdi)
  e7:	c3                   	retq   

00000000000000e8 <getval_340>:
  e8:	f3 0f 1e fa          	endbr64 
  ec:	b8 89 d1 94 c9       	mov    $0xc994d189,%eax
  f1:	c3                   	retq   

00000000000000f2 <addval_209>:
  f2:	f3 0f 1e fa          	endbr64 
  f6:	8d 87 ee 51 8b c2    	lea    -0x3d74ae12(%rdi),%eax
  fc:	c3                   	retq   

00000000000000fd <getval_288>:
  fd:	f3 0f 1e fa          	endbr64 
 101:	b8 48 89 e0 c3       	mov    $0xc3e08948,%eax
 106:	c3                   	retq   

0000000000000107 <setval_423>:
 107:	f3 0f 1e fa          	endbr64 
 10b:	c7 07 48 89 e0 c2    	movl   $0xc2e08948,(%rdi)
 111:	c3                   	retq   

0000000000000112 <addval_106>:
 112:	f3 0f 1e fa          	endbr64 
 116:	8d 87 8b ce c3 a4    	lea    -0x5b3c3175(%rdi),%eax
 11c:	c3                   	retq   

000000000000011d <getval_479>:
 11d:	f3 0f 1e fa          	endbr64 
 121:	b8 89 ce 90 c3       	mov    $0xc390ce89,%eax
 126:	c3                   	retq   

0000000000000127 <setval_245>:
 127:	f3 0f 1e fa          	endbr64 
 12b:	c7 07 48 89 e0 c3    	movl   $0xc3e08948,(%rdi)
 131:	c3                   	retq   

0000000000000132 <getval_150>:
 132:	f3 0f 1e fa          	endbr64 
 136:	b8 09 d1 20 c0       	mov    $0xc020d109,%eax
 13b:	c3                   	retq   

000000000000013c <getval_468>:
 13c:	f3 0f 1e fa          	endbr64 
 140:	b8 09 ce 84 db       	mov    $0xdb84ce09,%eax
 145:	c3                   	retq   

0000000000000146 <getval_171>:
 146:	f3 0f 1e fa          	endbr64 
 14a:	b8 48 89 e0 94       	mov    $0x94e08948,%eax
 14f:	c3                   	retq   

0000000000000150 <setval_289>:
 150:	f3 0f 1e fa          	endbr64 
 154:	c7 07 89 d1 30 d2    	movl   $0xd230d189,(%rdi)
 15a:	c3                   	retq   

000000000000015b <setval_153>:
 15b:	f3 0f 1e fa          	endbr64 
 15f:	c7 07 89 c2 20 c9    	movl   $0xc920c289,(%rdi)
 165:	c3                   	retq   

0000000000000166 <setval_481>:
 166:	f3 0f 1e fa          	endbr64 
 16a:	c7 07 48 a9 e0 c3    	movl   $0xc3e0a948,(%rdi)
 170:	c3                   	retq   

0000000000000171 <setval_415>:
 171:	f3 0f 1e fa          	endbr64 
 175:	c7 07 81 ce 38 db    	movl   $0xdb38ce81,(%rdi)
 17b:	c3                   	retq   

000000000000017c <getval_339>:
 17c:	f3 0f 1e fa          	endbr64 
 180:	b8 89 c2 00 c0       	mov    $0xc000c289,%eax
 185:	c3                   	retq   

0000000000000186 <setval_183>:
 186:	f3 0f 1e fa          	endbr64 
 18a:	c7 07 ea 89 d1 c3    	movl   $0xc3d189ea,(%rdi)
 190:	c3                   	retq   

0000000000000191 <setval_120>:
 191:	f3 0f 1e fa          	endbr64 
 195:	c7 07 48 89 e0 c2    	movl   $0xc2e08948,(%rdi)
 19b:	c3                   	retq   

000000000000019c <setval_359>:
 19c:	f3 0f 1e fa          	endbr64 
 1a0:	c7 07 55 89 c2 90    	movl   $0x90c28955,(%rdi)
 1a6:	c3                   	retq   

00000000000001a7 <addval_428>:
 1a7:	f3 0f 1e fa          	endbr64 
 1ab:	8d 87 89 ce 48 d2    	lea    -0x2db73177(%rdi),%eax
 1b1:	c3                   	retq   

00000000000001b2 <setval_316>:
 1b2:	f3 0f 1e fa          	endbr64 
 1b6:	c7 07 89 d1 c4 c0    	movl   $0xc0c4d189,(%rdi)
 1bc:	c3                   	retq   

00000000000001bd <getval_478>:
 1bd:	f3 0f 1e fa          	endbr64 
 1c1:	b8 48 89 e0 c7       	mov    $0xc7e08948,%eax
 1c6:	c3                   	retq   

00000000000001c7 <end_farm>:
 1c7:	f3 0f 1e fa          	endbr64 
 1cb:	b8 01 00 00 00       	mov    $0x1,%eax
 1d0:	c3                   	retq   
