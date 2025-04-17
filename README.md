# OpenMP, PThreads and Cuda study

Parallel programming with OpenMP, PThreads and Cuda.

## Building the Project

This project uses a Makefile to simplify the compilation and execution process.
Follow these steps to build and run the program.

- Ensure you have a C++ compiler installed (g++ recommended).
- The make tool should be available on your system.

### Building

Every algorithm has a Makefile available. Enter at the algorithm folder, then compile
the project by running: `make`

The `make` command will:

- Compile all `.cpp` files from the `src/` directory.
- Store the compiled object files in the `build/` directory.
- Generate the final executable inside the `bin/` directory.

### Running

After building the project, run: `make exec`

### Cleaning

To remove compiled files and binaries, run: `make clean`
