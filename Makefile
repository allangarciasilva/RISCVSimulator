MIF_FILE = $(BUILD_DIR)/memory_init.mif
BIN_FILE = $(BUILD_DIR)/main.bin
ELF_FILE = $(BUILD_DIR)/main.elf

ICMC_RAM_MIF = $(COMMON_DIR)/icmc_ram.mif
ICMC_RAM_CPP = $(BUILD_DIR)/icmc_ram.cpp
ICMC_RAM_OBJ = $(BUILD_DIR)/icmc_ram.cpp.o

SRC_DIR = src
BUILD_DIR = embedded-build
COMMON_DIR = $(SRC_DIR)/common
EMBEDDED_DIR = $(SRC_DIR)/embedded

CC = clang
CXX = clang++
ASM = clang
LINK = clang

RISCV_FLAGS = --target=riscv32 -march=rv32g -nostdlib
LINKER_FLAGS = $(RISCV_FLAGS) -Wl,--gc-sections

ASM_FLAGS = $(RISCV_FLAGS) -Os -g
C_FLAGS = $(ASM_FLAGS) -ffunction-sections -I$(SRC_DIR)
CC_FLAGS = $(C_FLAGS) -std=gnu99
CXX_FLAGS = $(C_FLAGS) -std=gnu++17

find_sources = $(wildcard $(1)/*.c $(1)/*.cpp $(1)/*.asm $(1)/**/*.c $(1)/**/*.cpp $(1)/**/*.asm)
COMMON_SOURCES = $(call find_sources,$(COMMON_DIR))
EMBEDDED_SOURCES = $(call find_sources,$(EMBEDDED_DIR))
SOURCES = $(COMMON_SOURCES) $(EMBEDDED_SOURCES) $(ICMC_RAM_OBJ)

LINKER_SCRIPT = $(EMBEDDED_DIR)/linking.ld
LINKER_FLAGS += -T $(LINKER_SCRIPT)

OBJ_DIR = $(BUILD_DIR)/objs
OBJECTS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SOURCES))

all : $(MIF_FILE)

$(MIF_FILE) : $(BIN_FILE)
	python scripts/binary_to_mif.py $(BIN_FILE) $(MIF_FILE)

$(BIN_FILE) : $(ELF_FILE)
	llvm-objcopy -S -O binary $(ELF_FILE) $(BIN_FILE)

debug: $(ELF_FILE)
	llvm-objdump -d $(ELF_FILE) > $(BUILD_DIR)/elf_dump
	llvm-objdump -s $(ELF_FILE) > $(BUILD_DIR)/bin_dump
	llvm-objdump -S $(ELF_FILE) > $(BUILD_DIR)/asm_dump.asm

$(ELF_FILE) : $(OBJECTS)
	$(LINK) $(LINKER_FLAGS) $(OBJECTS) -o $(ELF_FILE)

$(OBJ_DIR)/%.c.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) $^ -c -o $@

$(OBJ_DIR)/%.asm.o : $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASM_FLAGS) $^ -c -o $@

$(OBJ_DIR)/%.cpp.o : $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $^ -c -o $@

$(ICMC_RAM_OBJ) : $(ICMC_RAM_CPP)
	$(CXX) $(CXX_FLAGS) $^ -c -o $@

$(ICMC_RAM_CPP) : $(ICMC_RAM_MIF) scripts/mif_to_c.py
	mkdir -p $(dir $@)
	python scripts/mif_to_c.py $(ICMC_RAM_MIF) $(ICMC_RAM_CPP)