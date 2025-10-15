
def add(rd, rs1, rs2):
    return (0b00 << 6) | ((rd & 0b11) << 4) | ((rs1 & 0b11) << 2) | (rs2 & 0b11)

def li(rd, imm):
    return (0b10 << 6) | ((rd & 0b11) << 4) | (imm & 0b1111)

def bner0(addr, rs2):
    return (0b11 << 6) | ((addr & 0b1111) << 2) | (rs2 & 0b11)

class cpu:
    def __init__(self):
        self.pc = 0
        self.r = [0, 0, 0, 0]
        self.insts = []

    def exec(self, inst):
        match (inst >> 6 & 0b11):
            case 0b00:
                rd = (inst >> 4 & 0b11)
                rs1 = (inst >> 2 & 0b11)
                rs2 = (inst & 0b11)
                self.r[rd] = self.r[rs1] + self.r[rs2]
                self.pc = self.pc + 1
            case 0b10:
                rd = (inst >> 4 & 0b11)
                imm = (inst & 0b1111)
                self.r[rd] = imm
                self.pc = self.pc + 1
            case 0b11:
                addr = (inst >> 2 & 0b1111)
                rs2 = (inst & 0b11)
                if self.r[rs2] != self.r[0]:
                    self.pc = addr
                else:
                    self.pc = self.pc + 1
        print(self.pc, self.r)

    def get_pc(self):
        return self.pc


if __name__ == "__main__":
    uut = cpu()
    # 10以内自然数和
    # commands = [
    #         li(0, 10),
    #         li(1, 0),
    #         li(2, 0),
    #         li(3, 1),
    #         add(1, 1, 3),
    #         add(2, 2, 1),
    #         bner0(4, 1),
    #         bner0(7, 3)
    #         ]

    # 10以内奇数和
    commands = [
            li(0, 9),
            li(1, 1),
            li(2, 1),
            li(3, 2),
            add(1, 1, 3),
            add(2, 2, 1),
            bner0(4, 1),
            bner0(7, 3)
            ]
    for i in range(len(commands)):
        print("{}: {:08b}".format(i, commands[i]))
    # for i in range(10):
    while (uut.get_pc() != (len(commands) - 1)):
        c = commands[uut.get_pc()]
        # print("{:08b}".format(c))
        uut.exec(c)
