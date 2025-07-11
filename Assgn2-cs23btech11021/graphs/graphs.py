import matplotlib.pyplot as plt
import pandas as pd

files = [
    "../tables/timesexp1.txt",
    "../tables/timesexp2.txt",
    "../tables/timesexp3.txt",
]
colors = ["r", "g", "b", "y"]
markers = ["^", "x", "o", "v"]
labels_x = ["size of input", "taskInc", "no of threads"]

for idx, file in enumerate(files):
    df = pd.read_csv(file, sep=" ", header=None)
    df.columns = ["X", "sequential", "TAS", "CAS", "Bounded CAS"]

    plt.figure(figsize=(8, 6))

    plt.plot(
        df["X"],
        df["sequential"],
        marker=markers[0],
        linestyle="-",
        color=colors[0],
        label="sequential",
    )
    plt.plot(
        df["X"],
        df["TAS"],
        marker=markers[1],
        linestyle="-",
        color=colors[1],
        label="TAS",
    )
    plt.plot(
        df["X"],
        df["CAS"],
        marker=markers[2],
        linestyle="-",
        color=colors[2],
        label="CAS",
    )
    plt.plot(
        df["X"],
        df["Bounded CAS"],
        marker=markers[3],
        linestyle="-",
        color=colors[3],
        label="Bounded CAS",
    )

    plt.title(f"Graph for experiment {idx+1}")
    plt.xlabel(labels_x[idx])
    plt.ylabel("time")
    plt.legend()
    plt.grid(True)

    image_name = f"graph{idx+1}.png"
    plt.savefig(image_name, dpi=300, bbox_inches="tight")
    plt.close()

    print(f"{image_name} created")
