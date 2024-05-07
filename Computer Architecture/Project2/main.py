import re


def decimal_to_3bit_binary(decimal):
    binary = ""
    for i in range(2, -1, -1):
        binary += str(decimal >> i & 1)
    return binary


def twos_complement_10(decimal):
    if decimal >= 0:
        return bin(decimal)[2:].zfill(10)
    else:
        return bin(2**10 + decimal)[2:]


def twos_complement_17(decimal):
    if decimal >= 0:
        return bin(decimal)[2:].zfill(17)
    else:
        return bin(2**17 + decimal)[2:].zfill(17)


opcode = {
    "AND": "00000",
    "CAS": "00001",
    "LWS": "00010",
    "ADD": "00011",
    "SUB": "00100",
    "CMP": "00101",
    "JR": "00110",
    "ANDI": "00111",
    "ADDI": "01000",
    "SUBI": "01111",
    "LW": "01001",
    "SW": "01010",
    "BEQ": "01011",
    "J": "01100",
    "JAL": "01101",
    "LUI": "01110"
}

instruction_type = dict.fromkeys(["AND", "CAS", "LWS", "ADD", "SUB", "CMP", "JR"], "R")
instruction_type.update(dict.fromkeys(["ANDI", "ADDI", "SUBI", "LW", "SW", "BEQ"], "I"))
instruction_type.update(dict.fromkeys(["J", "JAL", "LUI"], "J"))
memory = ""
num_of_instructions = 0
with open("code.txt", "r") as file:
    lines = file.readlines()
    for instruction in lines:
        #instruction = "ADDI r0, r1, 7"
        instruction = instruction.upper()
        instruction = instruction.replace(",", "")
        comp = instruction.split()
        bin_instruct = ""
        tempCond = ""
        tempSF = ""
        if 'NE' in comp[0]:
            tempCond += "10"
        elif 'EQ' in comp[0] and comp[0] != 'BEQ':
            tempCond += "01"
        else:
            tempCond += "00"

        if "SF" in comp[0]:
            tempSF += "1"
        else:
            tempSF += "0"

        inst = comp[0].replace("EQ" if comp[0] != 'BEQ' else "", "").replace("NE", "").replace("SF", "")
        if instruction_type.get(inst) == "R":
            bin_instruct += tempCond
            bin_instruct += opcode.get(comp[0])
            bin_instruct += tempSF
            if comp[0] == "CMP":
                bin_instruct += "0"*3
                for i in range(len(comp) - 1):
                    bin_instruct += decimal_to_3bit_binary(int(re.sub(r'\D', '', comp[i+1])))
                bin_instruct += "0"*7
            elif comp[0] == "JR":
                bin_instruct += "0" * 3
                bin_instruct += decimal_to_3bit_binary(int(re.sub(r'\D', '', comp[1])))
                bin_instruct += "0" * 10
            else:
                for i in range(len(comp) - 1):
                    bin_instruct += decimal_to_3bit_binary(int(re.sub(r'\D', '', comp[i+1])))
                bin_instruct += "0"*7

        elif instruction_type.get(inst) == "I":
            bin_instruct += tempCond
            bin_instruct += opcode.get(comp[0])
            bin_instruct += tempSF

            for i in range(len(comp) - 2):
                bin_instruct += decimal_to_3bit_binary(int(re.sub(r'\D', '', comp[i + 1])))

            if comp[0] == "SUBI":
                bin_instruct += twos_complement_10(int(comp[3]) * (-1))
            else:
                bin_instruct += twos_complement_10(int(comp[3]))

        elif instruction_type.get(inst) == "J":
            bin_instruct += tempCond
            bin_instruct += opcode.get(comp[0])
            bin_instruct += twos_complement_17(int(comp[1]))
        print("Instruction: " + instruction.replace("\n", ""))
        print("Binary representation: " + bin_instruct)
        print("Hexadecimal representation: " + hex(int(bin_instruct, 2)))
        print("Length: " + str(len(bin_instruct)) + " bits")
        print()
        #memory += hex(int(bin_instruct, 2)).upper().replace("0X", "24'h") + ", "
        tempStr = hex(int(bin_instruct, 2)).upper().replace("0X", "")
        tempStr = "0" * (6 - len(tempStr)) + tempStr
        arrBytes = re.findall('.{1,2}', tempStr)
        tempStr = [("8'h" + arrBytes[i] + ",") for i in range(len(arrBytes))]
        memory += " ".join(tempStr) + " "
        num_of_instructions += 3

print(memory)
memory += ("8'h00, ") * (1024 - num_of_instructions)
index = memory.rindex(',')
new_mem = memory[:index] + memory[index+1:]

instruction_memory_code = "module instruction_memory(address, out_data, clk, reset);\n" + \
                          "output reg [23:0] out_data;\n" + \
                          "input [23:0] address;\n" + \
                          "input clk, reset;\n"

instruction_memory_code += "reg [7:0] mem [0:1023] = '{" + new_mem + "};\n"

instruction_memory_code += "always @(posedge clk or negedge reset)\n" + \
                           "begin\n" + \
                           "if (~reset)\n" + \
                           "begin\n" + \
                           "out_data = $unsigned(0);\n" + \
                           "end\n" + \
                           "else\n" + \
                           "begin\n"\
                           "out_data <= {mem[address], mem[address+1], mem[address+2]};\n" + \
                           "end\n" \
                           + "end\n" + \
                           "endmodule\n"

print("-" * 20)
print("Instruction memory Verilog code\n")
print(instruction_memory_code)
print("-" * 20)

# with open("./Multicycle_MIPS/Multicycle_MIPS/src/instruction_memory.v", "w") as file:
#     file.write(instruction_memory_code)
#
# print("Code is written to {instruction_memory.v} successfully.")


