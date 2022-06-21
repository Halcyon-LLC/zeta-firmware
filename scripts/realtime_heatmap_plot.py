from sys import platform
from functools import lru_cache

from serial import Serial
from serial.tools import list_ports
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.interpolate import interp2d


def autodetect_port():
    ports = list_ports.comports()
    if not ports:
        raise LookupError('No ports detected')

    arduino_port = None
    for port in ports:
        name = port.__str__().lower()
        if port.vid is not None or 'acm' in name or 'usbserial' in name or 'arduino' in name:
            arduino_port = port.device

    if arduino_port is None:
        raise LookupError('Arduino not detected')
    else:
        return arduino_port


@lru_cache(maxsize=2)
def setup_coordinates(num_rows, num_cols, scale):
    return [
        np.linspace(0, num_rows - 1, num_rows),
        np.linspace(0, num_cols - 1, num_cols),
        np.linspace(0, num_rows - 1, num_rows * scale),
        np.linspace(0, num_cols - 1, num_cols * scale)
    ]


def up_sample(raw_data):
    num_rows, num_cols = 4, 8
    res_scale = 25

    (x, y, xx, yy) = setup_coordinates(num_rows, num_cols, res_scale)

    f = interp2d(x, y, raw_data, kind='cubic')
    highres_data = f(xx, yy).T

    return highres_data


def animate(i, ser):
    plt.clf()
    raw_data = np.fromstring(ser.readline().decode()[:-1], dtype=int, sep=',')

    highres_data = up_sample(raw_data)

    plt.pcolormesh(highres_data, cmap='coolwarm')


if __name__ == '__main__':
    with Serial(autodetect_port(), 9600) as serial_port:
        ani = FuncAnimation(plt.gcf(), animate(ser=serial_port))
        plt.show()
