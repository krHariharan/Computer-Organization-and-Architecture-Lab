; Assembly Language Program using software interrupts
; Author : KR Hariharan CS19B079
; Date : 11/04/2021
; Assignment number : 7

%include "io.mac"
.DATA		
	inprompt		db	"Enter 32 bit Integer: ",0
	in_file_name		db	"input.txt",0
	mmap_arg:			; structure to pass to mmap
  		.addr:   dd 0
  		.len:    dd 4
  		.prot:   dd 1
  		.flags:  dd 2
  		.fd:     dd -1
  		.offset: dd 0
.UDATA
	number		resd	1
	fd_in 		resd	1
	
	
.CODE
	.STARTUP
	
	PutStr inprompt
	
	; create file
	mov EAX, 8
	mov EBX, in_file_name
	mov ECX, 0700
	int 0x80
	mov [fd_in],EAX
	
	; input number
	GetLInt	EAX
	mov [number], EAX
	mov EDX, 4
	mov EAX, 4
	mov EBX, [fd_in]
	mov ECX, number
	int 0x80
	
	; close file
	mov EAX, 6
	mov EBX,[fd_in]
	int 0x80
	
	;open file for read
	mov EAX, 5
	mov EBX, in_file_name
	mov ECX, 0
	mov EDX, 0700
	int 0x80
	push EAX
	
	; mmap creation
	mov [mmap_arg.fd], EAX
	mov EAX, 90
	mov EBX, mmap_arg
	int 0x80
	mov EBX, [EAX]
	mov [fd_in], EBX
	
	; forking
	mov	EAX,	2
	int	0x80
	cmp	EAX, 0
	jnz	parentProcess	
	
childProcess:
	mov ECX, 0
	mov EBX, [fd_in]
	call primeCheck
	PutLInt EBX
	nwln
	jmp end
	
parentProcess:
	; wait for child process to get over - wait_pid
	mov EAX, 7
	mov EBX, -1
	mov ECX, number
	mov EDX, 0
	int 0x80
	
	; reversing number
	mov EAX, [fd_in]
	mov ECX, 0
	call reverseNum
	nwln
	
	; unmap memory
	mov EAX, 91
	mov EBX, fd_in
	mov ECX, 4
	int 0x80

	; close file
	pop EBX
	mov EAX, 6
	int 0x80
	jmp end
	
primeCheck:
	add ECX, 2
	mov EAX, ECX
	mul ECX
	cmp EAX, EBX		; check if ECX+2 is still <= root of number, else terminated
	jg  divisorNotFound
	mov EAX, [fd_in]
	mov EDX, 0
	div ECX
	cmp EDX, 0		; check if number is divisible (remainder is 0) to terminate, else continue loop
	je divisorFound
	loop primeCheck
	
divisorNotFound:
	mov EBX, 0
	ret
	
divisorFound:
	mov EBX, 1
	ret
	
reverseNum:
	cmp EAX, 0		; if EAX is 0, all digits printed
	je return
	mov EDX, 0
	mov ECX, 10
	div ECX
	PutLInt EDX		; print smallest remaining digit
	loop reverseNum
	
return:
	ret

end:
.EXIT
	
