from debug import DEBUG
from reg import reg_list


def op_lui(memory, reg, ins):
    imm = ins.imm

    if DEBUG:
        print(f"write 0x{imm:08X} to {reg_list[ins.rd]}")

    reg.write(ins.rd, imm)
    reg.add_pc()