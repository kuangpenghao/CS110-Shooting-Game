
.section .text
.globl choose_button
.type choose_button, @function
choose_button:
	# Save return address
	addi sp, sp, -4
	sw ra, 0(sp)

    li t0,1

    beq a0,t0,LEFT
    beq a1,t0,DOWN
    beq a2,t0,RIGHT
    # Restore return address
    li a0,0
	lw ra, 0(sp)
	addi sp, sp, 4
    ret

LEFT:
	# Restore return address
    li a0,1
	lw ra, 0(sp)
	addi sp, sp, 4
    ret
DOWN:
    li a0,2
	lw ra, 0(sp)
	addi sp, sp, 4
    ret
RIGHT:
    li a0,3
	lw ra, 0(sp)
	addi sp, sp, 4
    ret