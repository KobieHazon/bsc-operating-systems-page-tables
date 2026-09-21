#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "os.h"

static uint64_t page_number(const char *text, uint64_t limit)
{
    char *end;
    errno = 0;
    int base = text[0] == '0' && (text[1] == 'x' || text[1] == 'X') ? 16 : 10;
    uint64_t value = strtoull(text, &end, base);
    if (text[0] == '-' || end == text || *end || errno || value > limit) {
        fprintf(stderr, "Invalid page number: %s\n", text);
        exit(EXIT_FAILURE);
    }
    return value;
}

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 3) {
        fprintf(stderr, "Usage: %s [VIRTUAL_PAGE PHYSICAL_PAGE]\n", argv[0]);
        return EXIT_FAILURE;
    }
    uint64_t vpn = argc == 3 ? page_number(argv[1], (1ULL << 45) - 1) : 0xcafe;
    uint64_t ppn = argc == 3 ? page_number(argv[2], (1ULL << 52) - 1) : 0xf00d;
    uint64_t root = alloc_page_frame();
    if (page_table_query(root, vpn) != NO_MAPPING)
        return EXIT_FAILURE;
    puts("Before mapping: NO_MAPPING");
    page_table_update(root, vpn, ppn);
    printf("Mapped virtual page 0x%" PRIx64 " to physical page 0x%" PRIx64 "\n", vpn, ppn);
    uint64_t result = page_table_query(root, vpn);
    printf("Query result: 0x%" PRIx64 "\n", result);
    if (result != ppn)
        return EXIT_FAILURE;
    page_table_update(root, vpn, NO_MAPPING);
    if (page_table_query(root, vpn) != NO_MAPPING)
        return EXIT_FAILURE;
    puts("After unmapping: NO_MAPPING");
    return EXIT_SUCCESS;
}
