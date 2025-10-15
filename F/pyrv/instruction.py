import struct

import reg

def sign_ext(value, bits):
    sign_bit = 1 << (bits - 1)
    if value & sign_bit:
        return value | ((~0) << bits) & 0xFFFFFFFF
    else:
        return value

def force_int32(value):
    sign_bit = 1 << 31
    if value & sign_bit:
        return value - (1<<32)
    else:
        return value

class Instruction:
    def __init__(self, raw):
        self.raw = raw
        self.op = self.raw & 0x7F
        self.rd = (self.raw >> 7) & 0x1F
        self.funct3 = (self.raw >> 12) & 0x7
        self.rs1 = (self.raw >> 15) & 0x1F
        self.rs2 = (self.raw >> 20) & 0x1F
        self.funct7 = (self.raw >> 25) & 0x7F

class InstructionR(Instruction):
    def __init__(self, raw):
        super().__init__(raw)

        # 判断指令类型
        match (self.funct7, self.funct3):
            case (0b0000000, 0b001):
                self.type = "sll"
            case (0b0000000, 0b101):
                self.type = "srl"
            case (0b0100000, 0b101):
                self.type = "sra"
            case (0b0000000, 0b000):
                self.type = "add"
            case (0b0100000, 0b000):
                self.type = "sub"
            case (0b0000000, 0b100):
                self.type = "xor"
            case (0b0000000, 0b110):
                self.type = "or"
            case (0b0000000, 0b111):
                self.type = "and"
            case (0b0000000, 0b010):
                self.type = "slt"
            case (0b0000000, 0b011):
                self.type = "sltu"
            case _:
                raise ValueError(f"未定义的R-Type指令：op={self.op:07b}, funct7={self.funct7:07b}, funct3={self.funct3:03b}, raw={self.raw:08X}")

    def __str__(self):
        return f"{self.type}\t{reg.reg_list[self.rd]}, {reg.reg_list[self.rs1]}, {reg.reg_list[self.rs2]}"

class InstructionI(Instruction):
    def __init__(self, raw):
        super().__init__(raw)
        self.imm = sign_ext((self.raw >> 20) & 0xFFF, 12)

        match self.op:
            case 0b0010011:
                match self.funct3:
                    case 0b001:
                        self.shamt = self.rs2
                        match self.funct7:
                            case 0b0000000:
                                self.type = "slli"
                            case _:
                                raise ValueError(f"未定义的I-Type指令：op={self.op:07b}, funct7={self.funct7:07b}, funct3={self.funct3:03b}, raw={self.raw:08X}")
                    case 0b101:
                        self.shamt = self.rs2
                        match self.funct7:
                            case 0b0000000:
                                self.type = "srli"
                            case 0b0100000:
                                self.type = "srai"
                            case _:
                                raise ValueError(f"未定义的I-Type指令：op={self.op:07b}, funct7={self.funct7:07b}, funct3={self.funct3:03b}, raw={self.raw:08X}")
                    case 0b000:
                        self.type = "addi"
                    case 0b100:
                        self.type = "xori"
                    case 0b110:
                        self.type = "ori"
                    case 0b111:
                        self.type = "andi"
                    case 0b010:
                        self.type = "slti"
                    case 0b011:
                        self.type = "sltiu"
                    case _:
                        raise ValueError(f"未定义的I-Type指令：op={self.op:07b}, funct3={self.funct3:03b}, raw={self.raw:08X}")
            case 0b1100111:
                match self.funct3:
                    case 0b000:
                        self.type = "jalr"
                    case _:
                        raise ValueError(f"未定义的I-Type指令：op={self.op:07b}, funct3={self.funct3:03b}, raw={self.raw:08X}")
            case 0b0000011:
                match self.funct3:
                    case 0b000:
                        self.type = "lb"
                    case 0b001:
                        self.type = "lh"
                    case 0b010:
                        self.type = "lw"
                    case 0b100:
                        self.type = "lbu"
                    case 0b101:
                        self.type = "lhu"
                    case _:
                        raise ValueError(f"未定义的I-Type指令：op={self.op:07b}, funct3={self.funct3:03b}, raw={self.raw:08X}")

    def __str__(self):
        match self.type:
            case "slli" | "slti" | "sltiu":
                return f"{self.type}\t{reg.reg_list[self.rd]}, {reg.reg_list[self.rs1]}, {self.shamt:2d}"
            case "jalr" | "lb" | "lh" | "lbu" | "lhu" | "lw":
                return f"{self.type}\t{reg.reg_list[self.rd]}, {force_int32(self.imm)}({reg.reg_list[self.rs1]})"
            case _:
                return f"{self.type}\t{reg.reg_list[self.rd]}, {reg.reg_list[self.rs1]}, {force_int32(self.imm)}"

class InstructionS(Instruction):
    def __init__(self, raw):
        super().__init__(raw)
        self.imm = sign_ext((((self.raw >> 25) & 0x7F) << 5) | ((self.raw >> 7) & 0x1F), 12)

        match self.funct3:
            case 0b000:
                self.type = "sb"
            case 0b001:
                self.type = "sh"
            case 0b010:
                self.type = "sw"
            case _:
                raise ValueError(f"未定义的S-Type指令：funct3={self.funct3:03b}, raw={self.raw:08X}")

    def __str__(self):
        return f"{self.type}\t{reg.reg_list[self.rs2]}, {force_int32(self.imm)}({reg.reg_list[self.rs1]})"

class InstructionB(Instruction):
    def __init__(self, raw):
        super().__init__(raw)
        imm_12 = (self.raw >> 31) & 0b1
        imm_10_5 = (self.raw >> 25) & 0b111111
        imm_4_1 = (self.raw >> 8) & 0b1111
        imm_11 = (self.raw >> 7) & 0b1
        self.imm = sign_ext((imm_12 << 12) | (imm_11 << 11) | (imm_10_5 << 5) | (imm_4_1 << 1), 13)

        match self.funct3:
            case 0b000:
                self.type = "beq"
            case 0b001:
                self.type = "bne"
            case 0b100:
                self.type = "blt"
            case 0b101:
                self.type = "bge"
            case 0b110:
                self.type = "bltu"
            case 0b111:
                self.type = "bgeu"
            case _:
                raise ValueError(f"未定义的B-Type指令：funct3={self.funct3:03b}, raw={self.raw:08X}")

    def __str__(self):
        return f"{self.type}\t{reg.reg_list[self.rs1]}, {reg.reg_list[self.rs2]}, {force_int32(self.imm)}"

class InstructionU(Instruction):
    def __init__(self, raw):
        super().__init__(raw)
        self.imm = self.raw & 0xFFFFF000

        match self.op:
            case 0b0110111:
                self.type = "lui"
            case 0b0010111:
                self.type = "auipc"
            case _:
                raise ValueError(f"未定义的U-Type指令：op={self.op:07b}, raw={self.raw:08X}")

    def __str__(self):
        return f"{self.type}\t{reg.reg_list[self.rd]}, 0x{(self.imm >> 12):X}"

class InstructionJ(Instruction):
    def __init__(self, raw):
        super().__init__(raw)
        imm_20 = (self.raw >> 31) & 0x1
        imm_10_1 = (self.raw >> 21) & 0x3FF
        imm_11 = (self.raw >> 20) & 0x1
        imm_19_12 = (self.raw >> 12) & 0xFF
        self.imm = sign_ext((imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1), 21)

        match self.op:
            case 0b1101111:
                self.type = "jal"
            case _:
                raise ValueError(f"未定义的J-Type指令：op={self.op:07b}, raw={self.raw:08X}")

    def __str__(self):
        return f"{self.type}\t{reg.reg_list[self.rd]}, {force_int32(self.imm)}"

def decode(raw):
    op = raw & 0x7F
    match op:
        case 0b0110011:
            return InstructionR(raw)
        case 0b0010011 | 0b1100111 | 0b0000011:
            return InstructionI(raw)
        case 0b0100011:
            return InstructionS(raw)
        case 0b1100011:
            return InstructionB(raw)
        case 0b0110111 | 0b0010111:
            return InstructionU(raw)
        case 0b1101111:
            return InstructionJ(raw)
        case _:
            raise ValueError(f"未定义的指令：op={op:07b}")