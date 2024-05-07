setactivelib -work
#Compiling UUT module design files
comp -include $dsn\src\main.v
comp -include "$dsn\src\datapath_tb.v"
asim +access +r datapath_tb

wave
wave -noreg clk
wave -noreg reset

run

#End simulation macro
