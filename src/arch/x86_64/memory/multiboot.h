#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

struct MultibootTag {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) typedef MultibootTag;
 
struct MultibootInfo {
    uint32_t total_size;
    uint32_t reserved;
    uint32_t type;
    struct MultibootTag tags[];
} __attribute__((packed)) typedef MultibootInfo;

struct MultibootMemMapEntry
{
    uint64_t addr;
    uint64_t len;
    #define MULTIBOOT_MEMORY_AVAILABLE 1
    #define MULTIBOOT_MEMORY_RESERVED 2
    #define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
    #define MULTIBOOT_MEMORY_NVS 4
    uint32_t type;
    uint32_t zero;
} __attribute__((packed)) typedef MultibootMemMapEntry;

struct MultibootMemMapTag
{
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    MultibootMemMapEntry entries[0];
}__attribute__((packed)) typedef MultibootMemMapTag;

struct MultibootElfTag
{
    uint32_t type;
    uint32_t size;
    uint32_t num;
    uint32_t entsize;
    uint32_t shndx;
    char sections[0];
}__attribute__((packed)) typedef MultibootElfTag;

struct ElfSectionHeader {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
}__attribute__((packed)) typedef ElfSectionHeader;

void parse_memmap_tag(MultibootTag * tag);
void parse_elf_tag(MultibootTag * tag);
void parse_multiboot_tags(void * multiboot_addr);

#define MAX_MMAP_REGIONS 16

uint8_t num_mmap_regions;
void * mmap_regions[MAX_MMAP_REGIONS][2]; // mmap_regions[i] --> [mem_start, mem_end]
uint64_t * PHYS_MEMORY_UPBOUND; // set based on mmap edge of mmap regions
// addr range of the memory used to for our kernel code, provdied by ELF tag
void * ELF_addr_lowbound;  
void * ELF_addr_upbound;

#endif