
# C�\�[�X�t�@�C��
SRCS = \
	vp_string.c \
	vp_uart.c \
	vp_gpio.c \
	main.c

# �A�Z���u���\�[�X�t�@�C��
ASRCS = \
	vector.s

# �^�[�Q�b�g�̃x�[�X�t�@�C����
TARGET = target

# �������}�b�s���O�t�@�C��
MEMMAP = memmap

# YAGARTO�p�X
YAGARTO = C:/winapp/yagarto/yagarto-20121222

# �C���N���[�h
INCDIR = .

########################################
# �ȉ��͕K�v�ɉ����ĕύX

# �A�Z���u���I�v�V����
AOPS = --warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6

# C�I�v�V����
COPS = -Wall -nostartfiles -ffreestanding

# �R���p�C���t���O
CFLAGS = -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=softfp -O1 -ggdb -g

# YAGARTO���s�t�@�C���̃p�X�ƃv���t�B�b�N�X
ARMGNU = $(YAGARTO)/bin/arm-none-eabi

# ���C�u����
LIB = -L $(YAGARTO)/lib/gcc/arm-none-eabi/4.7.2 -L $(YAGARTO)/arm-none-eabi/lib

########################################
# �ȉ��͌����Ƃ��ĕύX���Ȃ�

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