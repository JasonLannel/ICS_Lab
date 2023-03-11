import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

# Modify
taskno = 2
groupno = 8
kernelno = 5
labels = ["mnk", "t_mnk", "mnkkmn_b32", "mnk_lu2", "grade2"]
title = f'Task {taskno}' # FIXED
xtitle = 'Different implementations of compute kernel'
ytitle = 'Speedup'


titleFont = {'family': 'FreeSerif',
             'size': 18,
             'c': 'black'}
normalFont = {'family': 'P052',
                'size': 13,
                'c': 'black'}

# WARNING
# DO NOT MODIFY THE FOLLOWING PART!!!
data = []
for i in range (groupno):
    base = int(input())
    d = [1.0]
    for j in range (1, kernelno):
        x = int(input())
        d.append(base/x)
    data.append(d)
fig, ax = plt.subplots(figsize=(8, 5), dpi=100)
width = 0.1
for i in range (groupno):
    no = i + 1
    ax.bar(np.arange(len(data[i])) + width * i, data[i], width=width, tick_label=labels, label=f"{no}.txt")
plt.xticks([x+width/2*(groupno-1) for x in range(datasetno)], labels)
plt.xlabel('Different implementations of compute kernel', normalFont)
plt.ylabel('Speedup', normalFont)
plt.title(title, titleFont)
ax.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))

# Draw lines
for i in range (1,8):
    plt.axhline(i, linestyle='--', linewidth=0.5, c='grey')

ax.legend()
plt.savefig(f'/home/jasonlan/workstation/ICS_Lab/WORKSTATION/lab1/result/task{taskno}.png')
