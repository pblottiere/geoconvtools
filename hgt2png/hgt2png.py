# -*- coding: utf-8 -*-

import struct
import numpy as np
import sys
from pylab import imshow
from pylab import grid
from pylab import show
from pylab import figure
from pylab import savefig
from pylab import axis
import matplotlib.cm as cm


HGT_WIDTH = 1201
HGT_HEIGHT = HGT_WIDTH


class hgtFile(object):
    def __init__(self, filename):
        fi = open(filename, "rb")
        contents = fi.read()
        fi.close()

        z = struct.unpack(">1442401H", contents)
        zz = np.zeros((HGT_WIDTH, HGT_HEIGHT))
        for r in range(0, HGT_WIDTH):
            for c in range(0, HGT_HEIGHT):
                va = z[(HGT_HEIGHT * r) + c]
                if (va == 65535 or va < 0 or va > 2000):
                    va = 0.0
                zz[r][c] = float(va)

        self.data = np.log1p(zz)

    def show(self):
        imshow(self.data, interpolation='bilinear', cmap=cm.gray, alpha=1.0)
        grid(False)
        show()

    def saveas(self, png):
        fig = figure(dpi=HGT_WIDTH)
        fig.set_size_inches(1.0, 1.0)

        ax = fig.add_subplot(1, 1, 1)
        ax.set_axis_off()
        fig.add_axes(ax)

        fig.subplots_adjust(bottom = 0)
        fig.subplots_adjust(top = 1)
        fig.subplots_adjust(right = 1)
        fig.subplots_adjust(left = 0)

        ax.imshow(self.data, interpolation='bilinear', cmap=cm.gray, alpha=1.0)
        fig.savefig(png, dpi=HGT_WIDTH)


if __name__ == '__main__':
    hgt = hgtFile(sys.argv[1])
    hgt.show()
    hgt.saveas("hgt.png")
