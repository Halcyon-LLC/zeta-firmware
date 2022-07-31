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

        self.init_voltage_readings()

    def init_voltage_readings(self):
        samples = 3
        vouts = np.zeros(self.num_gnd * self.num_pwr)

        for _ in range(samples):
            vouts += self.get_readings()

        self.initial_vout = vouts / samples

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
        while True:
            try:
                self.serial_port.write(b'c')
                vin, vouts = self.serial_port.readline().decode()[:-1].split(',', maxsplit=1)
                print(vin)
                self.vin = int(vin)
                vouts = np.fromstring(vouts, dtype=dtype, sep=',')
                if vouts.shape[0] == self.num_gnd * self.num_pwr:
                    return vouts
            except ValueError as e:
                print(e)
                continue

    def up_sample(self, data, type='linear'):
        num_rows, num_cols = self.num_gnd, self.num_pwr
        (x, y, xx, yy) = self._setup_coordinates(num_rows, num_cols, self.resolution_scale)

        f = interp2d(x, y, data, kind=type)

        return f(xx, yy)

    def orientate(self, data: np.ndarray):
        return np.rot90(data, k=3)

    def animate(self, i):
        plt.clf()

        raw_data_delta = self.get_readings() - self.initial_vout
        raw_data_delta = np.where(raw_data_delta > self.vin * 0.1, raw_data_delta, 0)

        if self.resolution_scale > 1:
            highres_data = self.orientate(self.up_sample(raw_data_delta, type='cubic'))
        else:
            highres_data = self.orientate(np.reshape(raw_data_delta, (self.num_gnd, self.num_pwr)).T)

        fig = plt.figure(1)
        fig.set_size_inches(8, 8)
        plt.pcolormesh(highres_data, cmap='coolwarm')


@click.command()
@click.option('--num_pwr', '-p', type=int, required=True)
@click.option('--num_gnd', '-g', type=int, required=True)
@click.option('--resolution_scale', '-s', type=int, default=25)
def cli(num_pwr, num_gnd, resolution_scale):
    with Serial(autodetect_port(), 115200, timeout=1) as serial_port:
        mat = VelostatMat(serial_port, num_pwr, num_gnd, resolution_scale)
        ani = FuncAnimation(plt.gcf(), mat.animate)
        plt.show()


if __name__ == '__main__':
    cli()
