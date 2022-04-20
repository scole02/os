CC = ~/opt/cross/bin/$(arch)-elf-gcc
CFLAGS = -c -g -ffreestanding -fgnu89-inline -mno-red-zone
CC_warning_flags = -Wno-builtin-declaration-mismatch 
arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso

#hardware
serial_src := src/arch/$(arch)/serial.c
serial_obj := build/arch/$(arch)/serial.o
interrupts_src := src/arch/$(arch)/interrupts.c
interrupts_obj := build/arch/$(arch)/interrupts.o
pic_src := src/arch/$(arch)/pic.c
pic_obj := build/arch/$(arch)/pic.o
keyboard_src := src/arch/$(arch)/keyboard.c
keyboard_obj := build/arch/$(arch)/keyboard.o
ps2_src := src/arch/$(arch)/ps2.c
ps2_obj := build/arch/$(arch)/ps2.o
vga_src := src/arch/$(arch)/vga.c
vga_obj := build/arch/$(arch)/vga.o


# utils
libutils_src := src/arch/$(arch)/libutils.c
libutils_obj := build/arch/$(arch)/libutils.o
my_string_src := src/arch/$(arch)/my_string.c
my_string_obj := build/arch/$(arch)/my_string.o
printk_src := src/arch/$(arch)/printk.c
printk_obj := build/arch/$(arch)/printk.o

kmain_src := src/arch/$(arch)/kmain.c
kmain_obj := build/arch/$(arch)/kmain.o
linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%.o, $(assembly_source_files))

img_path := build/foo.img
BLOCKSIZE := 512
NUM_BLOCKS := 32768

.PHONY: all clean run iso img

all: $(kernel)

clean:
	@yes | rm -r build

run:
	@sudo qemu-system-x86_64 -s -drive format=raw,file=$(img_path) -serial stdio

# allows qemu to be ran via ssh, use ESC+2 and q+ENTER to quit
run-curses: 
	@sudo qemu-system-x86_64 -s -curses -drive format=raw,file=$(img_path) -serial stdio

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(assembly_object_files) $(linker_script)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(kmain_src) -o $(kmain_obj) 
	$(CC) $(CC_warning_flags) $(CFLAGS) $(my_string_src) -o $(my_string_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(vga_src) -o $(vga_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(printk_src) -o $(printk_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(libutils_src) -o $(libutils_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(ps2_src) -o $(ps2_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(keyboard_src) -o $(keyboard_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(pic_src) -o $(pic_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(interrupts_src) -o $(interrupts_obj)
	$(CC) $(CC_warning_flags) $(CFLAGS) $(serial_src) -o $(serial_obj)


	@ld -n -T $(linker_script) -o $(kernel) $(assembly_object_files) \
	$(kmain_obj) $(my_string_obj) $(vga_obj) $(libutils_obj) $(printk_obj) \
	$(ps2_obj) $(keyboard_obj) $(pic_obj) $(interrupts_obj) $(serial_obj)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@

img:	$(kernel) $(grub_cfg)
	@mkdir -p build/img.img/boot/grub/i386-pc
	@cp $(kernel) build/img.img/boot/kernel.bin
	@cp $(grub_cfg) build/img.img/boot/grub
	@sudo ./make_img.sh
