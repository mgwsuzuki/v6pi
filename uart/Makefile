
# Cソースファイル
SRCS = \
	vp_uart.c \
	vp_gpio.c \
	main.c

# アセンブラソースファイル
ASRCS = \
	vector.s

# ターゲットのベースファイル名
TARGET = target

# メモリマッピングファイル
MEMMAP = memmap

# YAGARTOパス
YAGARTO = C:/winapp/yagarto/yagarto-20121222

# インクルード
INCDIR = .

########################################
# 以下は必要に応じて変更

# アセンブラオプション
AOPS = --warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6

# Cオプション
COPS = -Wall -nostartfiles -ffreestanding

# コンパイルフラグ
CFLAGS = -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=softfp -O1 -ggdb -g

# YAGARTO実行ファイルのパスとプレフィックス
ARMGNU = $(YAGARTO)/bin/arm-none-eabi

# ライブラリ
LIB = -L $(YAGARTO)/lib/gcc/arm-none-eabi/4.7.2 -L $(YAGARTO)/arm-none-eabi/lib

########################################
# 以下は原則として変更しない

OBJS = $(SRCS:.c=.o)

AOBJS = $(ASRCS:.s=.o)

%.o: %.c
	$(ARMGNU)-gcc $(COPS) $(CFLAGS) -I $(INCDIR) -c $< -o $@

%.o: %.s
	$(ARMGNU)-as $(AOPS) $< -o $@

gcc : kernel.img $(TARGET).hex $(TARGET).bin $(TARGET).elf

all : gcc $(OBJS) $(AOBJS)

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.map

$(TARGET).elf : $(AOBJS) $(OBJS)
	$(ARMGNU)-ld $(AOBJS) $(OBJS) -Map=$(TARGET).map -T $(MEMMAP) -o $@ $(LIB) -lc -lm -lgcc
	$(ARMGNU)-objdump -D $(TARGET).elf > $(TARGET).list

$(TARGET).bin : $(TARGET).elf
	$(ARMGNU)-objcopy $< -O binary $@

$(TARGET).hex : $(TARGET).elf
	$(ARMGNU)-objcopy $< -O ihex $@

kernel.img : $(TARGET).bin
	cp $< $@
