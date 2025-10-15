class Memory:
    def __init__(self, size):
        self.size = size
        self.mem = bytearray(self.size)

    def load(self, filename):
        try:
            with open(filename, 'r') as f:
                addr = 0
                for line in f:
                    if line.startswith("v3.0"):
                        continue
                    values_str = line.split(" ")
                    values_str = values_str[1:]
                    for value_str in values_str:
                        value = int(value_str, 16)
                        self.write(addr, 4, value)
                        addr = addr + 4
                return addr

        except FileNotFoundError:
            print(f"加载文件失败：文件`{filename}`未找到")

    def _check_bounds(self, addr, size):
        if addr < 0 or addr + size > self.size:
            raise ValueError(f"内存越界：start={addr:08X}, end={(addr+size):08X}")

    def write(self, addr, size, value):
        self._check_bounds(addr, size)
        self.mem[addr:addr+size] = value.to_bytes(size, byteorder='little', signed=False)

    def read(self, addr, size, signed=False):
        self._check_bounds(addr, size)
        return  int.from_bytes(self.mem[addr:addr+size], byteorder='little', signed=signed)