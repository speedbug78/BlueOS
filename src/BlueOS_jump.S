    .text
    .align 2
    .thumb
    .thumb_func
    .globl User_Jump
    .type User_Jump, %function
User_Jump:

    //Disable interrupts (TODO: test if this is necessary)
    cpsid i

    //Not pushing the working registers onto the stack will
    //prevent coming back to the same state.

    //Move the stack pointer to the user's RAM
    ldr r0, =__StackTop
    msr msp, r0

    //Load the user code address into r0
    ldr r0, =__User_F_Bottom
    //Load a Thumb offset address into r1
    ldr r1, [r0, #0x04]
    //Jump to the address in r1
    bx r1
