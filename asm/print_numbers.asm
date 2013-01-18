code:
inc cx
cmp cx, 0
jz fin
mov ax, cx
call print_num
jmp code
start:
mov cx, 0
jmp code
fin:
hlt

print_num:
push ax
push bx
push cx
push dx
mov cx, 0
mov bx, 10
loop:
mov dx, 0
cmp ax, 0
jz print
div bx
push dx
inc cx
jmp loop
print:
mov dx, 04E9h
prt_loop:
cmp cx, 0
jz done
pop ax
add al, '0'
out [dx], al
dec cx
jmp prt_loop:
done:
mov al, 10
out [dx], al
mov al, 13
out [dx], al
pop dx
pop cx
pop bx
pop ax
ret


end start
