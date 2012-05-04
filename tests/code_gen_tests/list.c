#include <stdio.h> 
#include <string.h>
#include "machine.h"
#include "inst.h"
#include "/Users/cram/Courses/Compilers/Devel/Cream/lib/stdio.c"
entry static_area_initialization() {
strcpy((char *) (&(static_area[0])), "\n");
strcpy((char *) (&(static_area[1])), "\n");
strcpy((char *) (&(static_area[2])), "\n");
strcpy((char *) (&(static_area[3])), "\n");
strcpy((char *) (&(static_area[4])), " ");
strcpy((char *) (&(static_area[5])), "");
strcpy((char *) (&(static_area[6])), "Length = ");
strcpy((char *) (&(static_area[9])), "Sum = ");
strcpy((char *) (&(static_area[11])), "Length = ");
strcpy((char *) (&(static_area[14])), "Sum = ");
region_table[0] = 0;
region_table[1] = 16;
region_table[2] = 16;
region_table[3] = 16;
region_table[4] = 16;
}

entry sugar_main() { 
new_env(0,0);
call(main_40);
ret();
}
entry scan_int_3() {
new_env(0, 0);
scan_int();
null();
ret();
}
entry scan_float_4() {
new_env(0, 0);
scan_float();
null();
ret();
}
entry In_5() {
new_env(1, 0);
ildc(0);
load();
ret();
}
entry print_7() {
new_env(1, 0);
print_int();
null();
ret();
}
entry print_9() {
new_env(1, 0);
print_float();
null();
ret();
}
entry print_11() {
new_env(1, 0);
print_boolean();
null();
ret();
}
entry print_13() {
new_env(1, 0);
print_string();
null();
ret();
}
entry println_15() {
new_env(1, 0);
ildc(0);
load();
call(print_7);
pop();
ildc(0);
call(print_13);
pop();
null();
ret();
}
entry println_17() {
new_env(1, 0);
ildc(0);
load();
call(print_9);
pop();
ildc(1);
call(print_13);
pop();
null();
ret();
}
entry println_19() {
new_env(1, 0);
ildc(0);
load();
call(print_11);
pop();
ildc(2);
call(print_13);
pop();
null();
ret();
}
entry println_21() {
new_env(1, 0);
ildc(0);
load();
call(print_13);
pop();
ildc(3);
call(print_13);
pop();
null();
ret();
}
entry Out_23() {
new_env(1, 0);
ildc(0);
load();
ret();
}
entry List_28() {
new_env(2, 0);
ildc(0);
load();
ildc(0);
ildc(1);
load();
putfield();
pop();
ildc(0);
load();
ildc(1);
null();
putfield();
pop();
ildc(0);
load();
ret();
}
entry get_element_29() {
new_env(1, 0);
ildc(0);
load();
ildc(0);
getfield();
ret();
null();
ret();
}
entry push_front_30() {
new_env(2, 1);
ildc(2);
newobj(2);
ildc(1);
load();
call(List_28);
store();
pop();
ildc(2);
load();
ildc(1);
ildc(0);
load();
putfield();
pop();
ildc(2);
load();
ret();
null();
ret();
}
entry pop_front_33() {
new_env(1, 0);
ildc(0);
load();
ildc(1);
getfield();
ret();
null();
ret();
}
entry length_34() {
new_env(1, 0);
ildc(0);
load();
null();
oequal();
jz(label0);
ildc(0);
ret();
jmp(label1);
label0:
ildc(1);
ildc(0);
load();
ildc(1);
getfield();
call(length_34);
iadd();
ret();
label1:
null();
ret();
}
entry sum_35() {
new_env(1, 0);
ildc(0);
load();
null();
oequal();
jz(label2);
ildc(0);
ret();
jmp(label3);
label2:
ildc(0);
load();
call(get_element_29);
ildc(0);
load();
ildc(1);
getfield();
call(sum_35);
iadd();
ret();
label3:
null();
ret();
}
entry print_36() {
new_env(1, 1);
ildc(1);
ildc(0);
load();
store();
pop();
label4:
ildc(1);
load();
null();
onotequal();
jz(label5);
ildc(1);
load();
call(get_element_29);
call(print_7);
pop();
ildc(4);
call(print_13);
pop();
ildc(1);
ildc(1);
load();
ildc(1);
getfield();
store();
pop();
jmp(label4);
label5:
ildc(5);
call(println_21);
pop();
null();
ret();
}
entry List_38() {
new_env(1, 0);
ildc(0);
load();
ret();
}
entry main_40() {
new_env(0, 3);
ildc(1);
newobj(2);
ildc(0);
call(List_28);
store();
pop();
ildc(0);
ildc(1);
store();
pop();
label6:
ildc(0);
load();
ildc(6);
ilt();
jz(label7);
ildc(1);
ildc(1);
load();
ildc(0);
load();
call(push_front_30);
store();
pop();
label8:
ildc(0);
dup();
load();
swap();
dup();
load();
ildc(1);
iadd();
store();
pop();
pop();
jmp(label6);
label7:
ildc(1);
load();
call(print_36);
pop();
ildc(6);
call(print_13);
pop();
ildc(1);
load();
call(length_34);
call(println_15);
pop();
ildc(9);
call(print_13);
pop();
ildc(1);
load();
call(sum_35);
call(println_15);
pop();
ildc(2);
newobj(2);
ildc(1);
load();
call(get_element_29);
call(List_28);
store();
pop();
ildc(1);
ildc(1);
load();
call(pop_front_33);
store();
pop();
label9:
ildc(1);
load();
null();
onotequal();
jz(label10);
ildc(2);
ildc(2);
load();
ildc(1);
load();
call(get_element_29);
call(push_front_30);
store();
pop();
ildc(1);
ildc(1);
load();
call(pop_front_33);
store();
pop();
jmp(label9);
label10:
ildc(2);
load();
call(print_36);
pop();
ildc(11);
call(print_13);
pop();
ildc(2);
load();
call(length_34);
call(println_15);
pop();
ildc(14);
call(print_13);
pop();
ildc(2);
load();
call(sum_35);
call(println_15);
pop();
null();
ret();
}
entry Test_44() {
new_env(1, 0);
ildc(0);
load();
ret();
}
