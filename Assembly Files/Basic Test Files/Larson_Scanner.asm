START:
	LDA #$FF 	; Store 255 in the DDRB register in the 65C22
	STA $B402	
	LDA #00		; Clear the accumulator
	CLC
FORWARD:
	ADC #$01	; Count up in the accumulator
  	STA $B400	; Display the contents of the accumulator using PORTB
  	JSR DELAY 	; Delay for a bit
  	CMP #$FF	; Compare with 255
  	BNE FORWARD	; Continue until 255 is reached
  	SEC
BACKWARD:
	SBC #$01		; Count down in the accumulator
  	STA $B400		; Display the contents of the accumulator using PORTB
  	JSR DELAY 		; Delay for a bit
  	CMP #$00		; Compare with 0
  	BNE BACKWARD	; Continue until 0 is reached
  	CLC
  	JMP FORWARD		; Repeat
;;;; A delay loop that pauses the program execution for a certain amount of time ;;;;
DELAY:
	LDX #$00
	LDY #$00
LOOP1:
	INY 		; Count up in Y register
LOOP2:
	INX			; Count up in X register
	CPX #$01	; Cycle iterations X
	BNE LOOP2	
	CPY #$01	; Cycle iterations Y
	BNE LOOP1
END:
	RTS			; End subroutine
