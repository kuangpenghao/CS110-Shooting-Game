
.section .text
.globl draw_circ
.type draw_circ, @function
draw_circ:
	addi sp, sp, -4
	sw ra, 0(sp)
	
	jal LCD_DrawCircle
	
	lw ra, 0(sp)
	addi sp, sp, 4
	
	# Return
	ret