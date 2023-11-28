#define ASM
.global sig_code_start, sig_code_end
sig_code_start:
    movl    $10, %eax   /* sigreturn system call number */
    int     $0x80
    ret
sig_code_end:
    .long   0x00
