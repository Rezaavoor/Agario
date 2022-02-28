  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
 
.text
.global time2string
.global delay

delay:
 	# a0 = ms
 	while:
 		slti  $t0, $a0, 1	# if a0 < 1 => t0 = 1
 		bne $t0, $0, exitWhile
 		nop
 		#for loop
 		addi $t1, $0, 0
 		addi $t2, $0, 4000		# 4 or can be whatever
 		
 		for:
 			slt $t3, $t1, $t2	# if i<4711 => t3 = 1
 			beq $t3, $0, exitFor
 			nop
 			addi $t1,$t1,1
 			j for
 			nop
 		exitFor:
 			addi $a0, $a0, -1
 			j while
 			nop
 		
 	exitWhile:
 		jr $ra
 		nop
