section .text
global _start
_start:
call lerInteiro
mov DWORD [B], eax
call lerInteiro
mov DWORD [H], eax
mov eax, [B]
mov ebx, [H]
mul ebx
mov ebx, [DOIS]
div ebx
mov DWORD [R], eax
mov eax, [R]
mov esi, 0
call escreverInteiro
mov eax, 1
mov ebx, 0
int 0X80
section .data
msg dd 'Ok'
tamNum equ 4
num dd 0
B: dd 0
H: dd 0
R: dd 0
DOIS: dd 2
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
		cmp DWORD eax, 0
		je f_escreverInteiro
		jmp escreverInteiro
		f_escreverInteiro:	mov eax, 4
		mov ebx, 1
		lea ecx, [num+esi]
		mov edx, 1
		int 0X80
		dec esi
		cmp DWORD esi, 0
		jge f_escreverInteiro
		ret
