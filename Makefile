CC = ~/opt/cross/bin/$(arch)-elf-gcc
CC_warning_flags = -Wbuiltin-declaration-mismatch #not used
arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso


vga_src := src/arch/$(arch)/vga.c
vga_obj := build/arch/$(arch)/vga.o
string_src := src/arch/$(arch)/my_string.c
string_obj := build/arch/$(arch)/my_string.o
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
	@sudo qemu-system-x86_64 -hda $(img_path)

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(assembly_object_files) $(linker_script)
	$(CC) -g -c $(kmain_src) -o $(kmain_obj) 
	$(CC) -g -c $(string_src) -o $(string_obj)
	$(CC) -g -c $(vga_src) -o $(vga_obj)
	@ld -n -T $(linker_script) -o $(kernel) $(assembly_object_files) $(kmain_obj) $(string_obj) $(vga_obj)
# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@

img:	$(kernel) $(grub_cfg)
	@mkdir -p build/img.img/boot/grub/i386-pc
	@cp $(kernel) build/img.img/boot/kernel.bin
	@cp $(grub_cfg) build/img.img/boot/grub
	@sudo ./make_img.sh
