#include <stdint.h>
#include "../my_string.h"
#include "../printk.h"
#include "virt_memory.h"
#include "phys_memory.h"
#include "multiboot.h"

// given a virtual address, traverse page table and map physical address to it
void _map_memory(void* pml4, void* phys_addr, uint64_t v)
{
    VirtAddr *virt_addr = (VirtAddr*)&v;
    GenericTableEntry *pml4_entry, *pdpt_entry, *pd_entry;
    void * pf_addr = 0;
    PTEntry* pt_entry;

    pml4_entry = (GenericTableEntry*)(pml4 + virt_addr->pml4_index);
    if (!pml4_entry->present)
    {
        // p_base_addr is not valid, allocate page for this entry
        pf_addr = MMU_pf_alloc();
        //printk("Alloc'ed %x\n", pf_addr);
        memset(pf_addr, 0, PAGE_SIZE); // zero out new entry
        pml4_entry->p_base_addr = (uint64_t)pf_addr >> 12;
        pml4_entry->present = 1;
        pml4_entry->rw = 1;
        pml4_entry->user_supervisor = 1;
    }

    pdpt_entry = (GenericTableEntry*) ((uint64_t*)(uint64_t)(pml4_entry->p_base_addr) + virt_addr->pdpt_index);
    if (!pdpt_entry->present)
    {
        // p_base_addr is not valid, allocate page for this entry
        pf_addr = MMU_pf_alloc();
        //printk("Alloc'ed %x\n", pf_addr);
        memset(pf_addr, 0, PAGE_SIZE); // zero out new entry
        pdpt_entry->p_base_addr = (uint64_t)pf_addr >> 12;
        pdpt_entry->present = 1;
        pdpt_entry->rw = 1;
        pdpt_entry->user_supervisor = 1;
    }

    pd_entry = (GenericTableEntry*) ((uint64_t*)(uint64_t)(pdpt_entry->p_base_addr) + virt_addr->pd_index);
    if (!pd_entry->present)
    {
        // p_base_addr is not valid, allocate page for this entry
        pf_addr = MMU_pf_alloc();
        //printk("Alloc'ed %x\n", pf_addr);
        memset(pf_addr, 0, PAGE_SIZE); // zero out new entry
        pd_entry->p_base_addr = (uint64_t)pf_addr >> 12;
        pd_entry->present = 1;
        pd_entry->rw = 1;
        pd_entry->user_supervisor = 1;
    }

    pt_entry = (PTEntry*) ((uint64_t*)(uint64_t)(pd_entry->p_base_addr) + (virt_addr->pt_index));
    pt_entry->phys_addr = (uint64_t)phys_addr >> 12;
    pt_entry->present = 1;
    pt_entry->user_supervisor = 1;
    pt_entry->rw = 1; 
    //printk("done\n");
}

uint64_t _get_phys_page(void* pml4, uint64_t virt_addr)
{
    VirtAddr* v = (VirtAddr*)&virt_addr;
    PTEntry *pt_entry;
    GenericTableEntry *entry = (GenericTableEntry*) (pml4 + v->pml4_index);
    if(!entry->present) printk("ERROR %x, plm4 entry not present\n", virt_addr);
    
    entry = (GenericTableEntry*) ((uint64_t*)(uint64_t)(entry->p_base_addr) + v->pdpt_index);
    if(!entry->present) printk("ERROR %x, pdpt entry not present\n", virt_addr);

    entry = (GenericTableEntry*) ((uint64_t*)(uint64_t)(entry->p_base_addr) + v->pd_index);
    if(!entry->present) printk("ERROR %x, pd entry not present\n", virt_addr);

    pt_entry = (PTEntry*) ((uint64_t*)(uint64_t)(entry->p_base_addr) + v->pt_index);
    if(!entry->present) printk("ERROR %x, pt entry not present\n", virt_addr);

    return (uint64_t)(pt_entry->phys_addr);
} 

void init_virt_memory(void * pml4)
{
    //*pml4 = MMU_pf_alloc();

    int i = 0;
    //while(!i);
    
    // identity map physical memory to virtual memory
    for(uint64_t i=0; i < (uint64_t)0x1000; i += PAGE_SIZE)
        _map_memory(pml4, (uint64_t*)i, i);

    printk("Done mapping\n");

    uint64_t res = 0;
    for(uint64_t i =0; i < (uint64_t)0x1000; i += PAGE_SIZE)
    {
        res = _get_phys_page(pml4, i);
        if (res != i << 12) printk("%d != %d\n", res, i);
    }
    //asm ("mov %0, %%cr3" : : "r" (pml4)); // load pml4 into cr3 register



    // _map_memory(pml4, (void*)0x800000, 0x700000000);
    // printk("phys page: %d\n", _get_phys_page(pml4, 0x700000000));
    // uint64_t* test = (uint64_t*)0x700000000;
    // *test = 26;
    // printk("test: %d\n", * ((uint64_t*)0x800000));
    // printk("test: %d\n", *test);

}