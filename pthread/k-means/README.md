# K-Means Clustering Sequential

This project implements the K-Means clustering algorithm in C.

## About K-Means

K-Means is an unsupervised learning algorithm that groups data points into k clusters
based on feature similarity. The algorithm works as follows:

1. Initialize k centroids randomly
2. Assign each data point to the nearest centroid
3. Update centroids by calculating the mean of all points assigned to each cluster
4. Repeat steps 2-3 until convergence or maximum iterations reached

## Datasets

This project was designed to run its experiments on some specific datasets.

### Iris

The Iris dataset contains 150 samples of iris flowers with 4 features:

- Sepal length
- Sepal width
- Petal length
- Petal width

The dataset is divided into 3 classes (species of iris flowers). Its size makes it ideal
for minimal clustering experiments.

## Requirements

- GCC compiler (>=12.2.0)
- Make (>=4.3)
- Python >=3.11

## Building the Project

You can build the project using the provided `run_experiments.sh` script:

```bash
./run_experiments.sh <clusters> <max_iterations>
```

To use debug mode it's necessary to use Python to visualize the experiments. Create a
python virtualenv and activate it, after that run `pip install -r requirements.txt`.
After this setup run `./exec <dataset> <clusters> <max_iterations> 1`.
