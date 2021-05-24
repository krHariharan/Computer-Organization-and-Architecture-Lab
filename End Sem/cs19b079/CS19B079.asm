; Program to Implement 2 subtasks on arrays
; Author : KR Hariharan CS19B079
; Date : 08/05/2021
; End Semester Examination

%include "io.mac"
.DATA		
	MAX_SIZE	equ	100
	error_msg	db	"Invalid Choice",0
.UDATA
	array		resd	MAX_SIZE
	pos		resd	1
	sum		resd	1
	size		resd 	1
	choice		resw	1
	
.CODE
	.STARTUP
start:
	GetInt [choice]
	GetLInt ECX
	mov [size], ECX
	mov EBX, array
input_loop:
	GetLInt EAX
	mov [EBX], EAX
	add EBX, 4
	loop input_loop
	
	mov CX, [choice]
	cmp CX,1
	je sum_even
	cmp CX, 2
	je invert_array
	PutStr error_msg
	jmp end
	
sum_even:
	mov ECX, [size]
	mov EBX, array
	mov [pos], EBX
	mov dword[sum], 0
sum_iterator:
	mov EBX, [pos]
	mov EAX, [EBX]
	mov EDX, 0
	mov EBX, 2
	div EBX
	cmp EDX, 1
	je cont
	mov EAX, [sum]
	mov EBX, [pos]
	add EAX, [EBX]
	mov [sum], EAX
cont:
	mov EBX, [pos]
	add EBX, 4
	mov [pos], EBX
	loop sum_iterator
	mov EAX, [sum]
	PutLInt EAX
	jmp end
	
invert_array:
	mov ECX, [size]
	mov EAX, 4
	mul ECX
	sub EAX, 4
	mov EBX, array
	add EAX, EBX
	
invert_iterator:
	mov EDX, [EAX]
	xchg [EBX], EDX
	mov [EAX], EDX
	add EBX, 4
	sub EAX, 4
	cmp EAX, EBX
	jg invert_iterator
	
	mov ECX, [size]
	mov EBX, array
print_iterator:
	PutLInt [EBX]
	nwln
	add EBX, 4
	loop print_iterator
	
end:
.EXIT
