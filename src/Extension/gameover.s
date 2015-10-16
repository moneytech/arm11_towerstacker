ldr r2,=0x20200008
ldr r1,=0x20200004
ldr r0,=0x20200000

mov r3,#73
lsl r3,#6
add r3,#0x08000000
str r3,[r0]

mov r3,#9
lsl r3,#21
add r3,#0x00009000
add r3,#0x00000009
str r3,[r1]

mov r3,#0
add r3,#0x200000
add r3,#0x009200
add r3,#0x000040
str r3,[r2]

mov r2,#1

/* r4 is score*/
mov r4,#0
/* r5 is game status*/
/* bit0 = bool isGameOver*/
/* bit1-9 = gameover char*/
mov r5,#0
/* r6,r8 is reserved for animation*/
/* r7 is animation position */
mov r7,#0

main:
mov r1,#1
lsl r1,#2
str r1,[r0,#40]

tst r2,#0x000001
beq check2
/* set gpio2*/
mov r1,#1
lsl r1,#2
str r1,[r0,#28]


check2:
mov r1,#1
lsl r1,#3
str r1,[r0,#40]

tst r2,#0x000002
beq check3
/* set gpio3*/
mov r1,#1
lsl r1,#3
str r1,[r0,#28]

check3:
mov r1,#1
lsl r1,#4
str r1,[r0,#40]

tst r2,#0x000004
beq check4
/* set gpio4*/
mov r1,#1
lsl r1,#4
str r1,[r0,#28]

check4:
mov r1,#1
lsl r1,#9
str r1,[r0,#40]

tst r2,#0x000008
beq check5
/* set gpio9*/
mov r1,#1
lsl r1,#9
str r1,[r0,#28]

check5:
mov r1,#1
lsl r1,#10
str r1,[r0,#40]

tst r2,#0x000010
beq check6
/* set gpio10*/
mov r1,#1
lsl r1,#10
str r1,[r0,#28]

check6:
mov r1,#1
lsl r1,#11
str r1,[r0,#40]

tst r2,#0x000020
beq check7
/* set gpio11*/
mov r1,#1
lsl r1,#11
str r1,[r0,#28]

check7:
mov r1,#1
lsl r1,#14
str r1,[r0,#40]

tst r2,#0x000040
beq check8
/* set gpio14*/
mov r1,#1
lsl r1,#14
str r1,[r0,#28]

check8:
mov r1,#1
lsl r1,#15
str r1,[r0,#40]

tst r2,#0x000080
beq check9
/* set gpio15*/
mov r1,#1
lsl r1,#15
str r1,[r0,#28]

check9:
mov r1,#1
lsl r1,#17
str r1,[r0,#40]

tst r2,#0x000100
beq check10
/* set gpio17*/
mov r1,#1
lsl r1,#17
str r1,[r0,#28]

check10:
mov r1,#1
lsl r1,#18
str r1,[r0,#40]

tst r2,#0x000200
beq check11
/* set gpio18*/
mov r1,#1
lsl r1,#18
str r1,[r0,#28]

check11:
mov r1,#1
lsl r1,#22
str r1,[r0,#40]

tst r2,#0x000400
beq check12
/* set gpio22*/
mov r1,#1
lsl r1,#22
str r1,[r0,#28]

check12:
mov r1,#1
lsl r1,#23
str r1,[r0,#40]

tst r2,#0x000800
beq check13
/* set gpio23*/
mov r1,#1
lsl r1,#23
str r1,[r0,#28]

check13:
mov r1,#1
lsl r1,#24
str r1,[r0,#40]

tst r2,#0x001000
beq end
tst r2,#0x002000
beq end
tst r2,#0x004000
beq end
/* set gpio24*/
mov r1,#1
lsl r1,#24
str r1,[r0,#28]

end:
mov r1,#0x5E0000
wait:
    /* if gpio gplev = 1 stop*/
/*
     ldr r1,[r0,#52]
    tst r1,////25
    be set
*/
sub r1,#1

cmp r1,#0
bne wait

/* check isOver (bit0) */
tst r5,#0x000001
bne over

lsl r2,#1
cmp r2,#0x008000
ble main
mov r5,#1
b main

/*game over animation*/

over:

/*check stage 10*/
tst r5,#0x000200
beq stageR
mov r6,#0
b animate

/*check stage R*/
stageR:
tst r5,#0x000100
beq stageE2
cmp r7,#0
bne animate
mov r6,#0x00005D
lsl r6,#8
add r6,r6,#0x0000EF
b animate

/*check stage E2*/
stageE2:
tst r5,#0x000080
beq stageV
cmp r7,#0
bne animate
mov r6,#0x000079
lsl r6,#8
add r6,r6,#0x0000E7
b animate

/*check stage V*/
stageV:
tst r5,#0x000040
beq stageO
cmp r7,#0
bne animate
mov r6,#0x000005
lsl r6,#8
add r6,r6,#0x00006D
add r7,#1
b animate

/*check stage O*/
stageO:
tst r5,#0x000020
beq stageE1
cmp r7,#0
bne animate
mov r6,#0x00007B
lsl r6,#8
add r6,r6,#0x00006F
b animate

/*check stage E1*/
stageE1:
tst r5,#0x000010
beq stageM
cmp r7,#0
bne animate
mov r6,#0x000079
lsl r6,#8
add r6,r6,#0x0000E7
b animate

/*check stage M*/
stageM:
tst r5,#0x000008
beq stageA
cmp r7,#0
bne animate
mov r6,#0x00000B
lsl r6,#8
add r6,r6,#0x00007D
add r7,#1
b animate

/*check stage A*/
stageA:
tst r5,#0x000004
beq stageG
cmp r7,#0
bne animate
mov r6,#0x00005B
lsl r6,#8
add r6,r6,#0x0000EF
b animate

/*check stage G*/
stageG:
tst r5,#0x000002
beq stage0
cmp r7,#0
bne animate
mov r6,#0x00007B
lsl r6,#8
add r6,r6,#0x0000E7
b animate

/*stage0 */
stage0:
mov r2,#0
mov r5,#3
b main

/*movement*/
animate:
lsl r2,#3
and r8,r6,#7
lsl r8,#1
orr r2,r2,r8, lsr #1
mov r6, r6, lsr #3
add r7,r7,#1
cmp r7,#7
blt main
mov r7,#0
lsl r5,#1
add r5,r5,#1
b main


/*end of animation*/
