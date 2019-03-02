global string_length
global print_char
global print_newline
global print_string
global print_uint
global print_int
global string_equals
global parse_uint
global parse_int
global read_word
global string_copy
global exit
global read_string

section .text
string_length:
     xor rax, rax
        .loop:
            cmp byte[rdi+rax], 0x00;сравниваем значение текущего символа с 0
            je .exit ;если равны, то строка кончилась
            inc rax	;если нет идем в следующий символ
        jmp .loop
        .exit:
            ret
			
print_string:
        call string_length ;считаем длину строки
        mov rdx, rax 
        mov rax, 1 ;системный вызов write
        mov rsi, rdi
        mov rdi, 1
        syscall
            ret
			
print_char:
    push rdi ;сохраняем rdi, для вызова print_string
    mov rdi, rsp ;передаем указатель на символ
    call print_string
    pop rdi ;достаем сивмвол, восстанавливаем стек
    ret
	
print_newline:
    xor rax, rax
    mov rdi, '\n' ;печатаем сивмвол переноса строки
    jmp print_char
	
print_uint:
    mov r8, rsp ;сохраняем значение регистра rsp
    mov rax, rdi; записываем исходное число в rax
    mov r9, 10; сохраняем делитель (10) в регистре r9
    push 0; записываем 0 для окончания строки
    .loop:
		xor rdx, rdx; чистим регистр от отсатка предыдущего шага
		div r9; деление на 10, целое в ax, остаток в dx
		add dl, 0x30; приведение числа в формат символа
		dec rsp
		mov [rsp], dl; push полученного символа(1,байт) в стек
		cmp rax, 0; сравнение целой части при делении с нулем
    jne .loop
    mov rdi, rsp ;указатель на начало выводимой строки
    call print_string ; вызов функции вывода строки
    mov rsp, r8 ;восстанавливаем значение rsp
    ret
	
print_int:
    mov r10, rdi
    sub r10, 0 ;устанавливаем флаги
    jns print_uint ;если sf!=1, то положительное и вызов для печати беззнакового
    mov rdi, 0x2D ;если sf==1 - отрицательное, печатаем минус 
    call print_char
    neg r10 ;модуль числа	
    mov rdi, r10
    call print_uint; печатаем как беззнаковое
    ret
		
string_equals:
	xor	rcx, rcx
	xor	rax, rax
	.loop:
		mov	r8b, byte[rdi+rcx]
		mov	r9b, byte[rsi+rcx]
		inc	rcx
		cmp	r8b, r9b ;cmp symbols
		jnz	.not_equals ;if not equal -> end; else
		cmp	r8b, 0 ;if not null-terminator, then continue
	jnz	.loop ;else equals
	inc	rax ;return 1, if equals
	ret
	.not_equals:
		xor	rax, rax
		ret

read_char:
    xor rax, rax
    push rax ;записываем 0 - конец строки
    xor rdi, rdi ;системный вызов read
    mov rsi, rsp
    mov rdx, 1
    syscall
    pop rax ;восстанавливаем стек
    ret
	
read_word:
    xor r10, r10
    mov r8, rdi ; адрес буфера
    mov r9, rsi ; размер буфера
    .loop:
        call read_char ;считываем символы до конца строки 
        cmp rax, 0
        je .err
        cmp rax, 0x20 ;если пробельный символ, то продожаем пока
    jbe .loop	;не дойдем до других символов
    mov [r8], rax ;как только непробельный его сразу в буфер
    .loop2:
		inc r10
		cmp r10, r9 ;проверям не кончился ли буфер
        je .err ; если кончился, то возвращаем 0 и выходим
        call read_char ;если нет, то идем считывать дальше
        cmp rax, 0x20 ; пока непробельный(или конец строки)
        jbe .exit
        mov [r8+r10], rax ; складываем в буфер
        jmp .loop2
        .err:
            mov r9, 1
            mov r8, 0
    .exit:
    mov rdx, r10
    mov rax, r8
    ret
	
; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    xor r10, r10
    xor rdx, rdx
    .loop:
        mov dl, [rdi+r10] ;считываем побайтово символы
        cmp dl, 0x30 ;пропускаем цифры от 0 до 9 - коды 0х30..0х39
        jb .fail
        cmp dl, 0x39
        ja .fail
        inc r10
        sub dl, 0x30 ;восстанавливаем из кодов значения цифр
        imul rax, 10 ;умножаем на 10
        add rax, rdx ;прибавляем цифру
    jmp .loop
	.fail:
       cmp r10, 0 ;если не цифра - то смотрим на общее число считанных символов
       je .clear ;если 0, то по нулям и выход
       mov rdx, r10; если нет, то пишем длину считанного числа
    .exit:
        ret
    .clear:
        xor rax, rax
        xor rdx, rdx
    jmp .exit

parse_int:
        cmp byte[rdi], 0x2D;проверка первого символа на -
        je .minus
        call parse_uint ;если не минус, то считываем как беззнаковое
        .exit:
            ret
        .minus:
            inc rdi ;сдвиг за счет прочитанного символа -
            call parse_uint ;считываем как беззнаковое
            jz .letters ;если ничего не считали, значит символы - по нулям и выход
            neg rax ;восстанавливаем значение числа
            inc rdx ; учет длины числа(минуса)
        jmp .exit
        .letters:
            xor rax, rax
            xor rdx, rdx
        jmp .exit
		
string_copy:
    xor rcx, rcx 
    .loop:
		cmp rcx, rdx ;сравнение длин строки и буфера
		ja .zero ; если длина строки>длины буфера,то возвращаем 0
        mov al, byte[rdi+rcx] ; в al символ
        mov byte[rsi+rcx], al ;символ в буфер
        inc rcx ;следующий символ
    jmp .loop
	.zero:
        mov rax, 0
    ret

exit:
;sys_exit
        mov rax, 60
		xor rdi, rdi
        syscall
		
read_string:
	mov	r9, rdi;buffer
	mov	r8, rsi ;buffer length
	xor	r10, r10 ;счетчик
	.loop:
		call read_char
		cmp al, 0xA ;check if string ends with \n
		jz .end
		test al, al
		jz .end
		mov	byte[r9+r10], al
		inc r10
		cmp r8, r10
		jb .read_error
	jmp .loop
	.end:
		mov	byte[r9+r10], 0
		mov	rax, r9
		mov	rdx, r10
		ret
	.read_error: ;return 0 if string_length>buffer length
		xor	rax, rax
		ret
	