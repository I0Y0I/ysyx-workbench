import debug
from reg import reg_list


def op_add(memory, reg, ins):
    rs1 = reg.read(ins.rs1)
    rs2 = reg.read(ins.rs2)
    rd = (rs1 + rs2) & 0xFFFFFFFF

    if debug.DEBUG:
        print(f"0x{rs1:08X}({reg_list[ins.rs1]}) + 0x{rs2:08X}({reg_list[ins.rs2]}) = 0x{rd:08X}, write to {reg_list[ins.rd]}")

    reg.write(ins.rd, rd)
    reg.add_pc()

