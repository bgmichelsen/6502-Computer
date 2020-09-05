START:
  LDA #$FF    ; Set all bits in PORTB to be output bits
  STA $B402
  LDA #$01    ; Start the count with 1 (so that a shift can work)
  STA $B400   ; Output to PORTB
  JSR DELAY   ; Delay for a bit
LEFT:
  ASL         ; Shift left
  STA $B400   ; Output to PORTB
  JSR DELAY   ; Delay for a bit
  CMP #$80    ; Cycle until 128 is reached
  BNE LEFT
RIGHT:
  LSR         ; Right shift
  STA $B400   ; Output to PORTB
  JSR DELAY   ; Delay for a bit
  CMP #$01    ; Cycle until we're back down to 1
  BNE RIGHT   
  JMP LEFT    ; Repeat
DELAY:        ; Delay subroutine
  LDY #$00    ; Initialize X and Y registers
  LDX #$00
LOOP1:
  INY         ; Count Y up
LOOP2:
  INX         ; Count X up
  CPX #$02    ; Cycles for X
  BNE LOOP2
  CPY #$01    ; Cycles for Y
  BNE LOOP1
  RTS         ; Return from subroutine