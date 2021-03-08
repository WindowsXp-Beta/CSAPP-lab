/* Instruction set simulator for Y64 Architecture */

#include <stdio.h>
#include <stdlib.h>

#include "y64sim.h"

#define err_print(_s, _a ...) \
    fprintf(stdout, _s"\n", _a);


typedef enum {STAT_AOK, STAT_HLT, STAT_ADR, STAT_INS} stat_t;

char *stat_names[] = { "AOK", "HLT", "ADR", "INS" };

char *stat_name(stat_t e)
{
    if (e < STAT_AOK || e > STAT_INS)
        return "Invalid Status";
    return stat_names[e];
}

char *cc_names[8] = {
    "Z=0 S=0 O=0",
    "Z=0 S=0 O=1",
    "Z=0 S=1 O=0",
    "Z=0 S=1 O=1",
    "Z=1 S=0 O=0",
    "Z=1 S=0 O=1",
    "Z=1 S=1 O=0",
    "Z=1 S=1 O=1" };

char *cc_name(cc_t c)
{
    int ci = c;
    if (ci < 0 || ci > 7)
        return "???????????";
    else
        return cc_names[c];
}

bool_t get_byte_val(mem_t *m, long_t addr, byte_t *dest) 
//从内存 m 中获取addr处的 一个unsigned byte。data是unsigned byte数组。存入 dest 中。
{
    if (addr < 0 || addr >= m->len)
        return FALSE;
    *dest = m->data[addr];
    return TRUE;
}

bool_t get_long_val(mem_t *m, long_t addr, long_t *dest)
//从内存 m 中获取addr处的 八个unsigned byte（即一个long）。存入 dest 中。
{
    int i;
    long_t val;
    if (addr < 0 || addr + 8 > m->len)
	    return FALSE;
    val = 0;
    for (i = 0; i < 8; i++)
	    val = val | ((long_t)m->data[addr+i])<<(8*i); // << -> | -> = 
    *dest = val;
    return TRUE;
}

bool_t set_byte_val(mem_t *m, long_t addr, byte_t val)
{
    if (addr < 0 || addr >= m->len)
	    return FALSE;
    m->data[addr] = val;
    return TRUE;
}

bool_t set_long_val(mem_t *m, long_t addr, long_t val)
{
    int i;
    if (addr < 0 || addr + 8 > m->len)
	    return FALSE;
    for (i = 0; i < 8; i++) {
    	m->data[addr+i] = val & 0xFF;
    	val >>= 8;
    }
    return TRUE;
}

mem_t *init_mem(int len)
{
    mem_t *m = (mem_t *)malloc(sizeof(mem_t));
    len = ((len+BLK_SIZE-1)/BLK_SIZE)*BLK_SIZE; // n<(len/32)<=n+1 would become n+1
    m->len = len;
    m->data = (byte_t *)calloc(len, 1);

    return m;
}

void free_mem(mem_t *m)
{
    free((void *) m->data);
    free((void *) m);
}

mem_t *dup_mem(mem_t *oldm)
//将老内存复制到新内存
{
    mem_t *newm = init_mem(oldm->len);
    memcpy(newm->data, oldm->data, oldm->len);
    return newm;
}

bool_t diff_mem(mem_t *oldm, mem_t *newm, FILE *outfile)
{
    long_t pos;
    int len = oldm->len;
    bool_t diff = FALSE;
    
    if (newm->len < len)
	    len = newm->len;
    
    for (pos = 0; (!diff || outfile) && pos < len; pos += 8) {
        long_t ov = 0;  long_t nv = 0;
        get_long_val(oldm, pos, &ov);
        get_long_val(newm, pos, &nv);
        if (nv != ov) {
            diff = TRUE;
            if (outfile)
                fprintf(outfile, "0x%.16lx:\t0x%.16lx\t0x%.16lx\n", pos, ov, nv);
        }
    }
    return diff;
}


reg_t reg_table[REG_NONE] = {
//ids of registers are from -1(error) to 15(None)
    {"%rax", REG_RAX},//0
    {"%rcx", REG_RCX},//1
    {"%rdx", REG_RDX},
    {"%rbx", REG_RBX},
    {"%rsp", REG_RSP},
    {"%rbp", REG_RBP},
    {"%rsi", REG_RSI},
    {"%rdi", REG_RDI},
    {"%r8",  REG_R8},
    {"%r9",  REG_R9},
    {"%r10", REG_R10},
    {"%r11", REG_R11},
    {"%r12", REG_R12},
    {"%r13", REG_R13},
    {"%r14", REG_R14}//14
};

long_t get_reg_val(mem_t *r, regid_t id)
{
    long_t val = 0;
    if (id >= REG_NONE)
        return 0;
    get_long_val(r, id*8, &val); //寄存器所在的内存用r表示，一个寄存器存8位
    return val;
}

void set_reg_val(mem_t *r, regid_t id, long_t val)
{
    if (id < REG_NONE)
        set_long_val(r, id*8, val);
}

mem_t *init_reg()
//初始化寄存器所在的内存
{
    return init_mem(REG_SIZE);
}

void free_reg(mem_t *r)
{
    free_mem(r);
}

mem_t *dup_reg(mem_t *oldr)
//迁移
{
    return dup_mem(oldr);
}

bool_t diff_reg(mem_t *oldr, mem_t *newr, FILE *outfile)
{
    long_t pos;
    int len = oldr->len;
    bool_t diff = FALSE;
    
    if (newr->len < len)
	    len = newr->len;
    
    for (pos = 0; (!diff || outfile) && pos < len; pos += 8) {
        long_t ov = 0;
        long_t nv = 0;
        get_long_val(oldr, pos, &ov);
        get_long_val(newr, pos, &nv);
        if (nv != ov) {
            diff = TRUE;
            if (outfile)
                fprintf(outfile, "%s:\t0x%.16lx\t0x%.16lx\n",
                        reg_table[pos/8].name, ov, nv);
        }
    }
    return diff;
}

/* create an y64 image with registers and memory */
y64sim_t *new_y64sim(int slen)
{
    y64sim_t *sim = (y64sim_t*)malloc(sizeof(y64sim_t));
    sim->pc = 0;
    sim->r = init_reg();
    sim->m = init_mem(slen);
    sim->cc = DEFAULT_CC;  //cc is an unsigned char
    return sim;
}

void free_y64sim(y64sim_t *sim)
{
    free_reg(sim->r);
    free_mem(sim->m);
    free((void *) sim);
}

/* load binary code and data from file to memory image */
int load_binfile(mem_t *m, FILE *f)
{
    int flen;

    clearerr(f);
    flen = fread(m->data, sizeof(byte_t), m->len, f);
    if (ferror(f)) {
        err_print("fread() failed (0x%x)", flen);
        return -1;
    }
    if (!feof(f)) {
        err_print("too large memory footprint (0x%x)", flen);
        return -1;
    }
    return 0;
}

/*
 * compute_alu: do ALU operations 
 * args
 *     op: operations (A_ADD, A_SUB, A_AND, A_XOR)
 *     argA: the first argument 
 *     argB: the second argument
 *
 * return
 *     val: the result of operation on argA and argB
 */
long_t compute_alu(alu_t op, long_t argA, long_t argB)// 0 is add 1 is sub 2 is and 3 is xor 4 is none
{
    long_t val = 0;
    switch (op)
    {
    case 0:
        val = argB + argA;
        break;
    case 1:
        val = argB - argA;
        break;
    case 2:
        val = argB & argA;
        break;
    case 3:
        val = argB ^ argA;
        break;
    case 4:
        
        break;
    default:

        break;
    }
    return val;
}

/*
 * compute_cc: modify condition codes according to operations 
 * args
 *     op: operations (A_ADD, A_SUB, A_AND, A_XOR)
 *     argA: the first argument 
 *     argB: the second argument
 *     val: the result of operation on argA and argB
 *
 * return
 *     PACK_CC: the final condition codes
 */
cc_t compute_cc(alu_t op, long_t argA, long_t argB, long_t val)
{
    bool_t zero = (val == 0);
    bool_t sign = ((int)val < 0);
    bool_t ovf = FALSE;
    if ( op == 0 || op == 1)
    {
        if ((argA < 0)==(argB < 0) && (val < 0 != argA < 0))
        {
            ovf = TRUE;
        }
    }
    return PACK_CC(zero,sign,ovf);  
}

/*
 * cond_doit: whether do (mov or jmp) it?  
 * args
 *     PACK_CC: the current condition codes
 *     cond: conditions (C_YES, C_LE, C_L, C_E, C_NE, C_GE, C_G)
 *
 * return
 *     TRUE: do it
 *     FALSE: not do it
 */
bool_t cond_doit(cc_t cc, cond_t cond) 
{
    bool_t doit = FALSE;
    bool_t zero = cc >> 2 & 0x1;
    bool_t sign = cc >> 1 & 0x1;
    bool_t ovf = cc & 0x1;
    switch (cond)
    {
    case C_YES:
        doit = TRUE;
        break;
    case C_LE:
        doit = (sign ^ ovf) | zero;
        break;
    case C_L:
        doit = (sign ^ ovf);
        break;
    case C_E:
        doit = zero;
        break;
    case C_NE:
        doit = ~ zero;
        break;
    case C_GE:
        doit = ~ (sign ^ ovf);
        break;
    case C_G:
        doit = ~ (sign ^ ovf) & ~zero;
        break;
    }
    return doit;
}

/* 
 * nexti: execute single instruction and return status.
 * args
 *     sim: the y64 image with PC, register and memory
 *
 * return
 *     STAT_AOK: continue
 *     STAT_HLT: halt
 *     STAT_ADR: invalid instruction address
 *     STAT_INS: invalid instruction, register id, data address, stack address, ...
 */
stat_t nexti(y64sim_t *sim)
{
    byte_t codefun = 0; /* 1 byte */
    itype_t icode; //指令
    alu_t ifun; 
    //运算操作符  虽然 alu_t是op的枚举类型，但在执行的时候只需要把它当成一个unsigned int即可
    //通过阅读汇编代码，发现枚举类型用int来处理的，因此大于4也无所谓。
    long_t next_pc = sim->pc;
    
    /* get code and function （1 byte) */
    if (!get_byte_val(sim->m, next_pc, &codefun)) { //指令地址不合法
        err_print("PC = 0x%lx, Invalid instruction address", sim->pc);
        return STAT_ADR;
    }
    icode = GET_ICODE(codefun); //取字节高四位表示指令种类
    ifun = GET_FUN(codefun); //取字节低四位表示指令功能
    next_pc++;//此时pc位于第二字节，即寄存器的字节

    /* get registers if needed (1 byte) */
    regid_t rA, rB;
    bool_t reg_exist = TRUE;
    if (icode == I_RRMOVQ || icode == I_IRMOVQ || icode == I_RMMOVQ || icode == I_ALU || icode == I_PUSHQ || icode == I_POPQ)
    {
            if (!get_byte_val(sim->m, next_pc, &codefun)) {
                reg_exist = FALSE;
        }
            else {
                rA = GET_REGA(codefun);
                rB = GET_REGB(codefun);
                next_pc++;
        }      
    }
    /* get immediate if needed (8 bytes) */
    long_t immediate;
    bool_t im_exist = TRUE;
    if (icode == I_IRMOVQ || icode == I_RMMOVQ || icode == I_MRMOVQ || icode == I_JMP || icode == I_CALL)
    {
        if (!get_long_val(sim->m, next_pc, &immediate)) {
            im_exist = FALSE;
        }
        else {
            next_pc+=8;
        }
    }
    /* execute the instruction*/
    switch (icode) {
      case I_HALT: /* 0:0 */
	    return STAT_HLT;
	    break;
      case I_NOP: /* 1:0 */
    	sim->pc = next_pc;
    	break;
      case I_RRMOVQ:  /* 2:x regA:regB */
        {
            if (!reg_exist || ((rB > 14)||(rB < 0)) || ((rA > 14)||(rA < 0)))
            {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS; 
            }
            
            break;
        }
      case I_IRMOVQ: /* 3:0 F:regB imm */ //checked!!!
        {
            if (!reg_exist || !im_exist || rA != 15 || ((rB > 14) || (rB < 0))) {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS;
            }
            set_reg_val(sim->r, rB, immediate);
            sim->pc = next_pc;
            break;
        }
      case I_RMMOVQ: /* 4:0 regA:regB imm */ //register -> memory //checked!!!
        {
            if (!reg_exist || !im_exist || ((rA > 14) || (rA < 0)) || ((rB > 14) || (rB < 0))) {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS;
            }
            long_t val_A = get_reg_val(sim->r, rA);
            long_t val_B = get_reg_val(sim->r, rB);
            long_t dest_M = val_B + immediate;
            if(!set_long_val(sim->m, dest_M, val_A)) return STAT_ADR;
            sim->pc = next_pc;
            break;
        }
      case I_MRMOVQ: /* 5:0 regB:regA imm */ //memory -> register
        {
            if (!reg_exist || !im_exist || ((rA > 14) || (rA < 0)) || ((rB > 14) || (rB < 0))) {
                //err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                //return STAT_INS;
            }
            long_t val_B = get_reg_val(sim->r, rB);
            long_t new_val_A;
            long_t source = val_B + immediate;
            if (!get_long_val(sim->m, source, &new_val_A)) {
                err_print("PC = 0x%lx, Invalid data address 0x%.16lx", sim->pc, source);
                return STAT_ADR;
           }
           set_reg_val(sim->r, rA, new_val_A);
           sim->pc = next_pc;
           break;
        }
      case I_ALU: /* 6:x regA:regB */ //checked!!!
        { 
            if (!reg_exist || ((rA > 14) || (rA < 0)) || ((rB > 14) || (rB < 0))) {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS;
            } 
            long_t val_A = get_reg_val(sim->r, rA);
            long_t val_B = get_reg_val(sim->r, rB);
            long_t new_val_B = compute_alu(ifun, val_A, val_B);
            sim->cc = compute_cc(ifun, val_A, val_B, new_val_B);
            set_reg_val(sim->r, rB, new_val_B);
            sim->pc = next_pc;
            break;
        }
      case I_JMP: /* 7:x imm */
        {
            if (!im_exist) {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS;
            }
            break;
        }      
      case I_CALL: /* 8:x imm */
        {
           if (!im_exist) {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS;
            }
        } 
      case I_RET: /* 9:0 */
      case I_PUSHQ: /* A:0 regA:F */ //checked!!!
        {
           if (!reg_exist || ((rA > 14) || (rA < 0)) || rB != 15) {
                err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
                return STAT_INS;
            }
            long_t val_A = get_reg_val(sim->r, rA);
            long_t stack = get_reg_val(sim->r, REG_RSP);
            stack-=8;
            set_reg_val(sim->r, REG_RSP, stack);// Firstly write to stack then check stack address!!!
            if (!set_long_val(sim->m, stack, val_A)) {
                err_print("PC = 0x%lx, Invalid stack address 0x%.16lx", sim->pc, stack);  
                return STAT_ADR;
            }
            sim->pc = next_pc;
            break;
        }
      case I_POPQ: /* B:0 regA:F */
    	return STAT_INS; /* unsupported now, replace it with your implementation */
    	break;
      default:
    	err_print("PC = 0x%lx, Invalid instruction %.2x", sim->pc, codefun);
    	return STAT_INS;
    }
    
    return STAT_AOK;
}

void usage(char *pname)
{
    printf("Usage: %s file.bin [max_steps]\n", pname);
    exit(0);
}

int main(int argc, char *argv[])
{
    FILE *binfile;
    int max_steps = MAX_STEP;
    y64sim_t *sim;
    mem_t *saver, *savem;
    int step = 0;
    stat_t e = STAT_AOK;

    if (argc < 2 || argc > 3)
        usage(argv[0]);

    /* set max steps */
    if (argc > 2)
        max_steps = atoi(argv[2]);

    /* load binary file to memory */
    if (strcmp(argv[1]+(strlen(argv[1])-4), ".bin"))
        usage(argv[0]); /* only support *.bin file */
    
    binfile = fopen(argv[1], "rb");
    if (!binfile) {
        err_print("Can't open binary file '%s'", argv[1]);
        exit(1);
    }

    sim = new_y64sim(MEM_SIZE);
    if (load_binfile(sim->m, binfile) < 0) {
        err_print("Failed to load binary file '%s'", argv[1]);
        free_y64sim(sim);
        exit(1);
    }
    fclose(binfile);

    /* save initial register and memory stat */
    saver = dup_reg(sim->r);
    savem = dup_mem(sim->m);

    /* execute binary code step-by-step */
    for (step = 0; step < max_steps && e == STAT_AOK; step++)
        e = nexti(sim);

    /* print final stat of y64sim */
    printf("Stopped in %d steps at PC = 0x%lx.  Status '%s', CC %s\n",
            step, sim->pc, stat_name(e), cc_name(sim->cc));

    printf("Changes to registers:\n");
    diff_reg(saver, sim->r, stdout);

    printf("\nChanges to memory:\n");
    diff_mem(savem, sim->m, stdout);

    free_y64sim(sim);
    free_reg(saver);
    free_mem(savem);

    return 0;
}

;