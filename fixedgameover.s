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

/* r2 is the display register */
/* at first set to 0, meaning blank screen */
mov r2,#0

/* r4 is score*/
mov r4,#0
/* r5 is game status*/
/* bit0 = bool isGameOver*/
/* bit1-9 = gameover char*/
mov r5,#0
/* r6,r8 is reserved for animation*/
/* r7 is animation position */

/* r9 is the flash register */
/* r10 is the row register */
mov r9,#1
mov r10,#1
/* ------- */

/*for random generation*/
ldr r6,=0xab1cd2ef
ldr r7,=0x9b73e1ca


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



// -------------------------------------------------------------- the main wait -------------//

mov r1,#0x2E0000

tst r5,#0x000400
beq skip3
tst r2,#0
beq wait

skip3:
tst r5,#0x000002
beq skip2
mov r1,#0x1E0000
bne wait

skip2:
tst r5,#0x000001
bne waitEnd

// wait time - score*2000
mov r8,r4, lsl #0
mov r11,#0x01E000
mul r8,r8,r11
sub r1,r1,r8,lsl #0
tst r7,#1
bne rand
cmp r1,#0x100000
bge wait
mov r1,#0x100000
cmp r4,#25
ble wait
/*setting bit 0 of the r7 to be 1 */
add r7,r7,#1


//setting random speed //
rand:
mul r6,r6,r7
mov r8,#0x0000ff
lsl r8,#8
add r8,r8,#0x0000ff
lsl r8,#4
add r8,r8,#0x00000f
and r6,r6,r8,lsl #0
add r6,r6,#0x0a0000
mov r1,r6, lsl #0

//check for bounds//


wait:
    /* if gpio gplev = 1 stop*/

    ldr r8,[r0,#52]
    tst r8,#0x2000000
    //mov r11,#1
    //tst r8,r11,lsl #8
    beq buttonNotClick
        /*button clicked*/
            // change random pattern if it is in random //
            tst r7,#1
            beq skip4
            mov r6,r6,lsr #10
            mul r7,r7,r9
            orr r7,#1
            skip4:
            //-----------------------//

        // -- smaller wait -- //
    mov r12,#0x100000
    smallwait:
    ldr r8,[r0,#52]
    tst r8,#0x2000000
    bne smallwait
    sub r12,r12,#1
    cmp r12,#0
    bne smallwait


        // testing the r9
        cmp r4,#0
        beq successfulMove

        mov r8,#14      
        // 000...001110
        and r8,r8,r2,lsl #1

        tst r8,#8
        beq skip
        mov r8,#1
        skip:

        cmp r9,r8
        beq successfulMove
        mov r5,#1
        //bne beforegameover?
        successfulMove:
        add r4,r4,#1
        //-------------
        // if r10 = 4 ie at row 4, shift right r2 by 9 ie move display down 3 rows
        //    and decrement r10 by 3 
        tst r10,#8
        beq notRow4
        mov r2,r2,lsr #9
        mov r10,r10,lsr #3
        notRow4:

        lsl r10,#1
        b waitEnd
    buttonNotClick:
sub r1,r1,#1

cmp r1,#0
bne wait
waitEnd:
// ------------------------------------------------------------------------------------------//




/* check score <1000*/
mov r8,#1000
sub r8,r8,#1
cmp r4,r8
blt testOver
mov r5,#1

/* check isOver (bit0) */
testOver:
tst r5,#0x000001
bne over

/** check  what row it is in -> clear the whole row -> add flash register to display register **/
/* check which row */
tst r10,#1
bne clearFlashRow1
tst r10,#2
bne clearFlashRow2
tst r10,#4
bne clearFlashRow3
tst r10,#8
bne clearFlashRow4

clearFlashDone:
lsl r9,#1
tst r9,#8
beq endFlashRow
// if r9 = 8 means it goes off the edge, so set r9 to be 1 again
mov r9,#1
b endFlashRow

endFlashRow:
b main


//-----------------------------------------------------------------------loop end -----------------------//


/* flashing */
clearFlashRow1:
ldr r1,=0xfffffff8
and r2,r2,r1, lsr #0
//no shift in flash register r9
add r2,r2,r9
b clearFlashDone

clearFlashRow2:
ldr r1,=0xffffffc7
and r2,r2,r1, lsr #0
lsl r9,#3
add r2,r2,r9
mov r9,r9,lsr #3
b clearFlashDone

clearFlashRow3:
//clear row 3 // TO DO
ldr r1,=0xfffffe3f
and r2,r2,r1, lsr #0
lsl r9,#6
add r2,r2,r9
//shift right?
mov r9,r9,lsr #6
b clearFlashDone

clearFlashRow4:
ldr r1,=0xfffff1ff
and r2,r2,r1, lsr #0
lsl r9,#9
add r2,r2,r9
mov r9,r9,lsr #9
b clearFlashDone



/*game over animation*/

over:
tst r5,#0x010000
beq clear3
mov r2,#1
lsl r2,#18
sub r2,r2,#1
b main

/*check stage clear2*/
clear3:
tst r5,#0x008000
beq stageDig
lsl r5,#1
add r5,r5,#1
mov r2,#0
b main

/*check stage digit*/
stageDig:
tst r5,#0x004000
beq clear2
mov r2,#0
lsl r5,#1
add r5,r5,#1
removeTens:
sub r4,r4,#10
cmp r4,#10
bge removeTens

cmp r4,#9
bge display9
cmp r4,#8
bge display8
cmp r4,#7
bge display7
cmp r4,#6
bge display6
cmp r4,#5
bge display5
cmp r4,#4
bge display4
cmp r4,#3
bge display3
cmp r4,#2
bge display2
cmp r4,#1
bge display1
b display0

/*check stage clear2*/
clear2:
tst r5,#0x002000
beq stageTens
lsl r5,#1
add r5,r5,#1
mov r2,#0
b main

/*check stage tens*/
stageTens:
tst r5,#0x001000
beq clear1
mov r12,#0
lsl r5,#1
add r5,r5,#1
removeHundreds:
sub r4,r4,#100
cmp r4,#100
bge removeHundreds

cmp r4,#90
bge display9
cmp r4,#80
bge display8
cmp r4,#70
bge display7
cmp r4,#60
bge display6
cmp r4,#50
bge display5
cmp r4,#40
bge display4
cmp r4,#30
bge display3
cmp r4,#20
bge display2
cmp r4,#10
bge display1
add r4,r4,#10
b display0

/*check stage clear1*/
clear1:
tst r5,#0x000800
beq stageHundreds
lsl r5,#1
add r5,r5,#1
mov r2,#0
b main

/*check stage Hundreds*/
stageHundreds:
tst r5,#0x000400
beq stage9
lsl r5,#1
add r5,r5,#1

cmp r4,#900
bge display9
cmp r4,#800
bge display8
cmp r4,#700
bge display7
cmp r4,#600
bge display6
cmp r4,#500
bge display5
cmp r4,#400
bge display4
cmp r4,#300
bge display3
cmp r4,#200
bge display2
cmp r4,#100
bge display1
add r4,r4,#100
b display0


/*check stage 9*/
stage9:
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
mov r7,#0
b main

/*movement*/
animate:
lsl r2,#3
and r8,r6,#7
orr r2,r2,r8, lsr #0
mov r6, r6, lsr #3
add r7,r7,#1
cmp r7,#7
blt main
mov r7,#0
lsl r5,#1
add r5,r5,#1
b main

display9:
mov r2,#0x00007B
lsl r2,#8
add r2,#0x0000C9
b main

display8:
mov r2,#0x00007B
lsl r2,#8
add r2,#0x0000EF
b main

display7:
mov r2,#0x000072
lsl r2,#8
add r2,#0x000049
b main

display6:
mov r2,#0x000079
lsl r2,#8
add r2,#0x0000EF
b main

display5:
mov r2,#0x000079
lsl r2,#8
add r2,#0x0000CF
b main

display4:
mov r2,#0x00000B
lsl r2,#8
add r2,#0x0000C9
b main

display3:
mov r2,#0x000073
lsl r2,#8
add r2,#0x0000CF
b main

display2:
mov r2,#0x000073
lsl r2,#8
add r2,#0x0000E7
b main

display1:
mov r2,#0x000002
lsl r2,#8
add r2,#0x000049
b main

display0:
mov r2,#0x00007B
lsl r2,#8
add r2,#0x00006F
b main


/*end of animation*/
