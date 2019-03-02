global 	find_word

extern 	string_equals

section .text:

find_word:
 
	;rdi - points to key, 
	;rsi - points to last word
		mov		r10, rdi ;r10 key word
		mov		r11, rsi ;r11 next word
	.loop:	
		mov		rdi, r10
		lea		rsi, [r11 + 8]
		call 	string_equals
		jnz		.found
		mov		r11, [r11]
		test 	r11b, r11b
		jz		.notFound
		jmp		.loop
	.found:
		mov		rax, r11
		ret
	.notFound:
		xor		rax, rax
		ret
