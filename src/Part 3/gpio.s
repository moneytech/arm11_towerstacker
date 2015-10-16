ldr r0,=0x20200000
mov r1,#1
lsl r1,#9
str r1,[r0,#8]
begin:
mov r1,#1
lsl r1,#23
str r1,[r0,#28]
mov r2,#0x1E0000
wait:
sub r2,r2,#1
cmp r2,#0
bne wait
mov r1,#1
lsl r1,#23
str r1,[r0,#40]
mov r2,#0x1E0000
wait2:
sub r2,r2,#1
cmp r2,#0
bne wait2
b begin
