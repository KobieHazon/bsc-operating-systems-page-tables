#include <stddef.h>

#include "os.h"

#define OFFSET 12
#define VALID_BIT 0b1
#define ADDRESS_MASK (~0xfffULL)
#define LEVEL_CNT 5
#define CREATE_ENTRY 1

unsigned int get_level(uint64_t vpn, int level) {
    return (vpn >> (level * 9))     & (0b111111111);
}

uint64_t* get_last_level(uint64_t pt, uint64_t vpn, int is_create) {
    uint64_t curr_node = pt << OFFSET, *curr_virt;
    for (int i = 0; i < LEVEL_CNT; i++) {
        unsigned int index = get_level(vpn, i);
        curr_virt = phys_to_virt(curr_node);
        if (!(curr_virt[index] & VALID_BIT)) {
            if (is_create == CREATE_ENTRY) {
                curr_virt[index] = (alloc_page_frame() << OFFSET) | VALID_BIT;
                curr_node = curr_virt[index] & ADDRESS_MASK;
            }
            else
                return NULL;
        }
        else
            curr_node = curr_virt[index] & ADDRESS_MASK;
    }
    return curr_virt;
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    uint64_t *last_level = get_last_level(pt, vpn, !CREATE_ENTRY);
    if (ppn == NO_MAPPING && last_level != NULL)
        last_level[get_level(vpn, LEVEL_CNT - 1)] &= ~0b1;
    else if (last_level != NULL && ppn != NO_MAPPING)
        last_level[get_level(vpn, LEVEL_CNT - 1)] = (ppn << OFFSET) | VALID_BIT;
    else if (last_level == NULL && ppn != NO_MAPPING) {
        last_level = get_last_level(pt, vpn, CREATE_ENTRY);
        last_level[get_level(vpn, LEVEL_CNT - 1)] = (ppn << OFFSET) | VALID_BIT;
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    uint64_t* last_level;
    if ((last_level = get_last_level(pt, vpn, !CREATE_ENTRY)) == NULL)
        return NO_MAPPING;
    return last_level[get_level(vpn, LEVEL_CNT - 1)] >> OFFSET;
}
