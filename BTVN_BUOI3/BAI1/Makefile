TARGET = firmware

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 \
         -mthumb \
         -O0 \
         -Wall \
         -ffreestanding \
         -I Core/Inc

LDFLAGS = -mcpu=cortex-m3 \
          -mthumb \
          -nostdlib \
          -T linker.ld

SRC = Core/Src/main.c \
      Core/Src/i2c.c \
      Core/Src/ds1307.c \
      startup.c

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	sudo st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(TARGET).elf $(TARGET).bin
