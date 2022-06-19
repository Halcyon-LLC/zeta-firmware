from sys import platform

import matplotlib.pyplot as plt
import numpy as np
from serial import Serial
from serial.tools import list_ports


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


ser = Serial(autodetect_port(), 9600)
ser.close()
ser.open()

from matplotlib.animation import FuncAnimation
from scipy.interpolate import interp2d


def up_sample(raw_data):
    num_rows, num_cols = 4, 8
    res_scale = 10

    x, y = np.linspace(0, num_rows - 1,
                       num_rows), np.linspace(0, num_cols - 1, num_cols)
    xx, yy = np.linspace(0, num_rows - 1, num_rows * res_scale), np.linspace(
        0, num_cols - 1, num_cols * res_scale)

    f = interp2d(x, y, raw_data, kind='linear')
    highres_data = f(xx, yy).T

    return highres_data


def animate(i):
    raw_data = ser.readline().decode()[:-1]
    raw_data = np.fromstring(raw_data, dtype=int, sep=',')

    highres_data = up_sample(raw_data)

    plt.pcolormesh(highres_data, cmap='coolwarm')


ani = FuncAnimation(plt.gcf(), animate)
plt.show()
