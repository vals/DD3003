import matplotlib.pyplot as plt

with open("gram.txt") as handle:
    data = handle.read()

data2, data3 = data.split("\n\n\n")

data2 = data2.split("\n\n")[1:]
for i, core in enumerate(data2):
    data2[i] = core.split("\n")
    for j, sizes in enumerate(data2[i]):
        data2[i][j] = sizes.split()

data3 = data3.split("\n\n")
for i, core in enumerate(data3):
    data3[i] = core.split("\n")
    for j, sizes in enumerate(data3[i]):
        data3[i][j] = sizes.split()

fig = plt.figure()
ax2 = fig.add_subplot("121")
ax3 = fig.add_subplot("122")

# for core in data2:
#     sizes = [int(s[2].split("x")[0]) for s in core[:17]]
#     times = [float(s[-1]) for s in core[:17]]
#     ax.plot(sizes, times)

# for core in data3:
#     sizes = [int(s[2].split("x")[0]) for s in core[:17]]
#     times = [float(s[-1]) for s in core[:17]]
#     ax.plot(sizes, times)

for i, t1 in enumerate((float(s[-1])) for s in data2[0]):
    speedups = []
    cores = []
    for core in data2[1:]:
        speedups.append(float(core[i][-1]) / t1)
        cores.append(int(core[i][1]))

    if i in [4, 7, 9, 12, 14, 16]:
        ax2.plot(range(1, len(cores) + 1), speedups, 'o-', label=data2[0][i][2])

for i, t1 in enumerate((float(s[-1])) for s in data3[0]):
    speedups = []
    cores = []
    for core in data2[1:]:
        speedups.append(float(core[i][-1]) / t1)
        cores.append(int(core[i][1]))

    if i in [4, 7, 9, 12, 14, 16]:
        ax3.plot(range(1, len(cores) + 1), speedups, 'o-', label=data3[0][i][2])

ax2.legend(loc=2, title="Problem size")

plt.suptitle("Speeup for n cores is T_n / T_1", fontsize=16)

plt.subplot("121")
plt.xlabel("Number of cores")
plt.ylabel("Speedup")
plt.title("gram2")
plt.xticks([0, 1, 2, 3, 4, 5, 6], ["", 2, 4, 8, 16, 32, ""])
plt.grid(True)

plt.subplot("122")
plt.xlabel("Number of cores")
plt.ylabel("Speedup")
plt.title("gram3")
plt.xticks([0, 1, 2, 3, 4, 5, 6], ["", 2, 4, 8, 16, 32, ""])
plt.grid(True)

# plt.show()
plt.savefig("speedup.pdf")
