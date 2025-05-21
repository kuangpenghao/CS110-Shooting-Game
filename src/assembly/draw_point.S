
.section .text
.globl draw_point
.type draw_point, @function
draw_point:
	addi sp, sp, -4
	sw ra, 0(sp)
	
	jal LCD_DrawPoint
	
	lw ra, 0(sp)
	addi sp, sp, 4
	
	ret