# This is a sample Python script.

from instruction import decode
from memory import Memory
from op.op_add import op_add
from op.op_addi import op_addi
from op.op_jalr import op_jalr
from op.op_lbu import op_lbu
from op.op_lui import op_lui
from op.op_lw import op_lw
from op.op_sb import op_sb
from op.op_sw import op_sw
from reg import Reg

ins_dict = {
    "add" : op_add,
    "addi" : op_addi,
    "lui" : op_lui,
    "lw" : op_lw,
    "lbu" : op_lbu,
    "sw" : op_sw,
    "sb" : op_sb,
    "jalr": op_jalr
}

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    mem = Memory(1<<32)
    reg = Reg()
    cnt = 0

    while True:
        try:
            cmd = input(">> ").strip()
            if not cmd:
                continue

            parts = cmd.split()
            command = parts[0]
            args = parts[1:]

            match command:
                case "l":
                    if len(args) != 1:
                        continue
                    addr = mem.load(args[0])
                    print(f"加载成功，结束地址0x{addr:08X}")
                case "n":
                    steps = 1
                    if len(args) == 1:
                        steps = int(args[0])
                    if steps != 1:
                        print(f"运行{steps}步")
                    for i in range(steps):
                        pc = reg.get_pc()
                        raw = mem.read(pc, 4)
                        ins = decode(raw)
                        print(f"{i:4d}\t{pc:X}: {ins}")
                        # print(f"{pc>>2:4X}")
                        op = ins_dict[ins.type]
                        op(mem, reg, ins)
                    cnt = cnt + steps
                    print(f"当前已运行{cnt}步")
                case "r":
                    reg.reset()
                    print("重置成功")
                case "p":
                    print(reg)
                case _:
                    print("未定义命令")

        except (IOError, KeyboardInterrupt):
            print("退出")
            break

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
