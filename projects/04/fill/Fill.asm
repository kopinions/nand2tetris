// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

	(RESTART)
	@SCREEN
	D=A
	@pointer  // i=SCREEN
	M=D
	@color
	M=-1

	(LOOP)
	@KBD
	D=A
	@pointer
	D=M-D
	@END
	D			;JGE  // while(i-KBD >=0)

	@KBD
	D=M
	@BLACK  // color=BLACK
	D			;JGT
	@WHITE  // color=WHITE
	D			;JEQ
	
	(BLACK)
	@color   // color=BLACK
	M=-1
	@FILL
	0			;JMP

	
	(WHITE)
	@color	 // color=WHITE
	M=0
	@FILL
	0			;JMP

	(FILL)   // fill the screen with color
	@color
	D=M
	@pointer
	A=M
	M=D
	@pointer  // i=i+1
	M=M+1
	@LOOP
	0			;JMP
	
	(END)

	@RESTART
	0			;JMP
