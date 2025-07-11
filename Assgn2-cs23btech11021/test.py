import subprocess

for f in ["valid", "invalid"]:
    for i in range(20, 101, 10):
        val = i * i
        for j in range(5):
            subprocess.run(
                ["./a.out", f"tests/inputs/exp1/{f}s/inp{val}.txt", f"{f}_exp1_{val}"]
            )
    for i in range(10, 51, 5):
        for j in range(5):
            subprocess.run(
                ["./a.out", f"tests/inputs/exp2/{f}s/inp{i}.txt", f"{f}_exp2_{i}"]
            )

    t = 1
    for i in range(6):
        t = t * 2
        for j in range(5):
            subprocess.run(
                [
                    "./a.out",
                    f"tests/inputs/exp3/{f}s/inp{int(t/2)}.txt",
                    f"{f}_exp3_{t/2}",
                ]
            )
