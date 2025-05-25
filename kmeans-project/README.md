## K-Means Clustering in C (Sequential and Parallel)

This project implements the K-Means clustering algorithm in C, offering both a
sequential version and a parallel version using OpenMP. The goal is to study
performance improvements from parallelism and SIMD optimizations when handling large
datasets.

## Datasets Used

- Iris: Small dataset for testing.
- Rice: Medium dataset.
- HTRU2: Medium to large dataset.
- WESAD: Large dataset for stress testing parallel performance.

## Requirements
- GCC compiler (>=12.2.0)
- OpenMP support (>=5.1)
- Make (>=4.3)
- Python (>=3.11) for data visualization and post-processing (located at the `nbs` folder)

## Build Instructions

Use the provided bash script to compile and run experiments:

```bash
./run_experiments.sh <clusters> <max_iterations>
```

Example:

```bash
./run_experiments.sh 3 100
```
