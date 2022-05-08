#include "multiboot.h"
#include "printk.h"

uint8_t num_mmap_regions = 0;
void * mmap_regions[MAX_MMAP_REGIONS][2];
uint64_t * PHYS_MEMORY_UPBOUND = 0;

void parse_memmap_tag(MultibootTag * tag)
{
    MultibootMemMapEntry *mmap;
    for (mmap = ((MultibootMemMapTag*) tag)->entries;   // start iterating through entries
    (uint8_t *) mmap < (uint8_t *) tag + tag->size; // use size to find end of entries    
    mmap = (MultibootMemMapEntry*)((unsigned long) mmap + ((MultibootMemMapTag*) tag)->entry_size)) // use entry size to iterate
    {
        // printk (" base_addr = 0x%x, length = 0x%x, type = 0x%x\n",
        // (unsigned) (mmap->addr),
        // //(unsigned) (mmap->addr & 0xffffffff),
        // (unsigned) (mmap->len),
        // (unsigned) mmap->type);
        if(mmap->type != 1) continue;
        mmap_regions[num_mmap_regions][0] = (void*)mmap->addr;
        mmap_regions[num_mmap_regions][1] = (void*)mmap->addr + mmap->len;
        num_mmap_regions += 1;
        
    }      

}
       
void parse_elf_tag(MultibootTag * tag)
{
    char * strtbl;
    ElfSectionHeader * first_elf_sect, * last_elf_sect, *elf_sect;
    MultibootElfTag * elf = (MultibootElfTag *)tag;
    
    //printk("ELF tag:\n  size:%d\n  num:%d\n  entsize:%d\n  shndx:%x\n", elf->size, elf->num, elf->entsize, elf->shndx);
    // for(int i=1; i<elf->num; i+=elf->num - 2)
    // {
    //     elf_sect = (ElfSectionHeader*)( (elf->sections)+(i * elf->entsize) );
    //     printk("Section:%lx  addr_range: %x --> %x\n", i, elf_sect->sh_addr, (void*)(elf_sect->sh_addr) + elf_sect->sh_size);
    // }

    first_elf_sect = (ElfSectionHeader*)( (elf->sections)+(elf->entsize) );
    ELF_addr_lowbound = (void*)(first_elf_sect->sh_addr); // get base addr of first ELF entry
    last_elf_sect = (ElfSectionHeader*)((elf->sections)+((elf->num - 1) * elf->entsize));
    ELF_addr_upbound =  (void*)(last_elf_sect->sh_addr) + last_elf_sect->sh_size; // get base addr + size of last ELF entry

}

void parse_multiboot_tags(void * multiboot_addr)
{
    MultibootTag *tag;
    //http://nongnu.askapache.com/grub/phcoder/multiboot.pdf
    for(tag = (MultibootTag*)(multiboot_addr+8); // start at address of first tag
        tag->type != 0;                // end when tag 0 is encountered
        tag = (MultibootTag*) ((uint8_t*)tag + ((tag->size + 7) & ~7)))
    {
        //printk ("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
        if (tag->type == 6) parse_memmap_tag(tag); // continue until mem map
        else if(tag->type == 9) parse_elf_tag(tag);
    }
    // for(int i=0; i<num_mmap_regions; i++)
    // printk("mmap[%d]: %x --> %x\n", i, mmap_regions[i][0], mmap_regions[i][1]);
    printk("AVAILABLE MEMORY: 0x%x  -->  0x%x\n", ELF_addr_upbound, mmap_regions[num_mmap_regions-1][1]);
    printk("ELF MEMORY BOUNDS INITALIZED\n");
    PHYS_MEMORY_UPBOUND = mmap_regions[num_mmap_regions-1][1];
}
