import subprocess

for f in ["valid", "invalid"]:
    for i in range(2, 26):
        val = i * i
        for j in range(25):
            subprocess.run(
                ["./main", f"tests/inputs/{f}s/inp{val}x{val}.txt", f"{f}_{val}"]
            )
    for z in range(25):
        lno = 1
        for i in range(6):
            lno = lno * 2
            subprocess.run(
                ["./main", f"tests/inputs/36x36_{f}/inp_t{lno}.txt", f"t{f}_{lno}"]
            )
