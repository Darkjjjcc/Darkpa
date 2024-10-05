#include "cpu/exec.h"

// pa2.1_4 jal
make_EHelper(jal){
  uint32_t addr = cpu.pc + 4;
  rtl_sr(id_dest->reg, &addr, 4);

  rtl_add(&decinfo.jmp_pc, &cpu.pc, &id_src->val);
  rtl_j(decinfo.jmp_pc);

  print_asm_template2(jal);
}

// pa2.1_5 jalr
make_EHelper(jalr){
  uint32_t addr = cpu.pc + 4;
  rtl_sr(id_dest->reg, &addr, 4);

  decinfo.jmp_pc = (id_src->val+id_src2->val)&(~1);
  rtl_j(decinfo.jmp_pc);

  print_asm_template2(jalr);
}

// PA2.2 B-type instructions
make_EHelper(Branch){
  decinfo.jmp_pc = cpu.pc + id_dest->val;
  switch(decinfo.isa.instr.funct3){
    case 0b000:     // beq||beqz
      rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, decinfo.jmp_pc);
      print_asm_template3(beq);
      break;
    case 0b001:     // bne||bnez
      rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
      print_asm_template3(bne);
      break;
    case 0b100:     // blt||bltz
      rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, decinfo.jmp_pc);
      print_asm_template3(blt);
      break;
    case 0b101:     // bge||bgez
      rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
      print_asm_template3(bge);
      break;
    case 0b110:     // bltu
      rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
      print_asm_template3(bltu);
      break;
    case 0b111:   // bgeu
      rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
      print_asm_template3(bgeu);
      break;
    default:
      assert(0);
      break;
  }
}