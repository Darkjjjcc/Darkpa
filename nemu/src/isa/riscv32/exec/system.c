#include "cpu/exec.h"

extern void raise_intr(uint32_t NO, vaddr_t epc);
int32_t read_csr(int scr_id){
    switch (scr_id)
    {
    case 0x100: return decinfo.isa.sstatus; break;
    case 0x105: return decinfo.isa.stvec;   break;
    case 0x141: return decinfo.isa.sepc;    break;
    case 0x142: return decinfo.isa.scause;  break;
    default:
        assert("Unkown csr_id");
    }
}

void write_csr(int scr_id, int32_t val){
    switch (scr_id)
    {
    case 0x100: decinfo.isa.sstatus = val; break;
    case 0x105: decinfo.isa.stvec = val;   break;
    case 0x141: decinfo.isa.sepc = val;    break;
    case 0x142: decinfo.isa.scause = val;  break;
    default:
        assert("Unkown csr_id");
    }
}

make_EHelper(syscall){
    Instr instr = decinfo.isa.instr; 
    switch(decinfo.isa.instr.funct3){
        case 0b000:{
            if(decinfo.isa.instr.funct7 == 0b0000000){      //ecall
                raise_intr(reg_l(17), decinfo.seq_pc);
            }
            else if(decinfo.isa.instr.funct7 == 0b0001000){ //sret
                decinfo.jmp_pc = decinfo.isa.sepc+4;
                rtl_j(decinfo.jmp_pc);
            }
            }
            break;
        case 0b001:{                                        //csrrw
            int32_t val = read_csr(instr.csr);
            write_csr(instr.csr, id_src->val);
            rtl_sr(id_dest->reg, &val, 4);
            print_asm_template3(csrrw);
            break;
        }
        case 0b010:{                                        //csrrs
            int32_t val = read_csr(instr.csr);
            write_csr(instr.csr, val | id_src->val);
            rtl_sr(id_dest->reg, &val, 4);
            print_asm_template3(csrrs);
            break;
        }
        default:
            assert(0);
    }
    
}