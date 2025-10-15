#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <am.h>
#include <klib-macros.h>
uint32_t R[16];
uint8_t M[1 << 30];
uint32_t PC;
char N[16][5] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
                 "s0",   "s1", "a0", "a1", "a2", "a3", "a4", "a5"};

uint32_t display_buffer[256][256];

void reset() {
  size_t i;
  for (i = 0; i < 16; i++)
    R[i] = 0;
  PC = 0;
}

uint32_t read_word(uint32_t a) {
  return (M[a + 3] << 24) | (M[a + 2] << 16) | (M[a + 1] << 8) | M[a];
}

void write_word(uint32_t a, uint32_t w) {
  M[a + 3] = w >> 24;
  M[a + 2] = (w >> 16) & 0xFF;
  M[a + 1] = (w >> 8) & 0xFF;
  M[a] = w & 0xFF;
}

void write_reg(uint32_t r, uint32_t v) {
  if (r)
    R[r] = v;
}

uint32_t sign_ext(uint32_t value, uint32_t bits) {
  uint32_t sign_bit = 1 << (bits - 1);
  if (value & sign_bit)
    return value | ((~0) << bits);
  else
    return value;
}

uint32_t load(const char *path) {
  FILE *fp;
  uint32_t buf;
  fp = fopen(path, "rb");
  if (fp == NULL) {
    perror("Failed to open file.\n");
    return 0;
  }

  uint32_t i = 0;
  while (fread(&buf, sizeof(uint32_t), 1, fp) == 1) {
    // printf("0x%08X\n", buf);
    write_word(i, buf);
    i += 4;
  }

  fclose(fp);
  printf("Load %d instruction.\n", i >> 2);
  return i;
}

void op_add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  uint32_t reg = R[rs1] + R[rs2];
  printf("add\t%08X(%s) + 0x%08X(%s) = 0x%08X, write to %s\n", R[rs1], N[rs1],
         R[rs2], N[rs2], reg, N[rd]);
  write_reg(rd, reg);
}

void op_addi(uint32_t rd, uint32_t rs1, uint32_t imm) {
  uint32_t reg = R[rs1] + imm;
  printf("addi\t0x%08X(%s) + 0x%08X(imm) = 0x%08X, write to %s\n", R[rs1],
         N[rs1], imm, reg, N[rd]);
  write_reg(rd, reg);
}

void op_jalr(uint32_t rd, uint32_t rs1, uint32_t imm) {
  uint32_t next_pc = R[rs1] + imm;
  uint32_t reg = PC;
  printf("jarl\t0x%08X(%s) + 0x%08X(imm) = 0x%08X, set PC to 0x%08X, write "
         "0x%08X to "
         "%s\n",
         R[rs1], N[rs1], imm, next_pc, next_pc, reg, N[rd]);
  PC = next_pc;
  write_reg(rd, reg);
}

void op_lbu(uint32_t rd, uint32_t rs1, uint32_t imm) {
  uint32_t addr = R[rs1] + imm;
  uint32_t reg = M[addr];
  printf("lbu\t0x%08X(%s) + 0x%08X(imm) = 0x%08X, MEM[%08X] is 0x%08X, write "
         "to %s\n",
         R[rs1], N[rs1], imm, addr, addr, reg, N[rd]);
  write_reg(rd, reg);
}

void op_lui(uint32_t rd, uint32_t imm) {
  printf("lui\twrite 0x%08X to %s\n", imm, N[rd]);
  write_reg(rd, imm);
}

void op_lw(uint32_t rd, uint32_t rs1, uint32_t imm) {
  uint32_t addr = R[rs1] + imm;
  uint32_t reg = read_word(addr);
  printf("lw\t0x%08X(%s) + 0x%08X(imm) = 0x%08X, MEM[%08X] is 0x%08X, write to "
         "%s\n",
         R[rs1], N[rs1], imm, addr, addr, reg, N[rd]);
  write_reg(rd, reg);
}

void op_sb(uint32_t rs1, uint32_t imm, uint32_t rs2) {
  uint32_t addr = R[rs1] + imm;
  printf("sb\t0x%08X(%s) + 0x%08X(imm) = 0x%08X, write 0x%02X to MEM[0x%08X]\n",
         R[rs1], N[rs1], imm, addr, R[rs2], addr);
  M[addr] = R[rs2] & 0xFF;
}

void op_sw(uint32_t rs1, uint32_t imm, uint32_t rs2) {
  uint32_t addr = R[rs1] + imm;
  int x, y;
  printf("sw\t0x%08X(%s) + 0x%08X(imm) = 0x%08X, write 0x%08X to MEM[0x%08X]\n",
         R[rs1], N[rs1], imm, addr, R[rs2], addr);
  if (addr >= 0x20000000 && addr < 0x20040000) {
    x = (addr >> 2) & 0xFF;
    y = (addr >> 10) & 0xFF;
    display_buffer[y][x] = R[rs2];
  } else
    write_word(addr, R[rs2]);
}

int inst_cycle() {
  uint32_t raw, op, rd, funct3, rs1, rs2, imm;
  raw = read_word(PC);
  op = raw & 0x7F;
  rd = (raw >> 7) & 0x1F;
  funct3 = (raw >> 12) & 0x7;
  rs1 = (raw >> 15) & 0x1F;
  rs2 = (raw >> 20) & 0x1F;

  // printf("%X:%08X\n", PC, raw);

  PC = PC + 4;
  switch (op) {
  // add
  case 0b0110011:
    op_add(rd, rs1, rs2);
    break;
  case 0b0010011:
    imm = sign_ext((raw >> 20) & 0xFFF, 12);
    op_addi(rd, rs1, imm);
    break;
  case 0b1100111:
    imm = sign_ext((raw >> 20) & 0xFFF, 12);
    op_jalr(rd, rs1, imm);
    break;
  case 0b0000011:
    imm = sign_ext((raw >> 20) & 0xFFF, 12);
    switch (funct3) {
    case 0b010:
      op_lw(rd, rs1, imm);
      break;
    case 0b100:
      op_lbu(rd, rs1, imm);
      break;
    }
    break;
  case 0b0100011:
    imm = sign_ext((((raw >> 25) & 0x7F) << 5) | ((raw >> 7) & 0x1F), 12);
    switch (funct3) {
    case 0b000:
      op_sb(rs1, imm, rs2);
      break;
    case 0b010:
      op_sw(rs1, imm, rs2);
      break;
    }
    break;
  case 0b0110111:
    imm = raw & 0xFFFFF000;
    op_lui(rd, imm);
    break;
  case 0b1111111:
    return -1;
  default:
    printf("Undefined!!!!!!!!!!!!!!!!!!!!\n");
  }
  return 0;
}

int run() {
  int i = 0;
  while (!inst_cycle()) {
    printf("%d\t%X\t", i++, PC);
  }
  return R[10];
}

void show_display() {
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  io_write(AM_GPU_FBDRAW, (w - 256) >> 1, (h - 256) >> 1, display_buffer, 256,
           256, true);
};

int main() {
  reset();
  if (!load("/home/ioyoi/Documents/code/ysyx/E4/minirvEMU/vga.bin"))
    return -1;
  // mem.bin halt at 0x1220
  // M[0x1220] = 0xFF;
  // sum.bin halt at 0x228
  // M[0x228] = 0xFF;
  // vga.bin halt at 0xdb8
  M[0xdb8] = 0xFF;
  if (run()) {
    printf("return value not zero\n");
    return -1;
  }
  show_display();
  while (1) {
  }
  return 0;
}
