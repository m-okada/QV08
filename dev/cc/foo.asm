
# declare exit
# declare getchar
# declare malloc
# declare putchar
	mov a0,pc
	add a0,5
	push a0
	jmp.s main
	call exit
:main
# declare putnumber
# declare putmessage
# declare main
	push w0
	lea a0, sp+0
	push a0
	movi w0,20
	pop a0
	st [a0],w0
	lea a0, sp+0
	ld w0,[a0]
	inc w0
	st [a0],w0
	mov a0,181
	push a0
	ld a0, [sp+0]
	call a0
	add sp, 2
	mov a0,198
	push a0
	lea a0, sp+2
	ld w0,[a0]
	push w0
	movi w0,200
	pop w1
	mul w0
	push w0
	ld a0, [sp+2]
	call a0
	add sp, 4
	mov a0,144
	push a0
	movi w0,10
	push w0
	ld a0, [sp+2]
	call a0
	add sp, 4
	mov a0,251
	push a0
	movi w0,32767
	push w0
	ld a0, [sp+2]
	call a0
	add sp, 4
	movi w0,0
	add sp, 2
	ret
	add sp, 2
	ret
# declare Msg
# declare _i
# declare putmessage
	movi w0,0
	push w0
	mov a0,286
	push a0
	movi w0,0
	pop a0
	st [a0],w0
	mov a0,284
	push a0
	lea a0,String0
	pop a0
	st [a0],w0
:label_w_0
	movi w0,1
	jrz w0,label_w_e0
	mov a0,284
	ld w0,[a0]
	push w0
	lea a0, sp+2
	ld w0,[a0]
	pop w1
	add w0,w1
	ld b0,[a0]
	sxt w0
	push w0
	movi w0,0
	pop w1
	cmp w1,w0
	sete w0
	jrz w0,label_if_1
	add sp, 2
	ret
	jmp label_e_1
:label_if_1
:label_e_1
	mov a0,144
	push a0
	mov a0,284
	ld w0,[a0]
	push w0
	lea a0, sp+4
	ld w0,[a0]
	pop w1
	add w0,w1
	ld b0,[a0]
	sxt w0
	push w0
	ld a0, [sp+2]
	call a0
	add sp, 4
	lea a0, sp+0
	push a0
	lea a0, sp+2
	ld w0,[a0]
	push w0
	movi w0,1
	pop w1
	add w0,w1
	pop a0
	st [a0],w0
	jmp label_w_0
:label_w_e0
	add sp, 2
	ret
# declare putnumber
	movi w0,0
	push w0
	movi w0,10000
	push w0
	movi w0,0
	push w0
	lea a0, sp+8
	ld w0,[a0]
	push w0
	movi w0,0
	pop w1
	cmp w1,w0
	sete w0
	jrz w0,label_if_2
	mov a0,144
	push a0
	movi w0,48
	push w0
	ld a0, [sp+2]
	call a0
	add sp, 4
	add sp, 6
	ret
	jmp label_e_2
:label_if_2
:label_e_2
:label_w_3
	movi w0,1
	jrz w0,label_w_e3
	lea a0, sp+0
	push a0
	lea a0, sp+10
	ld w0,[a0]
	push w0
	lea a0, sp+6
	ld w0,[a0]
	pop w1
	div w0
	mov w0,w1
	pop a0
	st [a0],w0
	lea a0, sp+0
	ld w0,[a0]
	push w0
	movi w0,0
	pop w1
	cmp w1,w0
	setne w0
	push w0
	lea a0, sp+6
	ld w0,[a0]
	push w0
	movi w0,1
	pop w1
	cmp w1,w0
	sete w0
	pop w1
	or w0,w1
	jrz w0,label_if_4
	mov a0,144
	push a0
	movi w0,48
	push w0
	lea a0, sp+4
	ld w0,[a0]
	pop w1
	add w0,w1
	push w0
	ld a0, [sp+2]
	call a0
	add sp, 4
	jmp label_e_4
:label_if_4
:label_e_4
	lea a0, sp+0
	ld w0,[a0]
	push w0
	movi w0,0
	pop w1
	cmp w1,w0
	setne w0
	jrz w0,label_if_5
	lea a0, sp+4
	push a0
	movi w0,1
	pop a0
	st [a0],w0
	jmp label_e_5
:label_if_5
:label_e_5
	lea a0, sp+2
	ld w0,[a0]
	push w0
	movi w0,1
	pop w1
	cmp w1,w0
	sete w0
	jrz w0,label_if_6
	add sp, 6
	ret
	jmp label_e_6
:label_if_6
:label_e_6
	lea a0, sp+8
	push a0
	lea a0, sp+10
	ld w0,[a0]
	push w0
	lea a0, sp+4
	ld w0,[a0]
	push w0
	lea a0, sp+8
	ld w0,[a0]
	pop w1
	mul w0
	pop w1
	sub w1,w0
	mov w0,w1
	pop a0
	st [a0],w0
	lea a0, sp+2
	push a0
	lea a0, sp+4
	ld w0,[a0]
	push w0
	movi w0,10
	pop w1
	div w0
	mov w0,w1
	pop a0
	st [a0],w0
	jmp label_w_3
:label_w_e3
	add sp, 6
	ret