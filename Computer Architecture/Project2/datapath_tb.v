module datapath_tb;
reg clk, reset;
reg[23:0] P, PCx, IR, BusA, BusB, BusW, extended, memory_out, A, B, ALUresult, memory_content;
reg ALUzero, zFlag, BEQFlag, memRd, memWr;
reg [23:0] registers[0:7]; 
reg[2:0]Ra, Rb, Rw; 
reg[23:0] regPC[0:3];
reg[2:0] curr, Next, currState, NextState, ALUop;
integer insNum, CPI;
integer i;
reg prev, newd;
reg[4:0] opcode;


main datapath(clk, reset, opcode, currState, NextState, PCx, IR, registers, Ra, Rb, Rw, BusA, BusB, BusW, extended, memory_content, zFlag, BEQFlag, regPC, A, B, ALUresult, ALUzero, memory_out, insNum,
	ALUop, memRd, memWr);
initial
	begin
		clk = 0;
		reset = 0;
		CPI = 0;
		P = $unsigned(0);
		curr = $unsigned(0);
		Next = $unsigned(0);
		i = $unsigned(0);
		#10ns reset = ~reset;
		repeat(130)
		begin
		prev = clk;
		#50ns clk = ~clk;
		newd = clk; 
		
		 #90ns if (newd == 1 && prev == 0)
			begin
				case(currState)
					3'b000:	begin
						if (CPI != 0)
							begin
								$display("Flags:");
								$display("Z-flag: %b", zFlag);
								$display("BEQ-flag: %b\n", BEQFlag);
								$display("CPI for previous instruction: %0d", CPI);
								$display("-----------------------------------------");
								$display();
								$display();
								CPI = 0;
							end
						$display("Instruction no.: %0d", insNum);
						$write("Instruction: ");
						display_instruction(IR);
						$display();
						$display("IF stage");
						$display("Contents:");
						$display("IR: %b", IR);
						$display("PC: %h\n", PCx);
					end
					
					3'b001: begin
						$display("ID stage");
						
						$display("Register contents");
						for (i = 0; i < 8; i = i + 1) begin
							$display("R[%0d]: %h",i,registers[i]);
						end
						
						
						$display("Contents:");
						$display("Ra: %0d", Ra);
						$display("Rb: %0d", Rb);
						$display("BusA: %h", BusA);
						$display("BusB: %h", BusB);
						$display("Extended value: %h\n", extended);
						
						
						
						$display("PC calculations:");
						$display("PC + 3: %h", regPC[0]);
						$display("Jump target address: %h", regPC[1]);
						$display("Jump and return: %h", regPC[2]);
						$display("Branch: %h\n", regPC[3]);
					end
					
					3'b010: begin
						$display("EXEC stage");
						$display("Contents:");
						$display("Input A: %h", A);
						$display("Input B: %h\n", B);
						if (ALUop == 3'b000)
							$display("Operation: And");
						else if (ALUop == 3'b001 && (opcode == 5'b00011 || opcode == 5'b01000))
							$display("Operation: Add");
						else if (ALUop == 3'b001 && (opcode == 5'b00100 || opcode == 5'b01111))
							$display("Operation: Sub");
						else if (ALUop == 3'b001 && opcode == 5'b01011)
							$display("Operation: BEQ");
						else if (ALUop == 3'b010)
							$display("Operation: Max");
						else if (ALUop == 3'b011)
							$display("Operation: CMP");
						else if (ALUop == 3'b100)
							 $display("Operation: Shift");
						$display("ALU result: %h", ALUresult);
						$display("ALU zero: %b\n", ALUzero);
					end
					
					3'b011:	begin
						$display("MEM stage");
						$display("Contents:");
						if (memRd)
							begin
								$display("Read address: %h", ALUresult);
								$display("Memory content: %h", memory_content);
								$display("Memory output: %h\n", memory_out);
							end
						if (memWr)
							begin
								$display("Write address: %h", ALUresult);
								$display("Data in: %h", BusB);
								$display("Memory content: %h\n", memory_content);
							end
					end
					
					3'b100: begin
						$display("WB stage");
						$display("Contents:");
						$display("Destination register: %0d", $unsigned(Rw));
						$display("Data: %h", BusW);
						$display("Data in register file: %h\n", registers[$unsigned(Rw)]);
					end
				endcase
				CPI = CPI + 1;
			end
		end
end
endmodule


function display_instruction;
input[23:0] IR;
begin
	case(IR[21:17])
		5'b00000: $write("AND");
		5'b00001: $write("CAS");
		5'b00010: $write("LWS");
		5'b00011: $write("ADD");
		5'b00100: $write("SUB");
		5'b00101: $write("CMP");
		5'b00110: $write("JR");
		5'b00111: $write("ANDI");
		5'b01000: $write("ADDI");
		5'b01001: $write("LW");
		5'b01010: $write("SW");
		5'b01011: $write("BEQ");
		5'b01100: $write("J");
		5'b01101: $write("JAL");
		5'b01110: $write("LUI");
		5'b01111: $write("SUBI");
	endcase
	
	case(IR[16])
		1'b0: $write("");
		1'b1: $write("SF");
	endcase
	
	case(IR[23:22])
		2'b00: $write(" ");
		2'b01: $write("EQ ");
		2'b10: $write("NE ");
	endcase
	
	if ($unsigned(IR[21:17]) >= 0 && $unsigned(IR[21:17]) <=6)
	begin
		if ($unsigned(IR[21:17]) >= 0 && $unsigned(IR[21:17]) <=4)
			case(IR[15:13])
				3'h0: $write("R0, ");
				3'h1: $write("R1, ");
				3'h2: $write("R2, ");
				3'h3: $write("R3, ");
				3'h4: $write("R4, ");
				3'h5: $write("R5, ");
				3'h6: $write("R6, ");
				3'h7: $write("R7, ");
			endcase
		
		if ($unsigned(IR[21:17]) >= 0 && $unsigned(IR[21:17]) <=6)
			case(IR[12:10])
				3'h0: $write("R0, ");
				3'h1: $write("R1, ");
				3'h2: $write("R2, ");
				3'h3: $write("R3, ");
				3'h4: $write("R4, ");
				3'h5: $write("R5, ");
				3'h6: $write("R6, ");
				3'h7: $write("R7, ");
			endcase	
		
		if ($unsigned(IR[21:17]) >= 0 && $unsigned(IR[21:17]) <=5)
			case(IR[9:7])
				3'h0: $write("R0");
				3'h1: $write("R1");
				3'h2: $write("R2");
				3'h3: $write("R3");
				3'h4: $write("R4");
				3'h5: $write("R5");
				3'h6: $write("R6");
				3'h7: $write("R7");
			endcase
	end
	else if ($unsigned(IR[21:17]) == 15 || ($unsigned(IR[21:17]) >= 7 && $unsigned(IR[21:17]) <=11))
	begin
			case(IR[15:13])
				3'h0: $write("R0, ");
				3'h1: $write("R1, ");
				3'h2: $write("R2, ");
				3'h3: $write("R3, ");
				3'h4: $write("R4, ");
				3'h5: $write("R5, ");
				3'h6: $write("R6, ");
				3'h7: $write("R7, ");
			endcase

			case(IR[12:10])
				3'h0: $write("R0, ");
				3'h1: $write("R1, ");
				3'h2: $write("R2, ");
				3'h3: $write("R3, ");
				3'h4: $write("R4, ");
				3'h5: $write("R5, ");
				3'h6: $write("R6, ");
				3'h7: $write("R7, ");
			endcase	
		
		$write("%0d", $signed(IR[9:0]));
	end
	else if ($unsigned(IR[21:17]) >= 12 && $unsigned(IR[21:17]) <=14)
	begin
		$write("%0d", $signed(IR[16:0]));
	end
	
	
		
		



end
endfunction
