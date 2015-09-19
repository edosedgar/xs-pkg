# STM32F4 assembler project

clean:
	setterm -foreground red
	rm -f ./*.out
	rm -f ./*.o
	rm -f ./*.S~
	rm -f ./*.bin
	rm -f ./*.elf
	rm -f ./*.lst
	setterm -foreground default

bin: main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin
	setterm -foreground default

main.elf: main.o mem.ld
	arm-none-eabi-ld -T mem.ld -o main.elf main.o

main.o: main.S
	setterm -foreground green
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -o main.o main.S

flash: main.bin
	setterm -foreground blue
	st-flash write main.bin 0x08000000
	setterm -foreground default

erase:
	setterm -foreground red
	st-flash erase
	setterm -foreground default

