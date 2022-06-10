#ifndef VIRT_MEMORY_H
#define VIRT_MEMORY_H
#include <stdint.h>

typedef struct {
    uint64_t offset : 12; // page index
    uint64_t pt_index : 9; // page table
    uint64_t pd_index : 9; // page directory
    uint64_t pdpt_index : 9; // page directory page table
    uint64_t pml4_index : 9; // page map level 4
    uint64_t : 16;
} __attribute__ ((packed)) VirtAddr;

typedef struct {
    uint64_t : 3;
    uint64_t page_level_write_thru : 1;
    uint64_t page_level_cache_disable : 1;
    uint64_t : 7; 
    uint64_t pml4_base_addr : 40;
    uint64_t reserved_mbz : 12;
} __attribute__ ((packed)) CR3Register;

// typedef struct {
//     uint64_t present : 1;           // Must be 1, region invalid if 0.
//     uint64_t rw: 1;                 // If 0, writes not allowed.
//     uint64_t user_supervisor : 1;   // If 0, user-mode accesses not allowed.
//     uint64_t page_write_through: 1; // Determines the memory type used to access PDPT.
//     uint64_t page_cache : 1;        // Determines the memory type used to access PDPT.
//     uint64_t accessed : 1;          // If 0, this entry has not been used for translation.
//     uint64_t ignored1 : 1;
//     uint64_t page_size : 1;         // Must be 0 for PML4E.
//     uint64_t ignored2 : 4;
//     uint64_t p_base_addr : 36;              
//     uint64_t : 4;
//     uint64_t ignored3 : 11;
//     uint64_t nx : 1;                // If 1, instruction fetches not allowed.
// } __attribute__ ((packed)) GenericTableEntry; // used for pml4, pdpt, pd

typedef struct {
    uint64_t present : 1;
    uint64_t rw : 1;
    uint64_t user_supervisor : 1;
    uint64_t WriteThrough : 1;
    uint64_t CacheDisabled : 1;
    uint64_t Accessed : 1;
    uint64_t ignore0 : 1; 
    uint64_t LargerPages : 1;
    uint64_t ignore1 : 1;
    uint64_t available : 3;
    uint64_t p_base_addr : 52;
} __attribute__ ((packed)) GenericTableEntry;

typedef struct {
    uint64_t present : 1;          // Must be 1, region invalid if 0.
    uint64_t rw : 1;               // If 0, writes not allowed.
    uint64_t user_supervisor : 1;  // If 0, user-mode accesses not allowed.
    uint64_t page_write: 1;        // Determines the memory type used to access the memory.
    uint64_t page_cache : 1;       // Determines the memory type used to access the memory.
    uint64_t accessed : 1;         // If 0, this entry has not been used for translation.
    uint64_t dirty: 1;             // If 0, the memory backing this page has not been written to.
    uint64_t page_access_type: 1;  // Determines the memory type used to access the memory.
    uint64_t global: 1;            // If 1 and the PGE bit of CR4 is set, translations are global.
    uint64_t ignored2 : 3;
    uint64_t phys_addr : 36;             // addr to phys page frame
    uint64_t reserved : 4;
    uint64_t ignored3 : 7;
    uint64_t protect_key: 4;       // If the PKE bit of CR4 is set, determines the protection key.
    uint64_t nx : 1;               // If 1, instruction fetches not allowed.
} __attribute__ ((packed)) PTEntry;

void init_virt_memory(void * pml4);


#endif