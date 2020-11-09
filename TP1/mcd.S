#ifndef MCD_S
#define MCD_S

#include <sys/regdef.h>
#define SS 8

/* SRA */
#define O_FP 4
#define O_GP 0


.text
.align 2

/* 
unsigned int 
mcd(unsigned int m, unsigned int n)
*/
.globl mcd
.ent mcd
mcd:
	subu sp, sp, SS
	sw   fp, O_FP(sp)
 	sw   gp, O_GP(sp)
 	move fp, sp
	
	bgtu a0,a1,while
/*hacemos el swap */
	move t0,a0
	move a0,a1
	move a1,t0

while:
	beqz a1,return
	divu a0,a1
	move a0,a1
	mfhi a1
	b while	

return: 
	move v0,a0

/* stack unwinding */
	lw    fp, O_FP(sp)
  	lw    gp, O_GP(sp)
  	addiu sp, sp, SS
	
	jr ra
.end mcd

#endif /* MCD_S */