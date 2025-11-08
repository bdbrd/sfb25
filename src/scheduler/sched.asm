
global switch_context
global get_context

;  switch_context(struct context *old, struct context *new)
;                       ^RDI                ^RSI 
switch_context:

    pop r8 ; Fetch RIP from stack

    ; Save callee-saved registers to the old struct
    mov [rdi + 0x00], rbx
    mov [rdi + 0x08], rsp
    mov [rdi + 0x10], rbp
    mov [rdi + 0x18], r12
    mov [rdi + 0x20], r13
    mov [rdi + 0x28], r14
    mov [rdi + 0x30], r15
    mov [rdi + 0x38], r8

    ; Load the new context
    mov rbx, [rsi + 0x00]
    mov rsp, [rsi + 0x08]
    mov rbp, [rsi + 0x10]
    mov r12, [rsi + 0x18]
    mov r13, [rsi + 0x20]
    mov r14, [rsi + 0x28]
    mov r15, [rsi + 0x30]

    push r8

    jmp [rsi + 0x38]

    ret


; get_context(struct context *ctx)
get_context:
    pop r8 ; Fetch RIP from stack

    ; Save callee-saved registers to the old struct
    mov [rdi + 0x00], rbx
    mov [rdi + 0x08], rsp
    mov [rdi + 0x10], rbp
    mov [rdi + 0x18], r12
    mov [rdi + 0x20], r13
    mov [rdi + 0x28], r14
    mov [rdi + 0x30], r15
    mov [rdi + 0x38], r8

    push r8

    ret