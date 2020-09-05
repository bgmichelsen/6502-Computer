START:
	LDA #$01	; Load the accumulator with 1
	STA $B402	; Store that at the Direction Control B register in the 65C22
	STA $B400	; Output a "1" to the PORTB register to turn on an LED
END:
	JMP END		; Endless loop