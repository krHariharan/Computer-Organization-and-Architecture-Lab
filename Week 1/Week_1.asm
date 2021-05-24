; Assembly Language Program to add sequence of 32 bit numbers
; Author : KR Hariharan CS19B079
; Date : 16/02/2021
; Assignment number : 1

%include "io.mac"


.UDATA
count	resw	1		; count of numbers in sequence
num	resd	1		; memory location to input number
sum	resd	1		; memory location to store sum

.CODE
	.STARTUP
start:
	mov	EAX, 0		; initializing sum value to zero
	GetInt	[count]	; input count
	mov 	ECX, [count]	
summation:
	GetInt	[num]		; input number
	mov 	EBX, [num]	
	add 	EAX, EBX	; adding new input [via EBX] to existing sum
	loop 	summation	; loop using value of count [in ECX]
summed:
	mov 	[sum], EAX	; moving summation
	PutInt	[sum]		; printing summation
.EXIT
