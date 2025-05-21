
.section .text
.globl show_initial_string
.type show_initial_string, @function
show_initial_string:
	# Save return address
	addi sp, sp, -4
	sw ra, 0(sp)

    # Load the address of the global variable scene
    la t0, scene
    la t1, sceney

    # Show the first string
    addi sp, sp, -24
    sw t0, 0(sp)
    sw t1, 4(sp)
    sw a0, 8(sp)
    sw a1, 12(sp)
    sw a2, 16(sp)
    sw a3, 20(sp)

    lw a1, 0(t1)
    lw a2, 0(t0)
    jal LCD_ShowString

    lw t0, 0(sp)
    lw t1, 4(sp)
    lw a0, 8(sp)
    lw a1, 12(sp)
    lw a2, 16(sp)
    lw a3, 20(sp)
    addi sp, sp, 24

    # Show the second string
    addi sp, sp, -24
    sw t0, 0(sp)
    sw t1, 4(sp)
    sw a0, 8(sp)
    sw a1, 12(sp)
    sw a2, 16(sp)
    sw a3, 20(sp)

    lw a1, 4(t1)
    lw a2, 4(t0)
    jal LCD_ShowString

    lw t0, 0(sp)
    lw t1, 4(sp)
    lw a0, 8(sp)
    lw a1, 12(sp)
    lw a2, 16(sp)
    lw a3, 20(sp)
    addi sp, sp, 24

    # Show the third string
    addi sp, sp, -24
    sw t0, 0(sp)
    sw t1, 4(sp)
    sw a0, 8(sp)
    sw a1, 12(sp)
    sw a2, 16(sp)
    sw a3, 20(sp)

    lw a1, 8(t1)
    lw a2, 8(t0)
    jal LCD_ShowString

    lw t0, 0(sp)
    lw t1, 4(sp)
    lw a0, 8(sp)
    lw a1, 12(sp)
    lw a2, 16(sp)
    lw a3, 20(sp)
    addi sp, sp, 24

    # Load the address of init_string_success and modify it to 1
    la t0, init_string_success
    li t1, 1
    sw t1, 0(t0)

	# Restore return address
	lw ra, 0(sp)
	addi sp, sp, 4
	
	# Return
	ret