run:
	iverilog -g2012 -o hw8.out hw8_testbench.sv hw6.sv && vvp hw8.out > real.tmp 

diff:
	diff hw4_expected_display.text hw4_out_display.text
	diff hw6q1_expected_display.text hw6q1_out_display.text

sim:
	gtkwave dump_hw4.vcd && gtkwave dump_hw6q1.vcd

test: run diff

clean:
	rm -f *.out
	rm -f *.tmp
	rm -f *.vcd
	rm -f *.text