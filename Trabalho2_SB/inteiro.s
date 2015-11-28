section .text
global _start
_start:
call lerInteiro
mov [B], eax
mov eax, [B]
mov [R], eax
mov eax, [R]
mov esi, 0
call escreverInteiro
mov eax, 1
mov ebx, 0
int 80h
section .data
msg dd 'Ok'
tamNum equ 4
num dd 00
B: dd 00
R: dd 00
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
escreverInteiro:	
			mov edx, 0
			mov ebx, 0XA
			div ebx
			add edx, 0X30
			mov [num+esi], edx
			inc esi
			cmp eax, 0
			je f_escreverInteiro
			jmp escreverInteiro
f_escreverInteiro:	
			mov eax, 4
			mov ebx, 1
			mov ecx, num
			mov edx, 1
			int 0X80
			dec esi
			cmp esi, 0
			jne f_escreverInteiro
			ret