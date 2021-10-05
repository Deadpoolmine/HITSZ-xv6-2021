// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct kmem {
  struct spinlock lock;
  struct run *freelist;
};

struct kmem kmems[NCPU];
char        lockname[NCPU][6];

int
getcoreid() 
{
  int coreid;
  push_off();
  coreid = cpuid();
  pop_off();
  return coreid;
}


void
kinit()
{
  int id;
  for (id = 0; id < NCPU; id++)
  {
    snprintf(lockname[id], 5, "kmem%d", id);
    initlock(&kmems[id].lock, lockname[id]);
    kmems[id].freelist = (void *)0;
  }
  freerange(end, (void*)PHYSTOP);
  
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;
  
  int coreid = getcoreid();

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
  
  acquire(&kmems[coreid].lock);
  r->next = kmems[coreid].freelist;     /* 头插法 */
  kmems[coreid].freelist = r;
  release(&kmems[coreid].lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  int coreid = getcoreid();
  int id;

  acquire(&kmems[coreid].lock);
  r = kmems[coreid].freelist;
  if(r)
    kmems[coreid].freelist = r->next;
  release(&kmems[coreid].lock);
  
  if (!r) {
    for (id = 0; id < NCPU; id++)
    {
      if (id != coreid) {
        acquire(&kmems[id].lock);
        r = kmems[id].freelist;
        if (r) {
          kmems[id].freelist = r->next;
          release(&kmems[id].lock);
          break;
        }
        release(&kmems[id].lock);
      }
    }
  }

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
