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
strcpy((char *) (&(static_area[4])), "Int field is ");
region_table[0] = 0;
region_table[1] = 8;
region_table[2] = 8;
region_table[3] = 8;
}

entry sugar_main() { 
new_env(0,0);
call(main_27);
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
entry A_25() {
new_env(1, 0);
ildc(0);
load();
ildc(0);
ildc(1);
putfield();
pop();
ildc(0);
load();
ret();
}
entry main_27() {
new_env(0, 1);
ildc(0);
newobj(1);
call(A_25);
store();
pop();
ildc(0);
load();
ildc(0);
dup2();
getfield();
swap1_2();
dup2();
getfield();
ildc(1);
iadd();
putfield();
pop();
pop();
ildc(4);
call(print_13);
pop();
ildc(0);
load();
ildc(0);
getfield();
call(println_15);
pop();
null();
ret();
}
