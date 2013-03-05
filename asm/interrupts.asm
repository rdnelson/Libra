;this is a comment
dw	INT0
dw	0
dw	INT1
dw	0
dw	INT2
dw	0
dw	INT3
dw	0

main:
	mov ax, 0
	int 0
	int 1
	int 2
	int 3
	hlt
INT0:
	mov dx, 04E9h
	mov al, '0'
	out [dx], al
	iret
INT1:
	mov dx, 04E9h
	mov al, '1'
	out [dx], al
	iret
INT2:
	mov dx, 04E9h
	mov al, '2'
	out [dx], al
	iret
INT3:
	mov dx, 04E9h
	mov al, '3'
	out [dx], al
	iret

END	main
