#ifndef MEMORY_H
#define MEMORY_H

#define PAGE_SIZE 4096
#define PGFRM_LL_SIZE 2048 // the amount of pages we want to keep in our linked list

void init_pf_allocater();
void * MMU_pf_alloc(void); // returns address of page frame
void MMU_pf_free(void *pf); // adds page to free mem pool


#endif