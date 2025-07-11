import matplotlib.pyplot as plt
import numpy as np


def plot_graph(filename, name):
    # Read values from the file
    with open(filename, "r") as file:
        values = file.readline().split()

    # Convert values to float
    y_values = [float(value) for value in values]

    # Given x-axis values
    x_values = [0.1, 0.5, 1, 5, 10]

    # Plot the values
    plt.figure(figsize=(8, 5))
    plt.plot(
        x_values, y_values, marker="o", linestyle="-", color="b", label="Data Points"
    )
    plt.xlabel("X-axis values")
    plt.ylabel("Y-axis values")
    plt.title(name)
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()

    outputfile = f"graph_{name}.png"
    plt.savefig(outputfile)
    plt.close()


# Example usage
plot_graph("../timings1.txt", "producer semaphore")
plot_graph("../timings2.txt", "consumer semaphore")
plot_graph("../timings3.txt", "producer locks")
plot_graph("../timings4.txt", "consumer locks")
