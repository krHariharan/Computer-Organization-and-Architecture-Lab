; Assembly Language Program to encrypt input string
; Author : KR Hariharan CS19B079
; Date : 24/02/2021
; Assignment number : 2

%include "io.mac"
.DATA
msg1	db	'input string to be encrypted: ', 0
msg2	db	'Do you want to terminate program?: ', 0		

.UDATA
instring	resb	21

.CODE
	.STARTUP
initializing:
	PutStr	msg1
	GetStr	instring	; input string to be encrypted
	mov	ECX, 21		; max length - loop count
	mov	EAX, instring	; moves address of first character
encrypt:
	cmp	byte[EAX], 48	; if byte[EAX] < 48 or '0', not a digit
	jl	cont
	cmp	byte[EAX], 57	; if byte[EAX] > 57 or '9', not a digit
	jg	cont
	add	byte[EAX], 5	; adding 5 to digit to encrypt
	cmp	byte[EAX], 57	; if byte[EAX] > 57, digit was >=5
	jle	cont
	sub	byte[EAX], 10	; if digit was initially >=5, 10 subtracted to correctly encrypt digit
cont:
	inc	EAX
	loop	encrypt		; loop
output:
	PutStr	instring	; output encrypted string
	PutCh 	10
terminateCheck:
	PutStr	msg2
	GetCh	AL
	cmp	AL, 'Y'
	jz	end
	cmp	AL, 'y'
	jnz	initializing	; repeat program if input is not y nor Y
end:				; end of program
.EXIT
