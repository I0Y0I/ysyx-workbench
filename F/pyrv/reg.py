class Reg:
    def __init__(self):
        self.regs = [0] * 16
        self.pc = 0

    def reset(self):
        self.regs = [0] * 16
        self.pc = 0

    @staticmethod
    def _check_bounds(reg):
        if reg < 0 or reg > 15:
            raise ValueError(f"不存在的寄存器：reg={reg}")

    def write(self, reg, value):
        self._check_bounds(reg)
        if reg != 0:
            self.regs[reg] = value & 0xFFFFFFFF

    def read(self, reg):
        self._check_bounds(reg)
        return self.regs[reg]

    def set_pc(self, pc):
        self.pc = pc

    def add_pc(self):
        self.pc = self.pc + 4

    def get_pc(self):
        return self.pc

    def __str__(self):
        lines = []
        for i in range(16):
            lines.append(f"{reg_list[i]}:\t{self.regs[i]:08X}")
        return "\n".join(lines)

reg_list = ["zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5"]