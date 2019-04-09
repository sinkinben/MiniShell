#Executable Files
MiniShell = MiniShell

#PATH
INC_DIR   = ./include
SRC_DIR   = ./src
BUILD_DIR = ./build
BIN_DIR   = $(BUILD_DIR)/bin
OBJ_DIR   = $(BUILD_DIR)/obj
READLINE_DIR =  /lib/x86_64-linux-gnu/libreadline.so.6


#Compiler
CC := gcc
LD := gcc
CFLAGS := -Wall -I $(INC_DIR)
LFLAGS := $(CFLAGS) -lreadline -lpthread

#All C Files
C_FILES := $(wildcard $(SRC_DIR)/*/*.c)

#All Obj Files
OBJ_FILES := $(patsubst %.c, %.o, $(subst $(SRC_DIR), $(OBJ_DIR), $(C_FILES)))


#Build .c to .o
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@-$(CC) $(CFLAGS) -c -o $@ $<
	
#Build .o to bin exec files
$(BIN_DIR)/$(MiniShell) : $(OBJ_FILES)
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LFLAGS)

#Fast shell command
.PHONY : clean run test
clean : 
	@rm -r $(BUILD_DIR)

run : 
	-make
	./$(BIN_DIR)/$(MiniShell)

test :
	gcc test.c -o a.out
	./a.out
