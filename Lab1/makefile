# ---------------------------------------------------------------------------
# Имя проекта

NAME	= lab1

# Настройки компилятора и линкера

CC      = sdcc
CFLAGS  = -I./INCLUDE -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Настройки системы автоинкремента версии сборки

PROJECT  = $(cmd type PROJECT)
VERSION  = $(cmd type VERSION)
BUILD    = $(cmd type BUILD)
TYPE     = $(cmd type TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Настройки M3P

M3P		 = m3p
COMPORT	 = com3
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600	

# Каталоги с исходными текстами

SRC_DIR = SRC

DEL = rm -f --
# ---------------------------------------------------------------------------

all: lab1

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
$(SRC_DIR)/lab1.c \
$(SRC_DIR)/max.c \
$(SRC_DIR)/led.c \


LIST_OBJ = $(LIST_SRC:.c=.rel)

lab1 : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o lab1.hex $(LFLAGS)
	$(M3P) hb166 lab1.hex lab1.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@  

