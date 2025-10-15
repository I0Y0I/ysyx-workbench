import debug
from reg import reg_list


def op_jalr(memory, reg, ins):
    rs1 = reg.read(ins.rs1)
    imm = ins.imm
    pc = (rs1 + imm) & 0xFFFFFFFF

    reg.add_pc()
    rd = reg.get_pc() & 0xFFFFFFFF

    if debug.DEBUG:
        print(f"0x{rs1:08X}({reg_list[ins.rs1]}) + 0x{imm:08X}(imm) = 0x{pc:08X}, set PC to 0x{pc:08X}, write 0x{rd:08X} to {reg_list[ins.rd]}")

    reg.write(ins.rd, rd)
    reg.set_pc(pc)