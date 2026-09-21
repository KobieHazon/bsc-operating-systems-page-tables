# BSc Operating Systems - Page Tables

- Course: BSc Computer Science.
- Available copy: 2019–2020.
- Supplied exercise material is identified separately below.
- My implementation is kept separately from supplied exercise files.
- Submitted ZIP wrappers and Apple resource forks were omitted.

## Contents

Five-level page-table simulation implementing mapping update and query operations over a supplied physical-memory simulator.

## Files

Exercise/framework material:

- `assignment/MISSING_HANDOUT.md`
- `assignment/os.h`
- `assignment/os.c`

Implementation material:

- `src/pt.c`

## Tech Stack

- C.
- POSIX APIs where applicable.
- `pthread` for the parallel-find assignment.
- Linux kernel-module APIs for the message-slot assignment.

## Validate

```bash
make check
```

## Notes

The exact original handout was not recovered; the supplied simulator files from the submission archive are preserved as the framework.
