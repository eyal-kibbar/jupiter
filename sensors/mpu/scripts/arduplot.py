#! /usr/bin/env python

import serial
import fire
import re
import numpy as np
import matplotlib.pyplot as plt
import time
import collections
import matplotlib.gridspec as gridspec
import sys
import logging
from scipy.spatial.transform import Rotation as R


class SerialParser(serial.Serial):
    '''
    parses the serial output using a regular expression

    :param line_regex: regular expression for the parsing a single line
    :param groups_names: names of the parsed groups of the regular expression
    :param parsed_line_name: name of the parsed object
    '''

    def __init__(self, line_regex : str, groups_names : list, parsed_line_name : str, **kwargs):
        super().__init__(**kwargs)
        self._line_regex = re.compile(line_regex)
        self._parsed_obj = collections.namedtuple(parsed_line_name, groups_names)


    def _parserline(self):
        line = super().readline()
        try:
            line = line.decode('ascii')
        except UnicodeDecodeError as e:
            logging.warning(f'decode error: {e}')
            return None
        line = line.strip()
        m = re.search(self._line_regex, line)
        if m is None:
            logging.warning(f'regex mismatch: "{line}"')
            return None

        return self._parsed_obj(*[int(x) for x in m.groups()])

    def readline(self):
        while True:
            res = self._parserline()
            if res is not None:
                return res


class SensorPlot:

    '''
    continously reads lines from the serial port and plots the sensor data

    :param port: the serial port (e.g. /dev/ttyUSB0)
    :param baud: the serial port's baud rate (e.g. 9600, 57600)
    :param nsamples: number of samples showing in the window
    '''
    
    def __init__(self, port, baud, nsamples : int = 10):

        self._port = port
        self._baud = baud

        plt.ion()
        self._figure, ((ax1, ax2), (ax3, ax4)) = plt.subplots(ncols=2, nrows=2, figsize=(8,6))

        self._obj = np.array([
            [-1, -1, -1],
            [ 1, -1, -1],
            [ 1,  1, -1],
            [-1,  1, -1],
            [-1, -1, -1],


            [-1, -1,  1],
            [ 1, -1,  1],
            [ 1,  1,  1],
            [-1,  1,  1],
            [-1, -1,  1],

            [ 1, -1,  1],
            [ 1, -1, -1],
            [ 1, -1,  1],

            [ 1,  1,  1],
            [ 1,  1,  -1],
            [-1,  1,  -1],
            [-1,  1,  1]

            ])

        self._obj[:, 0] *= 6
        self._obj[:, 1] *= 3

        a = self._obj
        ax4.remove()
        ax4 = self._figure.add_subplot(2, 2, 4, projection='3d')
        ax4.set_xlim([-7, 7])
        ax4.set_ylim([-7, 7])
        ax4.set_zlim([-7, 7])

        self._obj_line, = ax4.plot(a[:, 0], a[:, 1], a[:, 2])

        ax1.set_ylim([-90, 90])
        ax2.set_ylim([-90, 90])
        ax3.set_ylim([-90, 90])

        self._accel = np.zeros((nsamples, 3))
        self._gyro  = np.zeros((nsamples, 3))
        self._angle = np.zeros((nsamples, 3))

        self._x = np.linspace(0, nsamples, nsamples)

        self._accel_line = []
        self._gyro_line = []
        self._angle_line = []

        for axis, ax in zip(range(3), [ax1, ax2, ax3]):
            line, = ax.plot(self._x, self._accel[:, axis], 'b-')
            self._accel_line.append(line)

            line,  = ax.plot(self._x, self._gyro[:, axis], 'r-')
            self._gyro_line.append(line)

            line, = ax.plot(self._x, self._angle[:, axis], 'g-')
            self._angle_line.append(line)

    def show(self):
        with SerialParser(
            line_regex='# ' + ','.join( ['(\-?\d+)' for x in range(9)] ),
            groups_names=['accel_x', 'gyro_x', 'angle_x', 'accel_y', 'gyro_y', 'angle_y', 'gyro_z', 'angle_z', 'elapsed_time'],
            parsed_line_name='angles',
            port=self._port,
            baudrate=self._baud) as sp:

            while True:
                x = sp.readline()
                logging.info(x)

                # accel
                self._accel = np.roll(self._accel, -1, axis=0)
                self._accel[-1, :] = (x.accel_x, x.accel_y, 0)

                # gyro
                self._gyro = np.roll(self._gyro, -1, axis=0)
                self._gyro[-1, :] = (x.gyro_x, x.gyro_y, x.gyro_z)

                # angle
                self._angle = np.roll(self._angle, -1, axis=0)
                self._angle[-1, :] = (x.angle_x, x.angle_y, x.angle_z)


                r = R.from_euler('xyz', self._angle[-1, :], degrees=True)
                a = r.apply(self._obj)

                self._obj_line.set_data_3d(a[:, 0], a[:, 1], a[:, 2])

                # X axis
                for axis in range(3):
                    self._accel_line[axis].set_xdata(self._x)
                    self._accel_line[axis].set_ydata(self._accel[:, axis])

                    self._gyro_line[axis].set_xdata(self._x)
                    self._gyro_line[axis].set_ydata(self._gyro[:, axis])

                    self._angle_line[axis].set_xdata(self._x)
                    self._angle_line[axis].set_ydata(self._angle[:, axis])

                self._figure.canvas.flush_events()



if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    fire.Fire(SensorPlot)
