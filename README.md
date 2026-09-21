# BSc Operating Systems - Page Tables

- Course: Operating Systems.

My five-level page-table implementation, using the supplied physical-memory simulator. It maps virtual page numbers to physical page numbers, queries mappings, and removes them. This is a userspace simulation: it does not change your computer's page tables and needs no administrator privileges.

## Requirements

macOS or Linux, a C compiler, and Make. On macOS, run `xcode-select --install` if `cc` is unavailable. On Debian/Ubuntu, install `build-essential` if needed.

## Build and run

From the repository root:

```sh
make build
make run
```

Expected output, after any build commands:

```text
Before mapping: NO_MAPPING
Mapped virtual page 0xcafe to physical page 0xf00d
Query result: 0xf00d
After unmapping: NO_MAPPING
```

Try your own page numbers (decimal or `0x` hexadecimal):

```sh
./build/page_table_demo 512 42
# Or:
make run ARGS="512 42"
```

The first argument is a 45-bit virtual **page number**, not a byte address. The second is a physical page number of at most 52 bits. Each run creates a fresh in-memory table, demonstrates the mapping, then removes it. There is no persistent state or interactive prompt.

## Tests

```sh
make test
```

The tests execute the supplied simulator assertions and check boundary page numbers, multiple mappings, overwrite, removal, remapping, and isolation between two page-table roots. `make check` is an alias for these tests; `make clean` removes build output.

## Files

- `src/pt.c`: my page-table implementation.
- `src/demo.c`: command-line demonstration.
- `assignment/os.c` and `assignment/os.h`: supplied simulator and API, unchanged.
- `tests/test_page_tables.c`: behavioral tests.
