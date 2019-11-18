.intel_syntax noprefix
.text
.global _main
_main:
	push	rbp
	mov rbp, rsp
	xor eax, eax
	mov rax, 20
	push 20
	mov rax, 64
	push 64
	mov rax, 4
	push 4
	mov rax, 4
	push 4
	pop rbx
	pop rax
	add eax, ebx
	push rax
	pop rbx
	pop rax
	sub eax, ebx
	push rax
	pop rbx
	pop rax
	mov edx, 0
	idiv ebx
	push rax
	pop rbx
	pop rax
	sub eax, ebx
	push rax
	mov rsp, rbp
	pop rbp
	ret
