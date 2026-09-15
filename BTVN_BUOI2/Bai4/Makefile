TARGET = firmware
CC = arm-none-eabi-gcc
MCU = -mcpu=cortex-m3 -mthumb

CFLAGS = $(MCU) -Wall -O2 -g -std=c11 \
  -DSTM32F103xB -DUSE_HAL_DRIVER \
  -IInc -IDrivers/STM32F1xx_HAL_Driver/Inc \
  -IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
  -IDrivers/CMSIS/Include

LDFLAGS = $(MCU) -TSTM32F103C8Tx_FLASH.ld -specs=nosys.specs -Wl,--gc-sections

SRCS = Src/main.c Src/stm32f1xx_it.c Src/system_stm32f1xx.c startup_stm32f103xb.s \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
  Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c

all: $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@
	arm-none-eabi-objcopy -O binary $(TARGET).elf $(TARGET).bin

flash: $(TARGET).elf
	st-flash write $(TARGET).bin 0x8000000

clean:
	rm -f $(TARGET).elf $(TARGET).bin
