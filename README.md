# Hillis-Steele-Prefix-Sum
### Parallel Prefix Sum using the Hillis and Steele Algorithm

**Author:** Riley McKinney  
**Course:** CS Class with Prof. Neeraj Mittal  
**Date:** March 25, 2025  

---

## Overview
`my-sum` is a **C++ parallel processing program** that implements the **Hillis and Steele concurrent prefix sum algorithm**. It reads an array of integers from an input file, computes the prefix sum using multiple concurrent processes, and writes the resulting array to an output file.  

The project demonstrates the use of **shared memory**, **barrier synchronization**, and **process-based parallelism** in C++ on UNIX systems.

---

## Features
- Implements the **Hillis and Steele** parallel prefix-sum algorithm.  
- Uses **shared memory** for process communication.  
- Employs **barrier synchronization** to coordinate process execution.  
- Accepts flexible input/output file paths and configurable process counts.  
- Supports arrays up to **10 million integers** (`MAX_N` configurable in `validate.h`).  

---

## Usage

### **Compilation**
In the project directory, run:
```bash
make
```

This will compile all source files and build the executable `my-sum`.  
Intermediate `.o` files are automatically removed after compilation.

To clean up all build artifacts manually:
```bash
make clean
```

---

### **Execution**
Run the program using:
```bash
./my-sum <n> <m> <input.txt> <output.txt>
```

Where:
- `<n>` — Number of integers in the input file  
- `<m>` — Number of processes to use for parallel computation  
- `<input.txt>` — Path to the input file containing integers  
- `<output.txt>` — Path to write the resulting prefix sum array  

Example:
```bash
./my-sum 8 4 input.txt output.txt
```

---

## File Structure

```
my-sum/
│
├── src/
│   ├── main.cpp          # Main driver: initializes shared memory, spawns processes, runs algorithm
│   ├── barrier.cpp       # Implements process synchronization using barriers
│   ├── io_handling.cpp   # Handles reading/writing arrays to files via shared memory
│   └── validate.cpp      # Validates command-line arguments and parameters
│
├── include/
│   ├── barrier.h
│   ├── io_handling.h
│   └── validate.h
│
├── input.txt             # Sample input array (modifiable)
├── output.txt            # Output file (auto-generated after run)
├── Makefile              # Build instructions
├── README.md             # Project documentation
└── .git/                 # Version control metadata
```

---

## Algorithm Summary: Hillis and Steele (Parallel Prefix Sum)
The Hillis and Steele algorithm computes prefix sums in **O(log n)** parallel time using **O(n log n)** work.

1. Each element `A[i]` initially holds the input value.  
2. For each iteration `k = 0, 1, 2, ...`:
   - Each process updates its assigned element:  
     `A[i] = A[i] + A[i - 2^k]` (if `i - 2^k >= 0`)
   - All processes synchronize using a **barrier** before proceeding.  
3. After `⌈log₂(n)⌉` rounds, all elements contain their prefix sums.

Example:

| Input | Round 1 | Round 2 | Result |
|:------|:--------|:--------|:--------|
| 1 2 3 4 | 1 3 5 7 | 1 3 6 10 | ✅ Prefix Sum = [1, 3, 6, 10] |

---

## 🧩 Example Input/Output

**`input.txt`**
```
1 2 3 4
```

**Command**
```bash
./my-sum 4 2 input.txt output.txt
```

**`output.txt`**
```
1 3 6 10
```

---

## Implementation Details

- **Parallelism:** Implemented using `fork()` to spawn child processes.  
- **Synchronization:** Each round is synchronized via a **custom barrier** implemented in `barrier.cpp`.  
- **Shared Memory:** All arrays are allocated in shared memory to allow concurrent read/write access.  
- **Error Handling:** The `validate.cpp` module ensures all inputs and files are valid before computation.  

---

## Notes
- Default maximum array size (`MAX_N`) is defined in `validate.h` as **10,000,000**.  
- Designed for UNIX/Linux environments (uses `<unistd.h>` and System V shared memory).  
- Ensure sufficient shared memory allocation if running large datasets.  

---

## Author
**Riley McKinney**  
University of Texas at Dallas  
*CS Student*
