import debug
from reg import reg_list


def op_lbu(memory, reg, ins):
    rs1 = reg.read(ins.rs1)
    imm = ins.imm
    addr = (rs1 + imm) & 0xFFFFFFFF
    rd = memory.read(addr, 1) & 0xFFFFFFFF

    if debug.DEBUG:
        print(f"0x{rs1:08X}({reg_list[ins.rs1]}) + 0x{imm:08X} = 0x{addr:08X}, MEM[0x{addr:08X}] is 0x{rd:02X}, write to {reg_list[ins.rd]}")

    reg.write(ins.rd, rd)
    reg.add_pc()
