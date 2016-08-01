mov ax, 0x07c0
mov ds, ax

mov ah, 0x0
mov al, 0x13
int 0x10

mov si, msg
mov ah, 0x0C


display_pixer_loop:
    lodsb

    or al, al
    jz hang
    mov cx, 0x10
    mov dx, 0x10
    int 0x10

    jmp display_pixer_loop

msg:
    db 10, 11, 12, 13, 0

hang:
    jmp hang

    times 510-($-$$) db 0

    db 0x55
    db 0xAA
