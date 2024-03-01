.data
	#.word 0x12345

.text
	ori t0, zero, 0x300
	ori t1, zero, 0x45
	add t2, t0, t1
	sub t3, t0, t1
	lui t4, 0x12
	add t4, t4, t2
	jal label1
	ori t6, zero, 0x1
label1:
	lui a0, 0x40
	sw t4, (a0)
	ori a1, zero, 0x4
	add a1, a0, a1
	sw t4, (a1) 
	lw t5, (a0)
	beq t4, t5, label2
	ori t6, zero, 0x2
label2:
	jal label2