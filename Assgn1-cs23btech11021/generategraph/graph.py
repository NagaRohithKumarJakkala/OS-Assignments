import matplotlib.pyplot as plt

# Data
for f in ["valid", "invalid"]:
    x = [i * i for i in range(3, 9)]
    y1 = []
    y2 = []
    y3 = []
    for p in range(3, 9):
        sum1 = 0.0
        sum2 = 0.0
        sum3 = 0.0
        with open(f"../tests/times/time{f}_{p*p}.txt", "r") as file:
            content = file.read()
            la = list(content.replace("\n", " ").split(" ")[:-1])
            i = 0
            for k in la:
                if i % 3 == 0:
                    sum1 += float(k)
                elif i % 3 == 1:
                    sum2 += float(k)
                else:
                    sum3 += float(k)
                i += 1
            y1.append(int(sum1 / 6))
            y2.append(int(sum2 / 6))
            y3.append(int(sum3 / 6))

    # Plot three lines
    plt.plot(x, y1, label="y = sequential", color="blue", marker=".")
    plt.plot(x, y2, label="y = chunk", color="red", marker=".")
    plt.plot(x, y3, label="y = mixed", color="green", marker=".")

    plt.grid(color="black", linestyle="--", linewidth=0.5)

    # Add title, labels, and legend
    plt.title(f"graph for {f} sudoku inputs(threads=8)")
    plt.xlabel("no of rows/cols in sudoku")
    plt.ylabel("time(in microseconds)")
    plt.legend()

    plt.savefig(f"graph_{f}.png")
    plt.figure()
for f in ["valid", "invalid"]:
    x = []
    y1 = []
    y2 = []
    y3 = []
    t = 2
    for p in range(4):
        t = t * 2
        x.append(t)
        sum1 = 0.0
        sum2 = 0.0
        sum3 = 0.0
        with open(f"../tests/times/timet{f}_{t}.txt", "r") as file:
            content = file.read()
            la = list(content.replace("\n", " ").split(" ")[:-1])
            i = 0
            for k in la:
                if i % 3 == 0:
                    sum1 += float(k)
                elif i % 3 == 1:
                    sum2 += float(k)
                else:
                    sum3 += float(k)
                i += 1
            y1.append(int(sum1 / 25))
            y2.append(int(sum2 / 25))
            y3.append(int(sum3 / 25))

    # Plot three lines
    plt.plot(x, y1, label="y = sequential", color="blue", marker=".")
    plt.plot(x, y2, label="y = chunk", color="red", marker=".")
    plt.plot(x, y3, label="y = mixed", color="green", marker=".")

    plt.grid(color="black", linestyle="--", linewidth=0.5)

    # Add title, labels, and legend
    plt.title(f"graph for 36x36 sudoku inputs and threads")
    plt.xlabel("no of threads")
    plt.ylabel("time(in microseconds)")
    plt.legend()

    plt.savefig(f"graph_t{f}.png")
    plt.figure()
