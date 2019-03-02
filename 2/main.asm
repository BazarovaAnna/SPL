global 	_start

extern 		exit
extern 		find_word
extern 		read_string
extern 		print_string
extern 		print_char

%define 	buf_length 255

section 	.data
%include 	"words.inc"
buffer: 	times buf_length+1 db 0

buff_overflow_msg: 
		db 		"Строка превышает размеры буфера.",0
word_not_found_msg: 
		db 		"Строка не найдена.",0

section 	.text
_start:
		mov 	rdi, buffer
		mov 	rsi, buf_length
		call 	read_string
		test 	rax, rax
		jz 		.buff_overflow
		mov 	rdi, buffer
		mov 	rsi, first
		call 	find_word
		test 	rax, rax
		jz 		.word_not_found
		add 	rax, 8 			;skipping next node
		xor 	r10, r10		; counter
		
	.loop: 						;skipping key-string, looping until null-terminator
		mov 	r11b, byte[rax+r10]
		cmp 	r11b, 0
		je 		.exit
		inc 	r10
		jmp 	.loop
		
	.exit:
		add 	rax, r10
		inc 	rax 			;rax points to the beginning of value
	
		mov 	rdi, rax 		;printing value
		call 	print_string
		mov 	rdi, 0x0A 		;printing \n
		call 	print_char
		call 	exit
	.buff_overflow:
		mov		rdi, buff_overflow_msg
		call 	print_string
		mov		rdi, 1
		call 	exit
	.word_not_found:
		mov		rdi, word_not_found_msg
		call 	print_string
		mov 	rdi, 0x0A 		;printing \n
		call 	print_char
		mov		rdi, 1
		call 	exit	
