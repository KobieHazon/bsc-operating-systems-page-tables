#include <assert.h>
#include <stdio.h>
#include "os.h"

int course_smoke_main(int argc, char **argv);

int main(void)
{
    assert(course_smoke_main(0, NULL) == 0);
    uint64_t root = alloc_page_frame();
    uint64_t other = alloc_page_frame();
    uint64_t pages[] = {0, 1, 511, 512, 513, 1ULL << 18, 1ULL << 27,
                        1ULL << 36, (1ULL << 45) - 1};
    size_t count = sizeof(pages) / sizeof(pages[0]);
    for (size_t i = 0; i < count; ++i) {
        assert(page_table_query(root, pages[i]) == NO_MAPPING);
        page_table_update(root, pages[i], i);
    }
    for (size_t i = 0; i < count; ++i) {
        assert(page_table_query(root, pages[i]) == i);
        assert(page_table_query(other, pages[i]) == NO_MAPPING);
    }
    page_table_update(root, 512, 0x12345);
    assert(page_table_query(root, 512) == 0x12345);
    page_table_update(root, 512, NO_MAPPING);
    assert(page_table_query(root, 512) == NO_MAPPING);
    assert(page_table_query(root, 513) == 4);
    page_table_update(root, 99999, NO_MAPPING);
    assert(page_table_query(root, 99999) == NO_MAPPING);
    page_table_update(root, 512, 0);
    assert(page_table_query(root, 512) == 0);
    puts("PASS: supplied smoke test, boundary mappings, overwrite, removal, remapping and root isolation");
    return 0;
}
