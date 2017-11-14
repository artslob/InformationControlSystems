# ---------------------------------------------------------------------------
# Имя проекта

NAME	= lab3

# Настройки компилятора и линкера

CC      = sdcc
CFLAGS  = -I./INCLUDE -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Настройки системы автоинкремента версии сборки

PROJECT  = $(cmd type PROJECT)
VERSION  = 1.0
BUILD    = $(cmd type BUILD)
TYPE     = $(cmd type TYPE)

TARBALL  = ${NAME}-1.0.tar

# Настройки M3P

M3P		 = m3p
COMPORT	 = com3
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600	

# Каталоги с исходными текстами

SRC_DIR = SRC

DEL = C:/MinGW/msys/1.0/bin/rm.exe -f --
# ---------------------------------------------------------------------------

all: lab3

clean:
	$(DEL) $(NAME).hex $(NAME).bin $(NAME).map $(NAME).mem $(NAME).lnk $(NAME).lk com?_log.txt pm3p_*.txt $(TARBALL).gz $(SRC_DIR)/*.asm $(SRC_DIR)/*.rel $(SRC_DIR)/*.rst $(SRC_DIR)/*.sym $(SRC_DIR)/*.lst
	#

load: all
	$(M3P) lfile load.m3p


dist:
	tar -cvf $(TARBALL) --exclude=*.tar .
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye



LIST_SRC = \
$(SRC_DIR)/lab3.c       \
$(SRC_DIR)/serial.c     \
$(SRC_DIR)/max.c        \
$(SRC_DIR)/led.c        \
$(SRC_DIR)/interrupt.c


LIST_OBJ = $(LIST_SRC:.c=.rel)

lab3 : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o lab3.hex $(LFLAGS)
	$(M3P) hb166 lab3.hex lab3.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@  

