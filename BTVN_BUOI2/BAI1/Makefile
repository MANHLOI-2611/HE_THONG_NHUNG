TARGET = firmware

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CPU = -mcpu=cortex-m3 -mthumb

CFLAGS = $(CPU) -O0 -g3 -Wall -ffreestanding
ASFLAGS = $(CPU) -g3
LDFLAGS = $(CPU) -T linker.ld -nostdlib -Wl,--gc-sections

INCLUDES = -ICore/Inc

C_SOURCES = \
Core/Src/code.c

ASM_SOURCES = \
Core/Src/startup_stm32f103xb.s

C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

all: $(TARGET).bin

$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

flash: $(TARGET).bin
	sudo st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).bin

.PHONY: all flash clean
