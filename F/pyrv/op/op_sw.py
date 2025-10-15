import debug
from reg import reg_list


def op_sw(memory, reg, ins):
    rs1 = reg.read(ins.rs1)
    imm = ins.imm
    addr = (rs1 + imm) & 0xFFFFFFFF
    rs2 = reg.read(ins.rs2)

    if debug.DEBUG:
        print(f"0x{rs1:08X}({reg_list[ins.rs1]}) + 0x{imm:08X}(imm) = 0x{addr:08X}, write 0x{rs2:08X} to MEM[0x{addr+4:08X}:0x{addr:08X}]")

    memory.write(addr, 4, rs2)
    reg.add_pc()
