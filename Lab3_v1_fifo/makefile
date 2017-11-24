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

DEL = rm -f --
# ---------------------------------------------------------------------------

all: lab3

clean:
	$(DEL) $(NAME).hex
	$(DEL) $(NAME).bin
	$(DEL) $(NAME).map
	$(DEL) $(NAME).mem
	$(DEL) $(NAME).lnk
	$(DEL) $(NAME).lk
	$(DEL) pm3p_*.txt
	$(DEL) com?_log.txt
	$(DEL) $(TARBALL).gz
	$(DEL) $(SRC_DIR)/*.asm
	$(DEL) $(SRC_DIR)/*.rel
	$(DEL) $(SRC_DIR)/*.rst
	$(DEL) $(SRC_DIR)/*.sym
	$(DEL) $(SRC_DIR)/*.lst 

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

