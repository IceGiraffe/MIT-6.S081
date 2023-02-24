* 根据`entry.S`中的注释，BootLoader将kernel.bin可执行文件加载到内存的0x80000000处
    * 这里的`0x80000000`是物理地址还是虚拟地址，应该是物理地址吧，应该是物理地址
    * The RISC-V starts with paging hardware disabled: virtual addresses map directly to physical addresses.
    * kernel.ld是链接脚本，在该脚本中，指定了代码段起始位置为0x80000000
* 用户态的fork函数，先陷入内核态，然后内核查中断表，之后执行sys_fork()，之后执行内核的fork()
* 数组起始是0, 系统调用号是1开始的