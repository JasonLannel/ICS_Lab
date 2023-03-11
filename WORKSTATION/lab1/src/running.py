import sys
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

# WARNING
# DO NOT MODIFY THE FOLLOWING PART
# UNLESS YOU ARE COMPLETELY UNDERSTAND THE FUNCTION OF OPTIONS

# Options
# General
taskno = 2

# Graph
groupno = 8
kernelno = 5
labels = ["mnk", "t_mnk", "mnkkmn_b32", "mnk_lu2", "grade2"]
xtitle = 'Different implementations of compute kernel'
ytitle = 'Speedup'

# Following Part should not be modified
Test_Flag = True
Make_Graph_Flag = True
path = sys.path[0]
if __name__ == "__main__":
    if sys.argv.count('--taskno'):
        taskno = int(sys.argv[sys.argv.index('--taskno')+1])
    if sys.argv.count('--groupno'):
        groupno = int(sys.argv[sys.argv.index('--groupno')+1])
    if sys.argv.count('--kernelno'):
        kernelno = int(sys.argv[sys.argv.index('--kernelno')+1])
    if sys.argv.count('--path'):
        path = sys.argv[sys.argv.index('--path')+1]
        if(not os.path.isdir(path)):
            print("Illegal Path.")
            exit
    if sys.argv.count('--notest'):
        Test_Flag = False
    if sys.argv.count('--nograph'):
        Make_Graph_Flag = False
    if sys.argv.count('--label'):
        x = sys.argv.index('--label')
        labels = []
        for i in range(x+1, x+kernelno+1):
            labels.append(sys.argv[i])

if path[-1] != '/':
    path = path + '/'
os.chdir(path)
if not os.path.exists(f"{path}result"):
    os.system("mkdir result")

# Running Test
def RunTest():
    if os.path.exists(f"{path}result/tasks{taskno}_result"):
        os.system(f"rm -f ./result/tasks{taskno}_result")
        print("Result File Clear.")

    print(f"\"tasks{taskno}\" Start")
    for i in range(1, groupno+1):
        print(f"Test Group {i}")
        os.system(f"make tasks{taskno} I={i} >> ./result/tasks{taskno}_result")
        
# Make Graph
def getNum(testdata):
    ret = -1
    while True:
        str = testdata.readline().replace('\r','').replace('\n','')
        if str:
            if str[0]=='T':
                continue
            if str[31]==':':
                ret = int(str[33:])
                break
        else:
            break
    return ret

def MakeGraph():
    # Fixed Option
    title = f'Task {taskno}' # FIXED
    titleFont = {'family': 'FreeSerif',
                'size': 18,
                'c': 'black'}
    normalFont = {'family': 'P052',
                    'size': 13,
                    'c': 'black'}
    # Check Graph
    if os.path.exists(f"{path}result/task{taskno}.png"):
        os.system(f"rm -f task{taskno}.png")
        print("Graph Clear.")
    testdata = open(f"./result/tasks{taskno}_result",'r')
    data = []
    for i in range (groupno):
        base = getNum(testdata)
        d = [1.0]
        for j in range (1, kernelno):
            x = getNum(testdata)
            d.append(base/x)
        data.append(d)
    fig, ax = plt.subplots(figsize=(8 * (kernelno / 6), 5), dpi=100)
    width = 0.1 * (groupno / 8)
    for i in range (groupno):
        no = i + 1
        ax.bar(np.arange(len(data[i])) + width * i, data[i], width=width, tick_label=labels, label=f"{no}.txt")
    plt.xticks([x+width/2*(groupno-1) for x in range(kernelno)], labels)
    plt.xlabel('Different implementations of compute kernel', normalFont)
    plt.ylabel('Speedup', normalFont)
    plt.title(title, titleFont)
    ax.yaxis.set_major_locator(mtick.MultipleLocator(1))
    ax.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # Draw lines
    plt.grid(True, ls='--', axis='y')

    ax.legend()
    plt.savefig(f'{path}result/task{taskno}.png')

# Main
if Test_Flag:
    RunTest()
if Make_Graph_Flag:
    MakeGraph()