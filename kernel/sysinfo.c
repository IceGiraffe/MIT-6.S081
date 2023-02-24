#include "sysinfo.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "stat.h"
#include "proc.h"


uint64 
sys_sysinfo(void){
    struct proc *p = myproc();
    uint64 addr;
	if (argaddr(0, &addr) < 0)
		return -1;
    // get the retval
    struct sysinfo si;
    si.freemem = 10;
    si.nproc = 10;

    // set the retval
    if (copyout(p->pagetable, addr, (char *)&si, sizeof(struct sysinfo)) < 0)
    {
        return -1;
    }
    return 0;
}