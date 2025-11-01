#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
uint8_t PC = 0;
uint8_t R[4];
// 01 xx xx rs1: out rs1
uint8_t M[16] = {0b10000000, 0b10010000, 0b10100000, 0b10110001, 0b00010111,
                 0b00101001, 0b11010001, 0b01000010, 0b11100011};

void inst_cycle();

void inst_cycle() {
  uint8_t code = M[PC];

  uint8_t op = (code >> 6) & 0b11;

  uint8_t rd, rs1, rs2, imm, addr;
  addr = PC + 1;
  switch (op) {
  case 0b00:
    rd = (code >> 4) & 0b11;
    rs1 = (code >> 2) & 0b11;
    rs2 = code & 0b11;
    R[rd] = R[rs1] + R[rs2];
    break;
  case 0b10:
    rd = (code >> 4) & 0b11;
    imm = code & 0b1111;
    R[rd] = imm;
    break;
  case 0b01:
    rs1 = code & 0b11;
    printf("R[%d] = %d\n", rs1, R[rs1]);
    break;
  case 0b11:
    rs2 = code & 0b11;
    if (R[0] != R[rs2])
      addr = (code >> 2) & 0b1111;
    break;
  default:
    printf("未定义的指令！\n");
  }
  PC = addr;
}

int main(int argc, char *argv[]) {
  int num = 10;
  if (argc == 2) {
    num = atoi(argv[1]);
  } else {
    printf("Use default n = 10\n");
  }
  if (num > 15) {
    printf("Overflow!, Use n = %d\n", num & 0xf);
  }
  M[0] |= (num & 0xf);
  while (PC != 8) {
    inst_cycle();
  }
  // printf("sum from 1 to %d = %d", num, R[2]);
  return 0;
}
