#include "uc_poll.h"
#include <errno.h>

struct poll_table_page {
    struct poll_table_page * next;
    struct poll_table_entry * entry;
    struct poll_table_entry entries[0];
};

//! polling functions
//#define ROUND_UP(x,y) (((x)+(y)-1)/(y))

#define POLL_TABLE_FULL(table) \
    ((unsigned long)((table)->entry+1) > PAGE_SIZE + (unsigned long)(table))

static void __pollwait(struct uc_file *filp, wait_queue_head_t *wait_address,
                poll_table *p);

void poll_initwait(struct poll_wqueues *pwq)
{
    init_poll_funcptr(&pwq->pt, __pollwait);
    pwq->error = 0;
    pwq->table = NULL;
    pwq->inline_index = 0;
}

static void free_poll_entry(struct poll_table_entry *entry)
{
    remove_wait_queue(entry->wait_address,&entry->wait);
//    fput(entry->filp);
}

void poll_freewait(struct poll_wqueues *pwq)
{
    struct poll_table_page * p = pwq->table;
    int i;
    for (i = 0; i < pwq->inline_index; i++)
         free_poll_entry(pwq->inline_entries + i);
    while (p) {
         struct poll_table_entry * entry;
         struct poll_table_page *old;

         entry = p->entry;
         do {
              entry--;
              free_poll_entry(entry);
         } while (entry > p->entries);
         old = p;
         p = p->next;
         free(old);
    }
}

static struct poll_table_entry *poll_get_entry(poll_table *_p)
{
    struct poll_wqueues *p = container_of(_p, struct poll_wqueues, pt);
    struct poll_table_page *table = p->table;

    if (p->inline_index < N_INLINE_POLL_ENTRIES)
         return p->inline_entries + p->inline_index++;

    if (!table ) {
         struct poll_table_page *new_table;

         new_table = (struct poll_table_page *) malloc(sizeof(struct poll_table_page));
         if (!new_table) {
              p->error = -ENOMEM;
//              __set_current_state(TASK_RUNNING);
              return NULL;
         }
         new_table->entry = new_table->entries;
         new_table->next = table;
         p->table = new_table;
         table = new_table;
    }

    return table->entry++;
}

/* Add a new entry */
static void __pollwait(struct uc_file *filp, wait_queue_head_t *wait_address,
                   poll_table *p)
{
    struct poll_table_entry *entry = poll_get_entry(p);
    if (!entry)
         return;
//    get_file(filp);
    entry->filp = filp;
    entry->wait_address = wait_address;
    init_waitqueue_entry(&entry->wait, NULL);
    add_wait_queue(wait_address,&entry->wait);
}
