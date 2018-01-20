#avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega16 ../effect.c -c -o Linker/effect.o
#avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega16 ../main.c -c -o Linker/main.o
#avr-gcc -g -Os -Wall -mcall-prologues -mmcu=atmega16 Linker/effect.o Linker/main.o -o Obj/cube555.obj
#avr-objcopy  -R .eeprom -O ihex Obj/cube555.obj Exe/cube555.hex
