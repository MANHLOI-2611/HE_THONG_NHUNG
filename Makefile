CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -Wall -DSTM32F10X_MD
LDFLAGS = -T stm32f103.ld -nostartfiles -Wl,--gc-sections

TARGET = main

all: $(TARGET).elf $(TARGET).bin $(TARGET).hex

$(TARGET).elf: main.o startup.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

startup.o: startup.s
	$(CC) $(CFLAGS) -c $< -o $@

flash: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
	-c "program $(TARGET).elf verify reset exit"

clean:
	rm -f *.o *.elf *.bin *.hex
