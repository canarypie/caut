import matplotlib.pyplot as plt
import csv

timestamps = []
generations = []

with open("bin/output/benchmark.csv", mode = 'r') as file:
    reader = csv.DictReader(file, fieldnames=["gen", "timestamp"])
    for row in reader:
        generations.append(int(row['gen']))
        timestamps.append(float(row['timestamp']) / 1_000_000)

plt.xlabel("Generation")
plt.ylabel("Time in ms")

plt.plot(generations, timestamps)
plt.savefig("bin/output/benchmark.png", bbox_inches='tight')
