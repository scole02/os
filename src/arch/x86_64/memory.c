#include "memory.h"
#include "multiboot.h"
#include "printk.h"

static uint64_t * free_mem_pool_head = 0;
static uint64_t free_mem_pool_size = 0;

void _refill_pool() // called when linked list is emptied, adds PGFRM_LL_SIZE pages to pool
{
    
    if((uint64_t*)((void*)free_mem_pool_head + PGFRM_LL_SIZE) >= (uint64_t*)PHYS_MEMORY_UPBOUND)
    {
        printk("ERROR: PHYSICAL MEMORY POOL EXHAUSTED");
        return;
    }
    // find the next free PAGE_SIZE aligned address, the start of free memory
    for(void * cur_pg=(void*)free_mem_pool_head; 
        cur_pg < (void*)free_mem_pool_head + (PGFRM_LL_SIZE*PAGE_SIZE); 
        cur_pg += PAGE_SIZE)
    {
        *((uint64_t**)cur_pg) = cur_pg + PAGE_SIZE; // write the addr of next page frame to cur frame
        //printk("%x --> %x\n", cur_pg, *(uint64_t**)cur_pg);
    }
    free_mem_pool_size = PGFRM_LL_SIZE; 
}

// initalize linked list of page frames
void init_pf_allocater()
{
    if(!ELF_addr_upbound || !ELF_addr_lowbound) 
    {
        printk("ERROR: ELF MEMORY BOUNDS NOT SET\n");
        return;
    }
    // set linked list head to first PAGE_SIZE aligned address outside 
    // of ELF kernel memory, the start of free memory
    free_mem_pool_head = (uint64_t*)(PAGE_SIZE - ((uint64_t)ELF_addr_upbound % PAGE_SIZE) + (uint64_t)ELF_addr_upbound);
    _refill_pool();
}

void * MMU_pf_alloc(void) // returns address of free page frame
{
    if (!free_mem_pool_size) _refill_pool();
    void * free_pg = (void*)free_mem_pool_head;
    free_mem_pool_head = *((uint64_t**)free_pg); // set head to value in former head
    return free_pg;

}

void MMU_pf_free(void *pf) // adds page to free mem pool
{
    *((uint64_t**)pf) = free_mem_pool_head; // newly freed page points to old head
    free_mem_pool_head = pf;
    // has to be something im missing?
}