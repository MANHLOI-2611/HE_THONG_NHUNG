
TARGET = firmware

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

PORT = FreeRTOS/portable/GCC/ARM_CM3
MEM = FreeRTOS/portable/MemMang

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g3 \
         -Wall -Wextra -ffreestanding \
         -ffunction-sections -fdata-sections \
         -fno-unwind-tables -fno-asynchronous-unwind-tables \
         -DSTM32F10X_MD \
         -I Core/Inc \
         -I FreeRTOS/include \
         -I $(PORT)

LDFLAGS = -T linker.ld -nostartfiles \
          -Wl,--gc-sections \
          --specs=nosys.specs

SRC = startup.c \
      Core/Src/main.c \
      FreeRTOS/tasks.c \
      FreeRTOS/queue.c \
      FreeRTOS/list.c \
      $(PORT)/port.c \
      $(MEM)/heap_4.c

OBJ = $(SRC:.c=.o)

all: $(TARGET).bin

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).bin

.PHONY: all clean flash
