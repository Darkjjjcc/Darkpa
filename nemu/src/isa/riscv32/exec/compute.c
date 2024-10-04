#include "cpu/exec.h"

// PA2.1_1: lui
make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

// PA2.1_2: auipc
make_EHelper(auipc) {
  rtl_add(&id_dest->val, &cpu.pc, &id_src->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(auipc);
}

// // PA2.1_3 addi
// // 备注：这里是为了PA2.1的结果迅速得出，后面重构I型指令时非常有可能会删除这个函数
// make_EHelper(addi) {
//   rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
//   rtl_sr(id_dest->reg, &id_dest->val, 4);
//   print_asm_template3(addi);
// }

// PA2.2 I-type instructions
make_EHelper(I_ir_4) {
  switch (decinfo.isa.instr.funct3) {
    case 0b000: // addi
      rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(addi);
      break;
    case 0b010: // slti
      id_dest->val = (int32_t)id_src->val < (int32_t)id_src2->val;
      print_asm_template3(slti);
      break;
    case 0b011: // sltiu
      id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
      print_asm_template3(sltiu);
      break;
    case 0b100: // xori
      rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(xori);
      break;
    case 0b110: // ori
      rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(ori);
      break;
    case 0b111: // andi
      rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(andi);
      break;
    case 0b001: // slli
      rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(slli);
      break;
    case 0b101:
      if((decinfo.isa.instr.funct7) == 0b0000000) // srli
        {rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);print_asm_template3(srli);}
      else // srai
        {rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);print_asm_template3(srai);}
      break;
    default:
      assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

// // PA2.2 R-type instructions
// make_EHelper(R_opcode_c) {
//   switch (decinfo.isa.instr.funct7) {
//     case 0b0000000:
//       switch (decinfo.isa.instr.funct3) {
//         case 0b000: // add
//           rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
//           print_asm_template3(add);
//           break;
//         case 0b001: // sll
//           rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
//           print_asm_template3(sll);
//           break;
//         case 0b010: // slt
//           id_dest->val = (int32_t)id_src->val < (int32_t)id_src2->val;
//           print_asm_template3(slt);
//           break;
//         case 0b011: // sltu
//           id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
//           print_asm_template3(sltu);
//           break;
//         case 0b100: // xor
//           rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
//           print_asm_template3(xor);
//           break;
//         case 0b101: // srl
//           rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
//           print_asm_template3(srl);
//           break;
//         case 0b110: // or
//           rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
//           print_asm_template3(or);
//           break;
//         case 0b111: // and
//           rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
//           print_asm_template3(and);
//           break;
//         default:
//           assert(0);
//       }
//       break;
//     case 0b0100000: // sub
//       rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
//       print_asm_template3(sub);
//       break;
//     default:
//       assert(0);
//   }
//   rtl_sr(id_dest->reg, &id_dest->val, 4);
// }

make_EHelper(R_opcode_c){
  switch (decinfo.isa.instr.funct3){
  case 0b000: {
    if(decinfo.isa.instr.funct7 == 0x00){       // add
      rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(add);
      
    }
    else if(decinfo.isa.instr.funct7 == 0x20){  // sub
      rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(sub);
    }
    else{                                       // mul
      rtl_imul_lo(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mul);
    }
    break;
  }
  case 0b001: {
    if(decinfo.isa.instr.funct7 == 0x00){       // sll
      rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(sll);

    }
    else{                                       // mulh
      rtl_imul_hi(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mulh);
    }
    break;
  }
  case 0b010: {
    if(decinfo.isa.instr.funct7 == 0x00){       // slt
      id_dest->val = (signed)id_src->val < (signed)id_src2->val;
      print_asm_template3(slt);
    }
    else{ // mulhsu
      TODO();
    }
    break;
  }
  case 0b011: {
    if(decinfo.isa.instr.funct7 == 0x00){       // sltu
      id_dest->val = (unsigned)id_src->val < (unsigned)id_src2->val;
      print_asm_template3(sltu);
    }
    else{                                       // mulhu
      TODO();
    }
    break;
  }
  case 0b100: {
  if(decinfo.isa.instr.funct7 == 0x00){         // xor
      rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(xor);
    }
  else{                                         // div
      rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(div);
    }
    break;
  }
  case 0b101: {
    if(decinfo.isa.instr.funct7 == 0x00){       // srl
      rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(srl);
    }
    else if(decinfo.isa.instr.funct7 == 0x20){  // sra
      rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(sra);
    }
    else{                                       // divu
      rtl_div_q(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(divu);
    }
    break;
  }
  case 0b110: {
    if(decinfo.isa.instr.funct7 == 0x00){       // or
      rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(or);
    }
    else{                                       // rem
      rtl_idiv_r(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(rem);
    }
    break;
  }
case 0b111: {                                       
    if(decinfo.isa.instr.funct7 == 0x00){       // and
      rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(and);
    }
    else{                                       // remu
      rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(remu);
    }
    break;
  }
  default: break;
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}