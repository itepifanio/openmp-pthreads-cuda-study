# visualize experiments

import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
import glob

def load_iteration_files():
    # Get all iteration files
    files = sorted(glob.glob('experiments/iteration_*.csv'))
    return files

def create_animation(features: tuple[int, int], output_gif: str):
    """
    Creates an animation of the k-means clustering process.

    Parameters:
    - features: Tuple of column indices to use for 2D visualization
    - output_gif: Output filename for the GIF
    """
    files = load_iteration_files()
    if not files:
        return

    # Setup the figure
    fig, ax = plt.subplots(figsize=(10, 6))

    def update(frame):
        file = files[frame]
        iteration = int(file.split('_')[1].split('.')[0])

        df = pd.read_csv(file)

        centroids_mask = df.iloc[:, 0].astype(str).str.startswith('c')
        data_points = df[~centroids_mask]
        centroids = df[centroids_mask]

        ax.clear()

        x_col = df.columns[features[0] + 1]  # +1 to skip the ID column
        y_col = df.columns[features[1] + 1]

        clusters = data_points['cluster'].unique()
        for cluster in clusters:
            cluster_points = data_points[data_points['cluster'] == cluster]
            ax.scatter(
                cluster_points[x_col],
                cluster_points[y_col],
                alpha=0.6,
                label=f'Cluster {cluster}'
            )

        ax.scatter(
            centroids[x_col],
            centroids[y_col],
            color='red',
            marker='X',
            s=100,
            label='Centroids'
        )

        ax.set_title(f'K-means Clustering - Iteration {iteration}')
        ax.set_xlabel(x_col)
        ax.set_ylabel(y_col)
        ax.legend()

        return ax,

    anim = FuncAnimation(
        fig,
        update,
        frames=len(files),
        interval=500,
        blit=False
    )

    anim.save(output_gif, writer='pillow', fps=2)
    print(f"Animation saved as {output_gif}")

if __name__ == "__main__":
    create_animation(features=(0, 1), output_gif='kmeans_animation_features_0_1.gif')
    create_animation(features=(0, 2), output_gif='kmeans_animation_features_0_2.gif')
    create_animation(features=(1, 2), output_gif='kmeans_animation_features_1_2.gif')
