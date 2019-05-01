#Executable Files
MiniShell = MiniShell

#PATH
INC_DIR   = ./include
SRC_DIR   = ./src
BUILD_DIR = ./build
BIN_DIR   = $(BUILD_DIR)/bin
OBJ_DIR   = $(BUILD_DIR)/obj



#Compiler
CC := gcc
LD := gcc
CFLAGS := -Wall -I $(INC_DIR)
LFLAGS := $(CFLAGS) -lreadline -lpthread

#MiniShell C Files
SHELL_C_FILES := $(shell find $(SRC_DIR)/process $(SRC_DIR)/shell $(SRC_DIR)/string -name "*.c")

#MiniShell Obj Files
SHELL_OBJ_FILES := $(patsubst %.c, %.o, $(subst $(SRC_DIR), $(OBJ_DIR), $(SHELL_C_FILES)))

#Build .c to .o
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@-$(CC) $(CFLAGS) -c -o $@ $<

#Build .o to bin 
$(BIN_DIR)/$(MiniShell) : $(SHELL_OBJ_FILES)
	@echo + LD $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LFLAGS)

#Fast shell command
.PHONY : clean run test
clean : 
	@rm -r $(BUILD_DIR)

run : 
	-make
	./build-mylib.sh
	$(BIN_DIR)/$(MiniShell)

test :
	gcc test.c -o a.out
	./a.out
