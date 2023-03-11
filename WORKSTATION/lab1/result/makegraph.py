import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

groupno = 4
datasetno = 6
data = []
titleFont = {'family': 'FreeSerif',
             'size': 18,
             'c': 'black'}
normalFont = {'family': 'P052',
                'size': 13,
                'c': 'black'}

for i in range (groupno):
    base = int(input())
    d = [1.0]
    for j in range (1, datasetno):
        x = int(input())
        d.append(base/x)
    data.append(d)
fig, ax = plt.subplots(figsize=(8, 5), dpi=100)
labels = ["mnk", "mkn", "kmn", "nmk", "nkm", "knm"]
width = 0.2
for i in range (groupno):
    no = i + 1
    ax.bar(np.arange(len(data[i])) + width * i, data[i], width=width, tick_label=labels, label=f"{no}.txt")
plt.xticks([x+width/2*(groupno-1) for x in range(datasetno)], labels)
plt.xlabel('Different implementations of compute kernel', normalFont)
plt.ylabel('Speedup', normalFont)
plt.title('Task 1', titleFont)
ax.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))

# Draw lines
for i in range (1,8):
    plt.axhline(i, linestyle='--', linewidth=0.5, c='grey')

ax.legend()
plt.savefig('/home/pi/WORKSTATION/lab1/result/task1.png')
