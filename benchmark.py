import matplotlib.pyplot as plt
import csv

timestamps = []
generations = []

with open("bin/output/benchmark.csv", mode = 'r') as file:
    reader = csv.DictReader(file, fieldnames=["timestamp", "gen"])
    for row in reader:
        timestamps.append(row['timestamp'])
        generations.append(row['gen'])

plt.xlabel("Time")
plt.ylabel("Generation")

plt.plot(timestamps, generations)
plt.savefig("bin/output/benchmark.png")
