
farm.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <start_farm>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	b8 01 00 00 00       	mov    $0x1,%eax
   d:	5d                   	pop    %rbp
   e:	c3                   	retq   

000000000000000f <getval_440>:
   f:	f3 0f 1e fa          	endbr64 
  13:	55                   	push   %rbp
  14:	48 89 e5             	mov    %rsp,%rbp
  17:	b8 48 89 c7 c3       	mov    $0xc3c78948,%eax
  1c:	5d                   	pop    %rbp
  1d:	c3                   	retq   

000000000000001e <setval_206>:
  1e:	f3 0f 1e fa          	endbr64 
  22:	55                   	push   %rbp
  23:	48 89 e5             	mov    %rsp,%rbp
  26:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  2a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  2e:	c7 00 c8 89 c7 c3    	movl   $0xc3c789c8,(%rax)
  34:	90                   	nop
  35:	5d                   	pop    %rbp
  36:	c3                   	retq   

0000000000000037 <addval_234>:
  37:	f3 0f 1e fa          	endbr64 
  3b:	55                   	push   %rbp
  3c:	48 89 e5             	mov    %rsp,%rbp
  3f:	89 7d fc             	mov    %edi,-0x4(%rbp)
  42:	8b 45 fc             	mov    -0x4(%rbp),%eax
  45:	2d 28 3c 25 06       	sub    $0x6253c28,%eax
  4a:	5d                   	pop    %rbp
  4b:	c3                   	retq   

000000000000004c <setval_310>:
  4c:	f3 0f 1e fa          	endbr64 
  50:	55                   	push   %rbp
  51:	48 89 e5             	mov    %rsp,%rbp
  54:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  58:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  5c:	c7 00 78 90 90 c3    	movl   $0xc3909078,(%rax)
  62:	90                   	nop
  63:	5d                   	pop    %rbp
  64:	c3                   	retq   

0000000000000065 <addval_412>:
  65:	f3 0f 1e fa          	endbr64 
  69:	55                   	push   %rbp
  6a:	48 89 e5             	mov    %rsp,%rbp
  6d:	89 7d fc             	mov    %edi,-0x4(%rbp)
  70:	8b 45 fc             	mov    -0x4(%rbp),%eax
  73:	2d b8 76 38 6f       	sub    $0x6f3876b8,%eax
  78:	5d                   	pop    %rbp
  79:	c3                   	retq   

000000000000007a <addval_370>:
  7a:	f3 0f 1e fa          	endbr64 
  7e:	55                   	push   %rbp
  7f:	48 89 e5             	mov    %rsp,%rbp
  82:	89 7d fc             	mov    %edi,-0x4(%rbp)
  85:	8b 45 fc             	mov    -0x4(%rbp),%eax
  88:	2d a8 6f 6f 3c       	sub    $0x3c6f6fa8,%eax
  8d:	5d                   	pop    %rbp
  8e:	c3                   	retq   

000000000000008f <getval_498>:
  8f:	f3 0f 1e fa          	endbr64 
  93:	55                   	push   %rbp
  94:	48 89 e5             	mov    %rsp,%rbp
  97:	b8 48 89 c7 91       	mov    $0x91c78948,%eax
  9c:	5d                   	pop    %rbp
  9d:	c3                   	retq   

000000000000009e <getval_462>:
  9e:	f3 0f 1e fa          	endbr64 
  a2:	55                   	push   %rbp
  a3:	48 89 e5             	mov    %rsp,%rbp
  a6:	b8 58 90 90 c3       	mov    $0xc3909058,%eax
  ab:	5d                   	pop    %rbp
  ac:	c3                   	retq   

00000000000000ad <mid_farm>:
  ad:	f3 0f 1e fa          	endbr64 
  b1:	55                   	push   %rbp
  b2:	48 89 e5             	mov    %rsp,%rbp
  b5:	b8 01 00 00 00       	mov    $0x1,%eax
  ba:	5d                   	pop    %rbp
  bb:	c3                   	retq   

00000000000000bc <add_xy>:
  bc:	f3 0f 1e fa          	endbr64 
  c0:	55                   	push   %rbp
  c1:	48 89 e5             	mov    %rsp,%rbp
  c4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  c8:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  cc:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  d0:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  d4:	48 01 d0             	add    %rdx,%rax
  d7:	5d                   	pop    %rbp
  d8:	c3                   	retq   

00000000000000d9 <getval_421>:
  d9:	f3 0f 1e fa          	endbr64 
  dd:	55                   	push   %rbp
  de:	48 89 e5             	mov    %rsp,%rbp
  e1:	b8 89 ce 60 d2       	mov    $0xd260ce89,%eax
  e6:	5d                   	pop    %rbp
  e7:	c3                   	retq   

00000000000000e8 <setval_454>:
  e8:	f3 0f 1e fa          	endbr64 
  ec:	55                   	push   %rbp
  ed:	48 89 e5             	mov    %rsp,%rbp
  f0:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  f4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  f8:	c7 00 48 89 e0 94    	movl   $0x94e08948,(%rax)
  fe:	90                   	nop
  ff:	5d                   	pop    %rbp
 100:	c3                   	retq   

0000000000000101 <getval_455>:
 101:	f3 0f 1e fa          	endbr64 
 105:	55                   	push   %rbp
 106:	48 89 e5             	mov    %rsp,%rbp
 109:	b8 8b d1 38 c9       	mov    $0xc938d18b,%eax
 10e:	5d                   	pop    %rbp
 10f:	c3                   	retq   

0000000000000110 <setval_263>:
 110:	f3 0f 1e fa          	endbr64 
 114:	55                   	push   %rbp
 115:	48 89 e5             	mov    %rsp,%rbp
 118:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 11c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 120:	c7 00 a9 c2 84 d2    	movl   $0xd284c2a9,(%rax)
 126:	90                   	nop
 127:	5d                   	pop    %rbp
 128:	c3                   	retq   

0000000000000129 <addval_122>:
 129:	f3 0f 1e fa          	endbr64 
 12d:	55                   	push   %rbp
 12e:	48 89 e5             	mov    %rsp,%rbp
 131:	89 7d fc             	mov    %edi,-0x4(%rbp)
 134:	8b 45 fc             	mov    -0x4(%rbp),%eax
 137:	2d df 76 31 3c       	sub    $0x3c3176df,%eax
 13c:	5d                   	pop    %rbp
 13d:	c3                   	retq   

000000000000013e <addval_244>:
 13e:	f3 0f 1e fa          	endbr64 
 142:	55                   	push   %rbp
 143:	48 89 e5             	mov    %rsp,%rbp
 146:	89 7d fc             	mov    %edi,-0x4(%rbp)
 149:	8b 45 fc             	mov    -0x4(%rbp),%eax
 14c:	2d 77 2e f7 36       	sub    $0x36f72e77,%eax
 151:	5d                   	pop    %rbp
 152:	c3                   	retq   

0000000000000153 <addval_383>:
 153:	f3 0f 1e fa          	endbr64 
 157:	55                   	push   %rbp
 158:	48 89 e5             	mov    %rsp,%rbp
 15b:	89 7d fc             	mov    %edi,-0x4(%rbp)
 15e:	8b 45 fc             	mov    -0x4(%rbp),%eax
 161:	2d 77 2e d7 24       	sub    $0x24d72e77,%eax
 166:	5d                   	pop    %rbp
 167:	c3                   	retq   

0000000000000168 <getval_194>:
 168:	f3 0f 1e fa          	endbr64 
 16c:	55                   	push   %rbp
 16d:	48 89 e5             	mov    %rsp,%rbp
 170:	b8 89 c2 60 db       	mov    $0xdb60c289,%eax
 175:	5d                   	pop    %rbp
 176:	c3                   	retq   

0000000000000177 <addval_458>:
 177:	f3 0f 1e fa          	endbr64 
 17b:	55                   	push   %rbp
 17c:	48 89 e5             	mov    %rsp,%rbp
 17f:	89 7d fc             	mov    %edi,-0x4(%rbp)
 182:	8b 45 fc             	mov    -0x4(%rbp),%eax
 185:	2d 37 31 6f 6f       	sub    $0x6f6f3137,%eax
 18a:	5d                   	pop    %rbp
 18b:	c3                   	retq   

000000000000018c <addval_395>:
 18c:	f3 0f 1e fa          	endbr64 
 190:	55                   	push   %rbp
 191:	48 89 e5             	mov    %rsp,%rbp
 194:	89 7d fc             	mov    %edi,-0x4(%rbp)
 197:	8b 45 fc             	mov    -0x4(%rbp),%eax
 19a:	2d 78 3d 7b 3f       	sub    $0x3f7b3d78,%eax
 19f:	5d                   	pop    %rbp
 1a0:	c3                   	retq   

00000000000001a1 <setval_142>:
 1a1:	f3 0f 1e fa          	endbr64 
 1a5:	55                   	push   %rbp
 1a6:	48 89 e5             	mov    %rsp,%rbp
 1a9:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 1ad:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 1b1:	c7 00 89 c2 30 db    	movl   $0xdb30c289,(%rax)
 1b7:	90                   	nop
 1b8:	5d                   	pop    %rbp
 1b9:	c3                   	retq   

00000000000001ba <getval_340>:
 1ba:	f3 0f 1e fa          	endbr64 
 1be:	55                   	push   %rbp
 1bf:	48 89 e5             	mov    %rsp,%rbp
 1c2:	b8 89 d1 94 c9       	mov    $0xc994d189,%eax
 1c7:	5d                   	pop    %rbp
 1c8:	c3                   	retq   

00000000000001c9 <addval_209>:
 1c9:	f3 0f 1e fa          	endbr64 
 1cd:	55                   	push   %rbp
 1ce:	48 89 e5             	mov    %rsp,%rbp
 1d1:	89 7d fc             	mov    %edi,-0x4(%rbp)
 1d4:	8b 45 fc             	mov    -0x4(%rbp),%eax
 1d7:	2d 12 ae 74 3d       	sub    $0x3d74ae12,%eax
 1dc:	5d                   	pop    %rbp
 1dd:	c3                   	retq   

00000000000001de <getval_288>:
 1de:	f3 0f 1e fa          	endbr64 
 1e2:	55                   	push   %rbp
 1e3:	48 89 e5             	mov    %rsp,%rbp
 1e6:	b8 48 89 e0 c3       	mov    $0xc3e08948,%eax
 1eb:	5d                   	pop    %rbp
 1ec:	c3                   	retq   

00000000000001ed <setval_423>:
 1ed:	f3 0f 1e fa          	endbr64 
 1f1:	55                   	push   %rbp
 1f2:	48 89 e5             	mov    %rsp,%rbp
 1f5:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 1f9:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 1fd:	c7 00 48 89 e0 c2    	movl   $0xc2e08948,(%rax)
 203:	90                   	nop
 204:	5d                   	pop    %rbp
 205:	c3                   	retq   

0000000000000206 <addval_106>:
 206:	f3 0f 1e fa          	endbr64 
 20a:	55                   	push   %rbp
 20b:	48 89 e5             	mov    %rsp,%rbp
 20e:	89 7d fc             	mov    %edi,-0x4(%rbp)
 211:	8b 45 fc             	mov    -0x4(%rbp),%eax
 214:	2d 75 31 3c 5b       	sub    $0x5b3c3175,%eax
 219:	5d                   	pop    %rbp
 21a:	c3                   	retq   

000000000000021b <getval_479>:
 21b:	f3 0f 1e fa          	endbr64 
 21f:	55                   	push   %rbp
 220:	48 89 e5             	mov    %rsp,%rbp
 223:	b8 89 ce 90 c3       	mov    $0xc390ce89,%eax
 228:	5d                   	pop    %rbp
 229:	c3                   	retq   

000000000000022a <setval_245>:
 22a:	f3 0f 1e fa          	endbr64 
 22e:	55                   	push   %rbp
 22f:	48 89 e5             	mov    %rsp,%rbp
 232:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 236:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 23a:	c7 00 48 89 e0 c3    	movl   $0xc3e08948,(%rax)
 240:	90                   	nop
 241:	5d                   	pop    %rbp
 242:	c3                   	retq   

0000000000000243 <getval_150>:
 243:	f3 0f 1e fa          	endbr64 
 247:	55                   	push   %rbp
 248:	48 89 e5             	mov    %rsp,%rbp
 24b:	b8 09 d1 20 c0       	mov    $0xc020d109,%eax
 250:	5d                   	pop    %rbp
 251:	c3                   	retq   

0000000000000252 <getval_468>:
 252:	f3 0f 1e fa          	endbr64 
 256:	55                   	push   %rbp
 257:	48 89 e5             	mov    %rsp,%rbp
 25a:	b8 09 ce 84 db       	mov    $0xdb84ce09,%eax
 25f:	5d                   	pop    %rbp
 260:	c3                   	retq   

0000000000000261 <getval_171>:
 261:	f3 0f 1e fa          	endbr64 
 265:	55                   	push   %rbp
 266:	48 89 e5             	mov    %rsp,%rbp
 269:	b8 48 89 e0 94       	mov    $0x94e08948,%eax
 26e:	5d                   	pop    %rbp
 26f:	c3                   	retq   

0000000000000270 <setval_289>:
 270:	f3 0f 1e fa          	endbr64 
 274:	55                   	push   %rbp
 275:	48 89 e5             	mov    %rsp,%rbp
 278:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 27c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 280:	c7 00 89 d1 30 d2    	movl   $0xd230d189,(%rax)
 286:	90                   	nop
 287:	5d                   	pop    %rbp
 288:	c3                   	retq   

0000000000000289 <setval_153>:
 289:	f3 0f 1e fa          	endbr64 
 28d:	55                   	push   %rbp
 28e:	48 89 e5             	mov    %rsp,%rbp
 291:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 295:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 299:	c7 00 89 c2 20 c9    	movl   $0xc920c289,(%rax)
 29f:	90                   	nop
 2a0:	5d                   	pop    %rbp
 2a1:	c3                   	retq   

00000000000002a2 <setval_481>:
 2a2:	f3 0f 1e fa          	endbr64 
 2a6:	55                   	push   %rbp
 2a7:	48 89 e5             	mov    %rsp,%rbp
 2aa:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 2ae:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 2b2:	c7 00 48 a9 e0 c3    	movl   $0xc3e0a948,(%rax)
 2b8:	90                   	nop
 2b9:	5d                   	pop    %rbp
 2ba:	c3                   	retq   

00000000000002bb <setval_415>:
 2bb:	f3 0f 1e fa          	endbr64 
 2bf:	55                   	push   %rbp
 2c0:	48 89 e5             	mov    %rsp,%rbp
 2c3:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 2c7:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 2cb:	c7 00 81 ce 38 db    	movl   $0xdb38ce81,(%rax)
 2d1:	90                   	nop
 2d2:	5d                   	pop    %rbp
 2d3:	c3                   	retq   

00000000000002d4 <getval_339>:
 2d4:	f3 0f 1e fa          	endbr64 
 2d8:	55                   	push   %rbp
 2d9:	48 89 e5             	mov    %rsp,%rbp
 2dc:	b8 89 c2 00 c0       	mov    $0xc000c289,%eax
 2e1:	5d                   	pop    %rbp
 2e2:	c3                   	retq   

00000000000002e3 <setval_183>:
 2e3:	f3 0f 1e fa          	endbr64 
 2e7:	55                   	push   %rbp
 2e8:	48 89 e5             	mov    %rsp,%rbp
 2eb:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 2ef:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 2f3:	c7 00 ea 89 d1 c3    	movl   $0xc3d189ea,(%rax)
 2f9:	90                   	nop
 2fa:	5d                   	pop    %rbp
 2fb:	c3                   	retq   

00000000000002fc <setval_120>:
 2fc:	f3 0f 1e fa          	endbr64 
 300:	55                   	push   %rbp
 301:	48 89 e5             	mov    %rsp,%rbp
 304:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 308:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 30c:	c7 00 48 89 e0 c2    	movl   $0xc2e08948,(%rax)
 312:	90                   	nop
 313:	5d                   	pop    %rbp
 314:	c3                   	retq   

0000000000000315 <setval_359>:
 315:	f3 0f 1e fa          	endbr64 
 319:	55                   	push   %rbp
 31a:	48 89 e5             	mov    %rsp,%rbp
 31d:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 321:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 325:	c7 00 55 89 c2 90    	movl   $0x90c28955,(%rax)
 32b:	90                   	nop
 32c:	5d                   	pop    %rbp
 32d:	c3                   	retq   

000000000000032e <addval_428>:
 32e:	f3 0f 1e fa          	endbr64 
 332:	55                   	push   %rbp
 333:	48 89 e5             	mov    %rsp,%rbp
 336:	89 7d fc             	mov    %edi,-0x4(%rbp)
 339:	8b 45 fc             	mov    -0x4(%rbp),%eax
 33c:	2d 77 31 b7 2d       	sub    $0x2db73177,%eax
 341:	5d                   	pop    %rbp
 342:	c3                   	retq   

0000000000000343 <setval_316>:
 343:	f3 0f 1e fa          	endbr64 
 347:	55                   	push   %rbp
 348:	48 89 e5             	mov    %rsp,%rbp
 34b:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 34f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 353:	c7 00 89 d1 c4 c0    	movl   $0xc0c4d189,(%rax)
 359:	90                   	nop
 35a:	5d                   	pop    %rbp
 35b:	c3                   	retq   

000000000000035c <getval_478>:
 35c:	f3 0f 1e fa          	endbr64 
 360:	55                   	push   %rbp
 361:	48 89 e5             	mov    %rsp,%rbp
 364:	b8 48 89 e0 c7       	mov    $0xc7e08948,%eax
 369:	5d                   	pop    %rbp
 36a:	c3                   	retq   

000000000000036b <end_farm>:
 36b:	f3 0f 1e fa          	endbr64 
 36f:	55                   	push   %rbp
 370:	48 89 e5             	mov    %rsp,%rbp
 373:	b8 01 00 00 00       	mov    $0x1,%eax
 378:	5d                   	pop    %rbp
 379:	c3                   	retq   
