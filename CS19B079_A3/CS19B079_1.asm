; Assembly Language Program to find nth Fibonacci number recursively
; Author : KR Hariharan CS19B079
; Date : 03/03/2021
; Assignment number : 3

%include "io.mac"
.DATA		
spacer	db	', ', 	0


.UDATA
startTimeD	resd	1
startTimeA	resd	1

.CODE
	.STARTUP
initializing:	
	rdtscp					; starting timestamp
	mov 		[startTimeD], 	EDX	; storing start timestamp
	mov 		[startTimeA], 	EAX
	GetLInt	EAX			; input string to be encrypted
	call 		fact			; calling function to find Fibonacci number
	PutLInt	EAX			; print Fibonacci number
	rdtscp					; ending timestamp
	PutStr 	spacer
	PutLInt 	[startTimeD]		; printing start timestamp
	PutStr 	spacer
	PutLInt 	[startTimeA]
	PutStr 	spacer
	PutLInt 	EDX			; printing end timestamp
	PutStr 	spacer
	PutLInt 	EAX
	nwln
	jmp		end


fact: 
	cmp 		EAX,		1	; if n<=1, f(n) = n
	jle 		done
	Push 		EAX			; push n
	sub 		EAX,		1
	call 		fact			; calling f(n-1)
	mov 		EBX,		EAX
	Pop 		EAX			; pop back n to get n-2
	Push 		EBX			; push in f(n-1)
	sub 		EAX,		2			
	call 		fact			; calling f(n-2)
	Pop 		EBX			; pop back f(n-1)
	add 		EAX,		EBX	; f(n) = f(n-1) + f(n-2)
done:
	ret
	
end:
.EXIT
