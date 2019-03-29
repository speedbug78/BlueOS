    .text
    .align 2
    .thumb
    .thumb_func
    .globl User_Jump
    .type User_Jump, %function
User_Jump:

    //Disable interrupts (TODO: test if this is necessary)
    cpsid i

    //Move the stack pointer to the user's RAM
    ldr r0, =__StackTop
    msr msp, r0

    //
    ldr r0, =__User_F_Bottom
    ldr r1, [r0, #0x04]
    bx r1
