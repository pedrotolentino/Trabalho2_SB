section .text
global _start
_start:
call lerInteiro
mov [B], eax
call lerInteiro
mov [H], eax
mov eax, B
mov ebx, H
mul ebx
mov ebx, DOIS
div ebx
mov [R], eax
mov eax, R
call escreverInteiro
mov eax, 1
mov ebx, 0
int 80h
section .data
tamNum equ 4
num dd 00
B: dd 00
H: dd 00
R: dd 00
DOIS: db 2
lerInteiro:	mov eax, 3
		mov ebx, 0
		mov ecx, num
		mov edx, tamNum
		int 0x80
		mov esi, 0
		mov eax, 0
		mov ebx, 0
converteNum:	mov BYTE bl, [num+esi]
		cmp bl, 0xA
		je f_lerInteiro
		sub bl, 0x30
		mov edx, 0xA
		mul edx
		movzx edx, bl
		add eax, edx
		inc esi
		mov ebx, 0
		jmp converteNum

		f_lerInteiro: ret
escreverInteiro:	mov edx, 0
			mov ebx, 0XA
			div ebx
			add edx, 0X30
			mov [num+esi], edx
			inc esi
			cmp eax, 0
			je f_escreverInteiro
			jmp escreverInteiro
f_escreverInteiro:	mov eax, 4
			mov ebx, 1
			mov ecx, num
			mov edx, tamNum
			int 0X80
			ret

