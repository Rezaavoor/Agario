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
  #
  # Please add your own code below this line
  #
  # you can write your code for subroutine "hexasc" below this line
 
.text
.global time2string
.global delay

hexasc: 
	andi	$a0,$a0,0x0000000f
	addi	$a0,$a0,48	# =0	
	
	li	$t0,0x3A	# =10
	slt	$t1,$a0,$t0	# if(a0 < 10) 	om a=10 => t1=0
	
	#om t1=0 ===>	lägg till 7
	
	bne  	$t1,$0, loo
	nop
	addi	$a0,$a0,0x7
	loo:
	move 	$v0,$a0
	jr 	$ra
	nop
  
time2string:
	# ex: $a1 = 0x5958
	PUSH $ra
	PUSH $s0
	PUSH $s1
	
	addi $s0, $a0, 0	# t0: address
	addi $s1, $a1, 0	# t1: time
	
	#first digit
	andi	$a0, $s1, 0xf000 	# take first digit
	srl	$a0, $a0, 12		# 3*4 = 12 bit to right
	jal hexasc			# get string value of the digit
	nop
	sb	$v0, 0($s0)
	
	#second digit
	andi	$a0, $s1, 0xf00 	# take second digit
	srl	$a0, $a0, 8		# 2*4 = 8 bit to right
	jal hexasc			# get string value of the digit
	nop
	sb	$v0, 1($s0)
	
	#semicolon
	addi	$a0,$0,0x3a		# semicolon
	sb	$a0, 2($s0)
	
	#third digit
	andi	$a0, $s1, 0xf0 		# take third digit
	srl	$a0, $a0, 4		# 1*4 = 4 bit to right
	jal hexasc			# get string value of the digit
	nop
	sb	$v0, 3($s0)
	
	#forth digit
	andi	$a0, $s1, 0xf		# take forth digit
	jal hexasc			# get string value of the digit
	nop
	sb	$v0, 4($s0)
	
	#null
	addi	$a0,$0,0x00		# null
	sb	$a0, 5($s0)
	
	
	POP $s1
	POP $s0
	POP $ra
  	
  	jr $ra
  	nop
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
