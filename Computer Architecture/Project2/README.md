***
In this reposotoiry, you'll find a detailed implmenetation of multicycle processor using Verilog.
***
First of all, project's [link](https://github.com/amohd63/Multicycle_MIPS/blob/master/Project-2-First-2022_2023.pdf).

The report of our implementation, [here](https://github.com/amohd63/Multicycle_MIPS/blob/master/FinalProjectReport.pdf).

The implementation can be found [here](https://github.com/amohd63/Multicycle_MIPS/tree/master/Multicycle_MIPS/Multicycle_MIPS/src)

The testing of all instructions, [here](https://github.com/amohd63/Multicycle_MIPS/tree/master/Testing).
***
For generating boolean expressions, I used these two [python codes](https://github.com/amohd63/Multicycle_MIPS/tree/master/Python%20codes) for reading the truth tables written in .csv files and extract the indcies of ones and don't care terms to be used later for writing the equations.

Also, I wrote a python code, [main.py](https://github.com/amohd63/Multicycle_MIPS/blob/master/Python%20codes/main.py), for assembling any assembly code, [for example](https://github.com/amohd63/Multicycle_MIPS/blob/master/Assembly%20code.txt), to be used in Verilog.

Otherwise, all .csv files [attached](https://github.com/amohd63/Multicycle_MIPS/tree/master/.CSV%20files), are the truth tables which I used with the codes above.
***
You can find online websites for generating the boolean equations, I found [this website](https://www.charlie-coleman.com/experiments/kmap/) very useful.
***
**How to run the Verilog code**

In the project files [here](https://github.com/amohd63/Multicycle_MIPS/tree/master/Multicycle_MIPS/Multicycle_MIPS/src), open **datapath_tb_runtest.do** file, then click on ["**Execute script**"](https://imgur.com/gqcQ2Rx).
