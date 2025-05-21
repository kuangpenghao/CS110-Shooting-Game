
.section .text
.globl draw_pointbig
.type draw_pointbig, @function
draw_pointbig:
	addi sp, sp, -4
	sw ra, 0(sp)
	
	jal LCD_DrawPoint_big
	
	lw ra, 0(sp)
	addi sp, sp, 4
	
	ret