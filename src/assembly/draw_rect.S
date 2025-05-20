
.section .text
.globl draw_rect
.type draw_rect, @function
draw_rect:
	# Parameters:
	# a0 = x position (already in a0)
	# a1 = y position (already in a1)
	# a2 = width (already in a2)
	# a3 = height (already in a3)
	# a4 = color (already in a4)
	
	# Save return address
	addi sp, sp, -4
	sw ra, 0(sp)
	
	# Parameters are already in the right registers,
	# so we can directly call LCD_DrawRectangle
	jal LCD_DrawRectangle
	
	# Restore return address
	lw ra, 0(sp)
	addi sp, sp, 4
	
	# Return
	ret