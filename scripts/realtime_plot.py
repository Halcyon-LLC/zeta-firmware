# %%
from sys import platform

import matplotlib.pyplot as plt
from serial import Serial
from serial.tools import list_ports


# %%
def autodetect_port():
    l = list_ports.comports()
    if not l:
        raise Exception("No ports detected")

    if platform == "linux" or platform == "linux2":
        for d in l:
            if 'ACM' in d.device or 'usbserial' in d.device:
                port = str(d.device)
    elif platform == "win32":
        comports = []
        for d in l:
            if d.device:
                devname = str(d.device)
                comports.append(devname)
        comports.sort()
        port = comports[0]
    else:
        for d in l:
            if d.vid:
                port = str(d.device)

    return port

# %%
ser = Serial(autodetect_port(), 115200)
ser.close()
ser.open()

# %%

from itertools import count

from matplotlib.animation import FuncAnimation

x_vals = []
y_vals = []

index = count()

def animate(i):
    x_vals.append(next(index))
    data = ser.readline().decode()[:-2]
    y_vals.append(data)
    plt.cla()
    plt.plot(x_vals, y_vals)


ani = FuncAnimation(plt.gcf(), animate)

plt.show()
