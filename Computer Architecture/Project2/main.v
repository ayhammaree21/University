module main(clk, reset, opcode, currState, NextState, PCx, IR, registers, Ra, Rb, Rw, BusA, BusB, BusW, extended, memory_content, zFlag, BEQFlag, regPC, A, B, ALUresult, ALUzero, memory_out, insNum,
	ALUop, memRd, memWr);
typedef struct {
    reg[23:0] BusA1, BusB1;
} reg_out;
 
typedef struct {
	reg[23:0] result;
	reg flag;
} alu_out;

typedef struct {
	reg[1:0] RWsrc1, WB1;
	reg RBsrc1, RegWR1, ALUsrc1, MRD1, MWR1, subFlag1, extendSel1, RSsrc1;
	reg[2:0] ALUop1;
} control_out;

typedef struct {
	reg z, beq;
} update_flags_out;

input clk, reset;

output reg[23:0] PCx= 24'h000000;
output reg[23:0] IR;
output reg[2:0] currState, NextState;
output reg[4:0] opcode;
output integer insNum;

output reg [23:0] registers[0:7] = '{24'h000000, 24'h000000, 24'h000000, 24'h000000, 24'h000000, 24'h000000, 24'h000000, 24'h000000};
output reg zFlag, BEQFlag;
output reg[2:0] Ra, Rb, Rw;
output reg[23:0] BusA, BusB, BusW, extended, memory_content;
reg[23:0] subB, extSub;
reg[1:0] RWsrc, WB;
reg RSsrc, extendSel, RBsrc, RegWR, ALUsrc, subFlag;
output reg memRd, memWr;
output reg[2:0] ALUop;
reg[1:0] PCsrc;
output reg ALUzero;
reg [7:0] mem_md [0:1023];
reg ExtOp = 0;
output reg[23:0] regPC[0:3];
output reg[23:0] A, B, ALUresult, memory_out;
reg[23:0] temp;
integer i = 0;

reg [7:0] mem [0:1023] = '{8'h11, 8'h00, 8'h00, 8'h09, 8'h24, 8'h00, 8'h10, 8'h24, 8'h05, 8'h82, 8'h44, 8'h00, 8'h14, 8'h2C, 8'h00, 8'h04, 8'h80, 8'h00, 8'h1A, 8'h00, 8'h09, 8'h06, 8'h00, 8'h80, 8'h18, 8'h00, 8'h0B, 8'h1C, 8'h00, 8'h05, 8'h0C, 8'h1C, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00, 8'h00 };

reg_out register_output;
alu_out alu_output;
control_out control_output;
update_flags_out uf_output;

function[23:0] instruction_memory;
	input [23:0] address;
	begin
		if (~reset)
		begin
			instruction_memory = $unsigned(0);
		end
		else if (clk)
		begin
			instruction_memory = {mem[address], mem[address+1], mem[address+2]};
		end
	end
endfunction

function reg_out register_file;
input[2:0] Ra1, Rb1, Rw1;
input[23:0] BusW1;
input enableW;
integer Ra_in1, Rb_in1, Rw_in1;
reg_out res;
begin
	Ra_in1 = $unsigned(Ra1);
	Rb_in1 = $unsigned(Rb1);
	Rw_in1 = $unsigned(Rw1);
	res.BusA1 = registers[Ra_in1];
	res.BusB1 = registers[Rb_in1];
	if (enableW && Rw_in1 != 0)
		registers[Rw_in1] = BusW1;
	if (enableW && Rw_in1 == 0)
		$display ("WARNING: tried to overwrite R0, but it's discarded!");
end
return res;
endfunction

function alu_out alu;
input[23:0] A1, B1;
input[2:0] ALUop1;
reg[23:0] tempOut1;
reg[23:0] andOut, addOut, maxOut, cmpOut, shifter;
reg addZ, cmpZ;
alu_out res;

begin
	andOut = A & B;
	addOut = A + B;
	maxOut = (A > B) ? A : B;
	cmpOut = 0;
	shifter = B << A; 
	
	addZ = ~(| addOut);
	cmpZ = A < B;
	res.result = mux8_1(andOut, addOut, maxOut, cmpOut, shifter, 0, 0, 0, ALUop[2], ALUop[1], ALUop[0]);
	res.flag = mux8_1(0, addZ, 0, cmpZ, 0, 0, 0, 0, ALUop[2], ALUop[1], ALUop[0]);
end
return res;

endfunction

function[23:0] extender;
input[9:0] imm10;
input[16:0] imm17;
input extOp;
input sel;
reg[23:0] extended;
reg[23:0] extended17, extended10;

assign extended17 = $signed(imm17);
assign extended10 = (extOp == 0) ? $signed(imm10) : $unsigned(imm10);

extender = mux2to1(extended10, extended17, sel);

endfunction

function[23:0] memory_data;
   input [23:0] in_data;
   input [23:0] address;
   input memRd1, memWr1;
   begin
	   if (memWr1)
		   	begin
		   	mem_md[address+2] = in_data[7:0];
		   	mem_md[address+1] = in_data[15:8];
		   	mem_md[address] = in_data[23:16];
		   	end
		if (memRd1)
			begin
			memory_data = {mem_md[address], mem_md[address+1], mem_md[address+2]};
			end
	end
endfunction


function control_out control_unit;
input[4:0] opcode;
input[1:0] condition;
input Zflag;

control_out res;
begin
	res.WB1[1] = (~opcode[4] & ~opcode[3] & ~opcode[1]) | (~opcode[4] & opcode[1] & opcode[0]) | (~opcode[4] & opcode[3] & ~opcode[0]);
	res.WB1[0] = (~opcode[4] & opcode[2] & ~opcode[1] & opcode[0]);
	
	res.RWsrc1[1] = (~opcode[4] & opcode[3] & opcode[2] & ~opcode[1]);
	res.RWsrc1[0] = (~opcode[4] & opcode[2] & opcode[1] & ~opcode[0]);
	
	
	res.ALUop1[0] = (~opcode[4] & ~opcode[2] & opcode[1]) | (~opcode[4] & opcode[2] & ~opcode[1]) | (~opcode[4] & opcode[3] & opcode[0]) | (~opcode[4] & opcode[3] & ~opcode[2]);
	res.ALUop1[1] = (~opcode[4] & ~opcode[3] & ~opcode[1] & opcode[0]);
	res.ALUop1[2] = (~opcode[4] & opcode[2] & opcode[1] & ~opcode[0]);
	
	res.extendSel1 = (~opcode[4] & opcode[2] & ~opcode[1]) | (~opcode[4] & opcode[2] & ~opcode[0]);
	res.RSsrc1 = (~opcode[4] & opcode[3] & opcode[2] & ~opcode[0]);
	
	res.ALUsrc1 = (~opcode[4] & ~opcode[2] & opcode[1] & opcode[0]) | (~opcode[4] & ~opcode[3] & ~opcode[2]) | (~opcode[4] & ~opcode[3] & ~opcode[1]);
	res.MWR1 = (~condition[1] & ~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & ~opcode[0] & Zflag) | (~condition[0] & ~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & ~opcode[0] & ~Zflag);
	
	res.RBsrc1 = (~opcode[4] & opcode[3]);
	
	res.MRD1 = (~condition[1] & ~opcode[4] & ~opcode[3] & ~opcode[2] & opcode[1] & ~opcode[0] & Zflag) | (~condition[1] & ~opcode[4] & opcode[3] & ~opcode[2] & ~opcode[1] & opcode[0] & Zflag) | (~condition[0] & ~opcode[4] & ~opcode[3] & ~opcode[2] & opcode[1] & ~opcode[0] & ~Zflag) | (~condition[0] & ~opcode[4] & opcode[3] & ~opcode[2] & ~opcode[1] & opcode[0] & ~Zflag);
		
	res.subFlag1 = (~opcode[4] & opcode[3] & opcode[1] & opcode[0]) | (~opcode[4] & ~opcode[3] & ~opcode[1]);
	res.RegWR1 = (~condition[1] & ~opcode[4] & ~opcode[3] & ~opcode[2] & Zflag) | (~condition[1] & ~opcode[4] & ~opcode[3] & ~opcode[1] & ~opcode[0] & Zflag) | (~condition[1] & ~opcode[4] & ~opcode[2] & ~opcode[1] & Zflag) | (~condition[1] & ~opcode[4] & opcode[3] & opcode[2] & opcode[1] & Zflag) | (~condition[0] & ~opcode[4] & ~opcode[3] & ~opcode[2] & ~Zflag) | (~condition[0] & ~opcode[4] & ~opcode[3] & ~opcode[1] & ~opcode[0] & ~Zflag) | (~condition[0] & ~opcode[4] & ~opcode[2] & ~opcode[1] & ~Zflag) | (~condition[0] & ~opcode[4] & opcode[3] & opcode[2] & opcode[1] & ~Zflag) | (~condition[0] & ~opcode[4] & ~opcode[3] & opcode[1] & opcode[0] & ~Zflag) | (~condition[1] & ~opcode[4] & ~opcode[3] & opcode[1] & opcode[0] & Zflag) | (~condition[0] & ~opcode[4] & opcode[3] & ~opcode[1] & opcode[0] & ~Zflag) | (~condition[1] & ~opcode[4] & opcode[3] & ~opcode[1] & opcode[0] & Zflag);
end
return res;
endfunction

function[1:0] pc_control;
input[4:0] opcode;
input[1:0] condition;
input BEQf, zF;
reg[1:0] PCsrc1;
begin
	PCsrc1[1] = (~condition[1] & ~opcode[4] & ~opcode[3] & opcode[2] & opcode[1] & ~opcode[0] & zF) | (~condition[1] & ~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & opcode[0] & zF & BEQf) | (~condition[0] & ~opcode[4] & ~opcode[3] & opcode[2] & opcode[1] & ~opcode[0] & ~zF) | (~condition[0] & ~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & opcode[0] & ~zF & BEQf);
	PCsrc1[0] = (~condition[1] & ~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & opcode[0] & zF & BEQf) | (~condition[1] & ~opcode[4] & opcode[3] & opcode[2] & ~opcode[1] & zF) | (~condition[0] & ~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & opcode[0] & ~zF & BEQf) | (~condition[0] & ~opcode[4] & opcode[3] & opcode[2] & ~opcode[1] & ~zF);
	
	pc_control = PCsrc1;
end
endfunction

function[2:0] state_machine;
input[2:0] curr;
input[4:0] opcode;
input[1:0] cond;
input zF;
reg[2:0] nextState;
begin
	nextState[2] = (~curr[2] & curr[0] & ~opcode[4] & opcode[3] & opcode[2] & ~opcode[1] & opcode[0] & ~cond[1] & zF) | (~curr[2] & curr[0] & ~opcode[4] & opcode[3] & opcode[2] & ~opcode[1] & opcode[0] & ~cond[0] & ~zF) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[3] & ~opcode[1] & ~opcode[0] & ~cond[1]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[3] & ~opcode[1] & ~opcode[0] & ~cond[0]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[2] & ~opcode[1] & ~opcode[0] & ~cond[1]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[2] & ~opcode[1] & ~opcode[0] & ~cond[0]) | (~curr[2] & curr[1] & ~opcode[4] & opcode[3] & opcode[2] & opcode[0] & ~cond[1]) | (~curr[2] & curr[1] & ~opcode[4] & opcode[3] & opcode[2] & opcode[0] & ~cond[0]) | (~curr[2] & curr[1] & ~opcode[4] & opcode[3] & opcode[2] & opcode[1] & ~cond[1]) | (~curr[2] & curr[1] & ~opcode[4] & opcode[3] & opcode[2] & opcode[1] & ~cond[0]) | (~curr[2] & curr[1] & curr[0] & ~opcode[4] & ~opcode[3] & ~cond[1]) | (~curr[2] & curr[1] & curr[0] & ~opcode[4] & ~opcode[3] & ~cond[0]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[3] & ~opcode[2] & ~opcode[1] & ~cond[1]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[3] & ~opcode[2] & ~opcode[1] & ~cond[0]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[3] & opcode[1] & opcode[0] & ~cond[0]) | (~curr[2] & curr[1] & ~opcode[4] & ~opcode[3] & opcode[1] & opcode[0] & ~cond[1]) | (~curr[2] & curr[1] & curr[0] & ~opcode[4] & ~opcode[1] & ~cond[1]) | (~curr[2] & curr[1] & curr[0] & ~opcode[4] & ~opcode[1] & ~cond[0]);
	nextState[1] = (~curr[2] & ~curr[1] & curr[0] & ~opcode[4] & ~opcode[2] & ~cond[1] & zF) | (~curr[2] & ~curr[1] & curr[0] & ~opcode[4] & ~opcode[2] & ~cond[0] & ~zF) | (~curr[2] & curr[0] & ~opcode[4] & ~opcode[3] & ~opcode[1] & ~cond[1] & zF) | (~curr[2] & curr[0] & ~opcode[4] & ~opcode[3] & ~opcode[1] & ~cond[0] & ~zF) | (~curr[2] & curr[1] & ~curr[0] & ~opcode[4] & ~opcode[2] & opcode[1] & ~opcode[0] & ~cond[1]) | (~curr[2] & curr[1] & ~curr[0] & ~opcode[4] & ~opcode[2] & opcode[1] & ~opcode[0] & ~cond[0]) | (~curr[2] & curr[1] & ~curr[0] & ~opcode[4] & opcode[3] & ~opcode[2] & ~opcode[1] & opcode[0] & ~cond[1]) | (~curr[2] & curr[1] & ~curr[0] & ~opcode[4] & opcode[3] & ~opcode[2] & ~opcode[1] & opcode[0] & ~cond[0]) | (~curr[2] & curr[0] & ~opcode[4] & ~opcode[3] & opcode[0] & ~cond[0] & ~zF) | (~curr[2] & curr[0] & ~opcode[4] & ~opcode[3] & opcode[0] & ~cond[1] & zF) | (~curr[2] & ~curr[1] & curr[0] & ~opcode[4] & opcode[3] & opcode[1] & ~cond[0] & ~zF) | (~curr[2] & ~curr[1] & curr[0] & ~opcode[4] & opcode[3] & opcode[1] & ~cond[1] & zF);
	nextState[0] = (~curr[2] & ~curr[1] & ~curr[0] & ~opcode[4] & ~cond[1]) | (~curr[2] & ~curr[1] & ~curr[0] & ~opcode[4] & ~cond[0]) | (~curr[2] & ~curr[0] & ~opcode[4] & ~opcode[2] & opcode[1] & ~opcode[0] & ~cond[1]) | (~curr[2] & ~curr[0] & ~opcode[4] & ~opcode[2] & opcode[1] & ~opcode[0] & ~cond[0]) | (~curr[2] & ~curr[0] & ~opcode[4] & opcode[3] & ~opcode[2] & ~opcode[1] & opcode[0] & ~cond[1]) | (~curr[2] & ~curr[0] & ~opcode[4] & opcode[3] & ~opcode[2] & ~opcode[1] & opcode[0] & ~cond[0]);
	
	
	state_machine = nextState;
end
endfunction

function update_flags_out update_flags;
input prevZ, prevBEQ, SF, ALUflag;
input[4:0] opcode;
update_flags_out res;
begin
res.z = (~opcode[4] & ~opcode[2] & prevZ) | (~opcode[4] & ~opcode[3] & opcode[2] & ~opcode[1] & ~opcode[0] & SF & ALUflag) | (~opcode[4] & prevZ & ~SF) | (~opcode[4] & opcode[3] & opcode[2] & opcode[1] & opcode[0] & SF & ALUflag) | (~opcode[4] & ~opcode[3] & opcode[0] & prevZ) | (~opcode[4] & opcode[1] & ~opcode[0] & prevZ) | (~opcode[4] & opcode[3] & ~opcode[1] & prevZ);
res.beq = (~opcode[4] & ~opcode[3] & prevBEQ) | (~opcode[4] & ~opcode[1] & prevBEQ) | (~opcode[4] & ~opcode[0] & prevBEQ) | (~opcode[4] & opcode[3] & ~opcode[2] & opcode[1] & opcode[0] & ALUflag) | (~opcode[4] & ~prevZ & prevBEQ & SF) | (~opcode[4] & opcode[2] & prevBEQ);
end
return res;
endfunction

always @(posedge clk or negedge reset) begin
	if (~reset) 
		begin
			 reset_reg();
		end
	else
		begin
			case(NextState)
				3'b000:	begin
					insNum = insNum + 1;
					currState = 3'b000;
					uf_output = update_flags(zFlag, BEQFlag, IR[16], ALUzero, IR[21:17]);
					zFlag = uf_output.z;
					BEQFlag = uf_output.beq;
					PCsrc = pc_control(IR[21:17], IR[23:22], BEQFlag, zFlag);
					PCx = mux4_1(regPC[0], regPC[1], regPC[2], regPC[3], PCsrc[0], PCsrc[1]);
					IR = instruction_memory(PCx);
					opcode = IR[21:17]; 
					//regPC[0] should be added here
				end
				
				3'b001: begin
					currState = 3'b001;
					
					
					control_output = control_unit(IR[21:17], IR[23:22], zFlag);
					
					RSsrc = control_output.RSsrc1;
					extendSel = control_output.extendSel1;
					RBsrc = control_output.RBsrc1;
					RWsrc =	control_output.RWsrc1;
					WB = control_output.WB1;
					RegWR = control_output.RegWR1;
					ALUsrc = control_output.ALUsrc1;
					memRd = control_output.MRD1;
					memWr = control_output.MWR1;
					ALUop = control_output.ALUop1;
					subFlag = control_output.subFlag1;
					
					Ra = IR[12:10];
				 	Rb = mux2to1(IR[9:7], IR[15:13], RBsrc);
					Rw = mux4_1(IR[15:13], 1, 7, 0, RWsrc[0], RWsrc[1]);
					register_output = register_file(Ra, Rb, 0, 0, 0);
					BusA = register_output.BusA1;
					BusB = register_output.BusB1;
					
					extended = extender(IR[9:0], IR[16:0], 0, extendSel); 
					
					ALUzero = 0;
					
					
					regPC[0] = PCx + 3;
					temp = extended * 3; 
					regPC[1] = {PCx[23:19], temp[18:0]};
					
					regPC[2] = BusA;
					regPC[3] = PCx + 3 + (3 * extended);
				end
				
				3'b010: begin
					currState = 3'b010;
					A = mux2to1(BusA, 4, RSsrc); 
					//modify instead of 3 muxes use 2
					subB = mux2to1(BusB, (~BusB + 1), subFlag);
					extSub = mux2to1(extended, (~extended + 1), subFlag); 
				 	B = mux2to1(extSub, subB, ALUsrc);
				   	alu_output = alu(A, B, ALUop);
					zFlag = (ALUop == 3'b011) ? alu_output.flag : zFlag;
					ALUresult = alu_output.result;
					ALUzero = alu_output.flag;
				end
				
				3'b011:	begin
					currState = 3'b011;
					memory_out = memory_data(BusB, ALUresult, memRd, memWr);
					memory_content = memory_data(0, ALUresult, 1, 0);
				end
				
				3'b100: begin
					currState = 3'b100;
					BusW = mux4_1(memory_out, regPC[0], ALUresult, 0, WB[0], WB[1]);
					register_file(0, 0, Rw, BusW, RegWR);
				end
			endcase
			NextState = state_machine(currState, IR[21:17], IR[23:22], zFlag);
		end
end

function[23:0] mux2to1;
	input[23:0] in1, in2;
	input s;
	begin
		if (s)
			mux2to1 = in2;
		else
			mux2to1 = in1;
	end
endfunction


function[23:0] mux4_1;
input[23:0] in1, in2, in3, in4;
input s1, s2;
begin
    case ({s2, s1})
        2'b00: mux4_1 = in1;
        2'b01: mux4_1 = in2;
        2'b10: mux4_1 = in3;
        2'b11: mux4_1 = in4;
    endcase
end
endfunction


function[23:0] mux8_1;
input[23:0] in1, in2, in3, in4, in5, in6, in7, in8;
input s3, s2, s1;
begin
    case ({s3, s2, s1})
        3'b000: mux8_1 = in1;
        3'b001: mux8_1 = in2;
        3'b010: mux8_1 = in3;
        3'b011: mux8_1 = in4;
		3'b100: mux8_1 = in5;
        3'b101: mux8_1 = in6;
        3'b110: mux8_1 = in7;
        3'b111: mux8_1 = in8;
    endcase
end
endfunction

function reset_reg;
begin
	PCx = 24'h000000;
			regPC[0] = 24'h000000;
			regPC[1] = 24'h000000;
			regPC[2] = 24'h000000;
			regPC[3] = 24'h000000;
			
			opcode = $unsigned(0);
			
			RSsrc = 24'h000000;
			extendSel = 24'h000000;
			ExtOp = 0;
			A = 24'h000000;
			B = 24'h000000;
			zFlag = 0;
			BEQFlag = 0;
			IR = $unsigned(0);
			BusA = $unsigned(0);
			BusB = $unsigned(0);
			subB = $unsigned(0);
			BusW = 24'h000000;
			ALUresult = $unsigned(0);
			RWsrc = $unsigned(0);
			ALUzero = $unsigned(0);
			Ra = $unsigned(0); 
			Rb = $unsigned(0);
			Rw = $unsigned(0);
			WB = $unsigned(0);
			RBsrc = $unsigned(0);
			RegWR = $unsigned(0);
			ALUsrc = $unsigned(0);
			memRd = $unsigned(0);
			memWr = $unsigned(0);
			ALUop = $unsigned(0);
			memory_out = $unsigned(0); 
			PCsrc = $unsigned(0);
			subFlag = $unsigned(0);
			NextState = $unsigned(0);
			currState = $unsigned(0);
			extended = $unsigned(0);
			memory_content = $unsigned(0);

			control_output.RBsrc1 = $unsigned(0);
			control_output.RWsrc1 = $unsigned(0);
			control_output.WB1 = $unsigned(0);
			control_output.RegWR1 = $unsigned(0);
			control_output.ALUsrc1 = $unsigned(0);
			control_output.MRD1 = $unsigned(0);
			control_output.MWR1 = $unsigned(0);
			control_output.ALUop1 = $unsigned(0);
			control_output.extendSel1 = $unsigned(0);
			control_output.RSsrc1 = $unsigned(0);
			
			register_output.BusA1 = $unsigned(0);
			register_output.BusB1 = $unsigned(0);
			
			alu_output.result = $unsigned(0);
			alu_output.flag = $unsigned(0);
			
			uf_output.z = $unsigned(0);
			uf_output.beq = $unsigned(0);
			insNum = 0;
			
			for (i = 0; i < 1024; i++)
				mem_md[i] = 8'h00;
end
endfunction

endmodule