#include "os.h"
#include "SPBufferset.h"

#define OFFSET 12
#define VALID_BIT 0b1
#define LEVEL_CNT 5
#define CREATE_ENTRY 1

unsigned int get_level(uint64_t vpn, int level) {
    return (vpn >> (level * 9))     & (0b111111111);
}

uint64_t* get_last_level(uint64_t pt, uint64_t vpn, int is_create) {
    uint64_t curr_node = pt, *curr_virt;
    for (int i = 0; i < LEVEL_CNT; i++) {
        curr_virt = phys_to_virt(curr_node);
        if (!(curr_virt[get_level(vpn, i)] & VALID_BIT)) {
            if (is_create == CREATE_ENTRY) {
                curr_virt[get_level(vpn, i)] = (alloc_page_frame() << OFFSET) | VALID_BIT;
                curr_node = curr_virt[get_level(vpn, i)];
            }
            else
                return NULL;
        }
        else
            curr_node = curr_virt[get_level(vpn, i)];
    }
    return !(curr_node & VALID_BIT) ? NULL : curr_virt;
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