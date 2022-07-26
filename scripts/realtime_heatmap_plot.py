from sys import platform
from functools import lru_cache

import click
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


class VelostatMat():

    def __init__(self, serial_port: Serial, num_pwr: int, num_gnd: int, resolution_scale: int):
        self.serial_port = serial_port
        self.num_pwr = num_pwr
        self.num_gnd = num_gnd
        self.resolution_scale = resolution_scale

    @lru_cache(maxsize=2)
    def _setup_coordinates(self, num_rows, num_cols, scale):
        return [
            np.linspace(0, num_rows - 1, num_rows),
            np.linspace(0, num_cols - 1, num_cols),
            np.linspace(0, num_rows - 1, num_rows * scale),
            np.linspace(0, num_cols - 1, num_cols * scale)
        ]

    def get_readings(self, dtype: type = int):
        """Parse string sent from MCU.
            MCU sends data from all pressure points in one line.
            The parsed data will be reshaped later to proper size.

        Returns:
            NDArray: 1 x N matrix where N is the number of pressure points
        """
        return np.fromstring(self.serial_port.readline().decode()[:-1], dtype=dtype, sep=',')

    def up_sample(self, data, type='linear'):
        num_rows, num_cols = self.num_gnd, self.num_pwr
        (x, y, xx, yy) = self._setup_coordinates(num_rows, num_cols, self.resolution_scale)

        f = interp2d(x, y, data, kind=type)

        return np.rot90(np.flip(f(xx, yy).T, axis=1))

    def animate(self, i):
        plt.clf()

        raw_data = self.get_readings()
        highres_data = self.up_sample(raw_data, type='cubic')

        fig = plt.figure(1)
        fig.set_size_inches(8, 8)
        plt.pcolormesh(highres_data, cmap='coolwarm')


@click.command()
@click.option('--num_pwr', '-p', type=int, required=True)
@click.option('--num_gnd', '-g', type=int, required=True)
@click.option('--resolution_scale', '-s', type=int, default=25)
def cli(num_pwr, num_gnd, resolution_scale):
    with Serial(autodetect_port(), 9600) as serial_port:
        mat = VelostatMat(serial_port, num_pwr, num_gnd, resolution_scale)
        ani = FuncAnimation(plt.gcf(), mat.animate)
        plt.show()


if __name__ == '__main__':
    cli()
