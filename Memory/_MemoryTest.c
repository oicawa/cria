#include "Memory.h"

static void
dump_buffer(unsigned char *buf, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%02x ", buf[i]);
        if (i % 16 == 15)
        {
            printf("\n");
        }
    }
    printf("\n");
}

static void
fill_buffer(unsigned char *buf, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        buf[i] = i;
    }
}

int
main(void)
{
    unsigned char *p1;
    unsigned char *p2;
    unsigned char *p3;
    char *p4;

    p1 = Memory_malloc(10);
    dump_buffer(p1, 10);
    fill_buffer(p1, 10);
    dump_buffer(p1, 10);

    Memory_dumpBlocks(stdout);

    p2 = Memory_malloc(10);
    fill_buffer(p2, 10);
    p2 = Memory_realloc(p2, 15);
    dump_buffer(p2, 15);
    Memory_dumpBlocks(stdout);

    p2 = Memory_realloc(p2, 8);
    dump_buffer(p2, 8);
    Memory_dumpBlocks(stdout);

    p3 = NULL;
    p3 = Memory_realloc(p3, 10);
    dump_buffer(p3, 10);
    fill_buffer(p3, 10);
    dump_buffer(p3, 10);
    Memory_dumpBlocks(stdout);

    Memory_free(p2);
    dump_buffer(p2, 8);
    //Memory_free(p1);
    dump_buffer(p1, 10);
    Memory_free(p3);
    dump_buffer(p3, 10);
    fprintf(stderr, "pre final dump\n");
    Memory_dumpBlocks(stdout);
    
    p4 = Memory_string("uncocco!");
    fprintf(stdout, "p4 = %s\n", p4);
    Memory_dumpBlocks(stdout);
    
    
    fprintf(stderr, "Memory disposed!\n");
    Memory_dispose();
    fprintf(stderr, "final dump\n");
    Memory_dumpBlocks(stdout);

    return 0;
}

