# note for lab6
> 这个lab相比以往的lab提供了很多的文件和工具，所以先阅读这些文件的README是大有裨益的。
## Part A
写一些简单的y86-64程序并熟悉y86-64 tools
## Part B
拓展SEQ simulator一条新指令
测试：
1. build一个新的模拟器 通过`make VERSION=full`
2. 首先使用简单的程序比如 asumi.yo 在TTY模式下检测 对比和ISA模拟的结果，如果ISA测试失败了，debug。
3. 重新使用基准程序检测你的解法，一旦模拟器能够正确执行小program了，那你可以进入 /y86-code文件夹里进行自动化检测。
   > `cd ../y86-code; make testssim`
   
   **注意，这些程序没有检查你新加入的指令，它只是保证你没有引入错误。**
4. 一旦你能正确执行这些基准程序，你应该检测额外的regression tests。
   `cd ../ptest; make SIM=../seq/ssim`
   检测除iaddq外的所有指令
   `cd ../ptest; make SIM=../seq/ssim TFLAGS=-i`
   检测你的iaddq的实现
## Part C
lab6的核心，优化y86-64基准程序和处理器设计
### task
修改 **ncopy.ys** 和 **pipe-full.hcl** 使得 **ncopy.ys**运行的尽可能快。
你需要提交这两份文件，并在每份文件的头部注释上，你为什么how和why你修改你的代码。
### coding rules
1. 你的**ncopy.ys**必须能够适用于任意数组规模。
2. 你的**ncopy.ys**必须能够在YIS下正确执行。
3. 你的汇编版本ncopy文件必须小于1000字节，你可以通过提供的脚本来检测
   `unix> ./check-len.pl < ncopy.yo`
4. 你的**pipe-full.hcl**必须正确，通过在`../y86-code`和`../ptest`中的regression tests。
### 测试你的solution
1. `unix> make drivers`会构造两个文件
   - `sdriver.yo`: 用含有4个元素的小数组测试你的ncopy，如果正确，那么会 `halt with 2 in %rax`
   - `ldriver.yo`: 用含有63个元素的大数组测试你的ncopy，如果正确，那么会 `halt with a value of 31 (0x1f)in %rax`
2. 每次修改完**ncopy.ys**后要rebuild drivers。
   `unix> make drivers`
3. 每次修改完**pipe-full.hcl**后要rebuild你的模拟器。
   `unix> make psim VERSION=full`
4. 在GUI模式下用4字节或63字节数组测试你的solution
   `unix> ./psim -g sdriver.yo`
   `unix> ./psim -g ldriver.yo`
5. 在ISA模拟器下测试你的driver file，保证你的**ncopy.ys**在YIS下正确。
   `unix> make drivers`
   `unix> ../misc/yis sdriver.yo`
6. 在ISA模拟器下使用测试不同的数组长度。
   `unix> ./correctness.pl`
7. 如果你长度为K的数组测试结果出错，你可以生成一份该长度的driver file，包含检查代码。
   `unix> ./gen-driver.pl -f ncopy.ys -n K -rc > driver.ys`
   `unix> make driver.yo`
   `unix> ../misc/yis driver.yo`
   程序的返回值 `%rax` 有以下情况
   > 0xaaaa : All tests pass.
   0xbbbb : Incorrect count
   0xcccc : Function ncopy is more than 1000 bytes long.
   0xdddd : Some of the source data was not copied to its destination.
   0xeeee : Some word just before or just after the destination region was corrupted
8. 用额外的regression test检测你的流水线模拟器，
   e.g. 如果你实现了`iaddq`指令，那么
   `unix> (cd ../ptest; make SIM=../pipe/psim TFLAGS=-i)`
9. 用流水线模拟器测试不同长度的数组，你可以在流水线模拟器下做你之前在ISA模拟器下做的测试。
   `unix> ./correctness.pl -p`
10. 评分
    用`./benchmark.pl`测试不同长度的数组并返回平均CPE
    S =  0 , c > 10.5
    S =  20 · (10.5 − c) , 7.50 ≤ c ≤ 10.50
    S = 60 , c < 7.50
    **attention:该脚本不检查正确性，先检查正确性再测速**
## 文件结构
1. misc/
   y86 汇编器 yas 指令模拟器 yis isa.c 处理器模拟器使用-t选项检查和ISA的结果
2. seq
   seq和seq+模拟器的代码，包括HCL和homework
3. pipe/
   流水线模拟器的代码
4. ISA 模拟器，即YIS
   就是之前写的lab4，模拟机器指令的执行。一条一条指令执行。
   > CSAPP_zh p253
   YIS 模拟Y86-64代码的执行而不用试图模拟任何具体处理器的行为。