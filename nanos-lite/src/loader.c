#include "proc.h"
#include "fs.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  // PA3.2 加载用户程序到正确位置
  // Elf_Ehdr ehdr;
  // ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
  // assert((*(uint32_t *)ehdr.e_ident == 0x464c457f));

  // Elf_Phdr phdr[ehdr.e_phnum];
  // ramdisk_read(phdr, ehdr.e_phoff, sizeof(Elf_Phdr)*ehdr.e_phnum);
  // for (int i = 0; i < ehdr.e_phnum; i++) {
  //   if (phdr[i].p_type == PT_LOAD) {
  //     ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
  //     memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
  //   }
  // }
  // return ehdr.e_entry;
  Elf_Ehdr Ehdr;
  int fd = fs_open(filename, 0, 0);
  fs_lseek(fd, 0, SEEK_SET);
  fs_read(fd, &Ehdr, sizeof(Ehdr));
  //ramdisk_read(&Ehdr, 0, sizeof(Ehdr));
  for(int i = 0; i < Ehdr.e_phnum;i++){
      Elf_Phdr Phdr;
      fs_lseek(fd, Ehdr.e_phoff + i*Ehdr.e_phentsize, SEEK_SET);
      //printf("res:%d\n", res);
      fs_read(fd, &Phdr, sizeof(Phdr));
      //ramdisk_read(&Phdr, Ehdr.e_phoff + i*Ehdr.e_phentsize, sizeof(Phdr));
      if(!(Phdr.p_type & PT_LOAD)){
          continue;
      }
      fs_lseek(fd, Phdr.p_offset, SEEK_SET);
      fs_read(fd, (void*)Phdr.p_vaddr, Phdr.p_filesz);
      for(unsigned int i = Phdr.p_filesz; i < Phdr.p_memsz;i++){
          ((char*)Phdr.p_vaddr)[i] = 0;
      }
      //ramdisk_read((void*)Phdr.p_vaddr, Phdr.p_offset, Phdr.p_filesz);
  }

  return Ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
