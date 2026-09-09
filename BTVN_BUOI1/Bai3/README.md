# Bài tập 03 – STM32F103: PA0-PA7 Input / PA8-PA15 Output (đảo bit)

Lập trình thuần bằng **arm-none-eabi-gcc** trên Linux (không dùng Keil C, không dùng CubeIDE).

## Cấu trúc project
```
stm32_project/
├── src/
│   ├── main.c       # Logic chính: đọc PA0-PA7, đảo bit, ghi PA8-PA15
│   └── startup.c    # Vector table + Reset_Handler (khởi động chip)
├── ld/
│   └── STM32F103C8.ld  # Linker script (Flash 64K / RAM 20K - đổi lại nếu dùng chip khác)
├── Makefile
└── README.md
```

## Nguyên lý
- `RCC->APB2ENR` bật clock cho GPIOA.
- `GPIOA->CRL = 0x44444444` → PA0-PA7 = Input floating.
- `GPIOA->CRH = 0x11111111` → PA8-PA15 = Output push-pull, 2MHz.
- Vòng lặp `while(1)`: đọc `GPIOA->IDR` (8 bit thấp), đảo bit (`~`), ghi vào 8 bit cao của `GPIOA->ODR` để điều khiển LED tại PA8-PA15.
- Nối nút nhấn (có điện trở kéo lên/xuống phù hợp) vào PA0-PA7, LED vào PA8-PA15 để thử nghiệm: nhấn nút (0) → LED sáng (1), thả nút (1) → LED tắt (0) — hoặc ngược lại tùy loại nút bạn nối (kéo lên/kéo xuống).

## 1. Cài công cụ trên Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install gcc-arm-none-eabi          # trình biên dịch ARM
sudo apt install stlink-tools               # cung cấp lệnh st-flash (nạp qua ST-Link)
# Hoặc dùng OpenOCD thay cho stlink-tools:
sudo apt install openocd
```

## 2. Biên dịch
```bash
cd stm32_project
make
```
Kết quả sinh ra trong thư mục `build/`:
- `baitap03.elf`
- `baitap03.bin`
- `baitap03.hex`

## 3. Nạp chương trình vào STM32F103 (qua mạch nạp ST-Link)
Cắm mạch nạp ST-Link V2 vào board Blue Pill / mạch STM32F103, sau đó:
```bash
make flash
```
(lệnh này chạy `st-flash write build/baitap03.bin 0x08000000`)

Nếu dùng OpenOCD thay vì st-flash:
```bash
make flash-openocd
```

## 4. Dọn dẹp
```bash
make clean
```

## Lưu ý
- Linker script mặc định cho **STM32F103C8** (Flash 64KB). Nếu board của bạn dùng chip khác (VD STM32F103RB - Flash 128KB, STM32F103C6 - Flash 32KB), sửa `LENGTH` trong `ld/STM32F103C8.ld` cho đúng dung lượng Flash/RAM thực tế.
- Đã build thử thành công bằng `gcc-arm-none-eabi` (không lỗi, không cảnh báo).
