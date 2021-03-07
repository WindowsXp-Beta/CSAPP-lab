# note of Y86-64 Simulator
## 题目解读
yat 用来检验你的实现是否正确
y64sim-base 模拟器的正确实现
y64asm-base 汇编器的正确实现
.bin files从.ys files中产生，它们用来作为yat检验你正确性的标准
y64-ins-bin 33个测试 .bin文件 每一个对应着一种Y86的指令，注意其中有一些指令是不正确的，你的模拟器需要报错
y64-app-bin 20个测试 .bin文件，每一个对应着简单的汇编程序
检验指令正确性
`./yat -s <ins_name>` `ins_name -> rrmovq`

