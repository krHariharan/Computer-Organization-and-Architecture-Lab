; Assembly Language Program to find nth Fibonacci number iteratively
; Author : KR Hariharan CS19B079
; Date : 03/03/2021
; Assignment number : 3

%include "io.mac"
.DATA		
spacer	db	',',	0


.UDATA
startTimeD	resd	1
startTimeA	resd	1

.CODE
	.STARTUP
initializing:
	rdtscp					; starting timestamp
	mov 		[startTimeD],	edx	; storing starting timestamp
	mov 		[startTimeA],	eax
	GetLInt	ecx			; input string to be encrypted
	cmp 		ecx,		1	; if n<=1, f(n) = n
	jle 		Output
	mov 		eax,		0	; eax holds f(i-2)
	mov 		ebx,		1	; ebx hold f(i-1)
	sub 		ecx,		1	; n -> n-1 loops
iterative:
	add 		eax,		ebx	; f(i) = f(i-2) + f(i-1)
	mov 		edx,		eax
	mov 		eax,		ebx
	mov 		ebx,		edx
	loop 		iterative
	mov 		ecx,		edx	; move f(n) to ecx
Output:
	PutLInt 	ecx			; print Fibonacci number
	rdtscp					; ending timestamp
	PutStr 	spacer			
	PutLInt 	[startTimeD]		; printing start timestamp
	PutStr 	spacer
	PutLInt 	[startTimeA]		
	PutStr 	spacer
	PutLInt 	edx			; printing end timestamp
	PutStr 	spacer
	PutLInt 	eax
	nwln
.EXIT
