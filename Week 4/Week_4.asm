; Assembly Language Program to perform Binary Search
; Author : KR Hariharan CS19B079
; Date : 11/03/2021
; Assignment number : 4

%include "io.mac"
.DATA		
	MAX_SIZE	equ	100
	msg1		db	"Enter number of elements: ",0
	msg2		db	"Enter elements in ascending order: ",0
	msg3		db	"Enter element to be searched: ", 0
.UDATA
	array		resd	MAX_SIZE
	first		resd	1	; storing
	last		resd	1	; index
	mid		resd	1	; positons
	num		resd	1	; number to search for
.CODE
	.STARTUP
	PutStr		msg1
	GetInt		CX			; size of array	
	mov		[last],	CX
	PutStr		msg2
	mov		EAX,		array
input_loop:					; loop to input array
	GetLInt	EBX
	mov		[EAX],		EBX
	add		EAX,		4
	loop		input_loop
	
	PutStr		msg3
	GetLInt	[num]			; number to search for
	
	sub		dword[last],	1	; last array index
	mov		dword[first],	0	; first array index
	
binary_search:
	mov		EAX, 		[first]
	mov		ECX, 		[last]
	cmp		EAX, 		ECX	; check if first <= last
	jg		not_found		; if first > last, element not in array
	add		EAX, ECX
	mov		EDX, 0
	mov		ECX, 2
	div		ECX			; mid = (first+last)/2
	mov		[mid], EAX
	mov		ECX, 4
	mul		ECX
	add		EAX, array		; array[mid] = [array + 4*mid]
	mov		EBX, [EAX]		; array[mid]
	mov		ECX, [num]		
	cmp		ECX, EBX		; compare array[mid] and num
	jl		less_than		; if num < array[mid], num can only be in fist half of array
	jg		greater_than		; if num > array[mid], num can only be in second half of array
	PutInt		1			; if neither jump statement is invoked, num = array[mid] -> element found
	jmp		end	
	
less_than:
	mov		EAX, [mid]
	sub		EAX, 1
	mov		[last], EAX		; last moved to mid - 1 -> second half of array discarded
	jmp		binary_search

greater_than:
	mov		EAX, [mid]
	add		EAX, 1
	mov		[first], EAX		; first moved to mid + 1 -> first half of array discarded
	jmp		binary_search
	
not_found:
	PutInt		0			; element not found
end:
.EXIT
	
