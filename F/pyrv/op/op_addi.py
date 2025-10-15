import debug
from reg import reg_list


def op_addi(memory, reg, ins):
    rs1 = reg.read(ins.rs1)
    imm = ins.imm
    rd = (rs1 + imm) & 0xFFFFFFFF

    if debug.DEBUG:
        print(f"0x{rs1:08X}({reg_list[ins.rs1]}) + 0x{imm:08X} = 0x{rd:08X}, write to {reg_list[ins.rd]}")

    reg.write(ins.rd, rd)
    reg.add_pc()

