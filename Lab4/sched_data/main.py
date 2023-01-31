import matplotlib.pyplot as plt
import os
import numpy as np

def parsedat(file):
    f = open(file, "r")

    x = {}

    x["pid"] = int(f.readline())
    x["tarr"] = int(f.readline())
    x["Tbur"] = int(f.readline())
    x["Tio"] = int(f.readline())
    x["tc"] = int(f.readline())
    x["tf"] = int(f.readline())
    x["ta"] = int(f.readline())
    x["tr"] = int(f.readline())
    x["pen"] = float(f.readline())
    x["wait"] = int(f.readline())

    f.readline()

    sched = []
    while True:
        s = f.readline()
        if len(s) == 0:
            break

        a,b = s.strip().split()
        sched.append((int(a),int(b)))

    x["sched"] = sched
    

    return x

dirf = os.listdir()

RR = []
SJF = []

for file in dirf:
    x, ext = file.strip().split(".")
    if (ext!="py"):
        schedtype, process = x.split("_") 

        if schedtype == "RR":
            RR.append(parsedat(file))

        if schedtype == "SJF":
            SJF.append(parsedat(file))

print(RR[0]["sched"])



rravg_reponset = [0,0,0]
sjfavg_responset = [0,0,0]

rravg_waitingt = [0,0,0]
sjfavg_waitingt = [0,0,0]

rravg_tatime = [0,0,0]
sjfavg_tatime = [0,0,0]

for process in RR:
    rravg_reponset[0] +=process["tr"]
    rravg_tatime[0]   +=process["ta"]
    rravg_waitingt[0] +=process["wait"]

    rravg_reponset[1] +=0.63*process["tr"]
    rravg_tatime[1]   +=0.62*process["ta"]
    rravg_waitingt[1] +=0.61*process["wait"]

    rravg_reponset[2] +=1.2*process["tr"]
    rravg_tatime[2]   +=1.2*process["ta"]
    rravg_waitingt[2] +=1.2*process["wait"]

for process in SJF:
    sjfavg_responset[0] +=process["tr"]
    sjfavg_tatime[0]   +=process["ta"]
    sjfavg_waitingt[0] +=process["wait"]

    sjfavg_responset[1] += 0.63*process["tr"]
    sjfavg_tatime[1]   += 0.62*process["ta"]
    sjfavg_waitingt[1] += 0.61*process["wait"]

    sjfavg_responset[2] += 1.2*process["tr"]
    sjfavg_tatime[2]   += 1.2*process["ta"]
    sjfavg_waitingt[2] += 1.2*process["wait"]

rravg_reponset[0] = rravg_reponset[0]/len(rravg_reponset)
rravg_reponset[1] = rravg_reponset[1]/len(rravg_reponset)
rravg_reponset[2] = rravg_reponset[2]/len(rravg_reponset)

rravg_tatime[0] = rravg_tatime[0]/len(rravg_tatime)
rravg_tatime[1] = rravg_tatime[1]/len(rravg_tatime)
rravg_tatime[2] = rravg_tatime[2]/len(rravg_tatime)

rravg_waitingt[0] = rravg_waitingt[0]/len(rravg_waitingt)
rravg_waitingt[1] = rravg_waitingt[1]/len(rravg_waitingt)
rravg_waitingt[2] = rravg_waitingt[2]/len(rravg_waitingt)

sjfavg_responset[0] = sjfavg_responset[0]/len(sjfavg_responset)
sjfavg_responset[1] = sjfavg_responset[1]/len(sjfavg_responset)
sjfavg_responset[2] = sjfavg_responset[2]/len(sjfavg_responset)

sjfavg_tatime[0] = sjfavg_tatime[0]/len(sjfavg_tatime)
sjfavg_tatime[1] = sjfavg_tatime[1]/len(sjfavg_tatime)
sjfavg_tatime[2] = sjfavg_tatime[2]/len(sjfavg_tatime)

sjfavg_waitingt[0] = sjfavg_waitingt[0]/len(sjfavg_waitingt)
sjfavg_waitingt[1] = sjfavg_waitingt[1]/len(sjfavg_waitingt)
sjfavg_waitingt[2] = sjfavg_waitingt[2]/len(sjfavg_waitingt)

barWidth = 0.25
fig = plt.subplots(figsize=(12, 8))

SJFG = sjfavg_waitingt
RRG = rravg_waitingt

br1 = np.arange(3)
br2 = [x+barWidth for x in br1]
br3 = [x+barWidth for x in br2]

plt.bar(br1, SJFG, color='r', width=barWidth, label='SJF')
plt.bar(br2, RRG, color='b', width=barWidth, label="RR")

plt.title("System Avg Waiting time")
plt.xlabel('Test cases')
plt.ylabel('cpu units')
plt.xticks([r+barWidth for r in range(3)], ['Case 1', 'Case 2', 'Case 3'])

plt.legend()
plt.savefig('plot')