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
region_table[0] = 0;
region_table[1] = 4;
region_table[2] = 4;
region_table[3] = 4;
}

entry sugar_main() { 
new_env(0,0);
call(main_25);
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
entry main_25() {
new_env(0, 2);
ildc(0);
ildc(3);
store();
pop();
ildc(1);
fldc(3.1415);
store();
pop();
ildc(0);
load();
call(println_15);
pop();
ildc(1);
load();
call(println_17);
pop();
ildc(1);
ildc(0);
load();
int2float();
ildc(1);
load();
fadd();
store();
pop();
ildc(1);
load();
call(println_17);
pop();
null();
ret();
}
entry Test_28() {
new_env(1, 0);
ildc(0);
load();
ret();
}
