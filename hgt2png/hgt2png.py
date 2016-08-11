# -*- coding: utf-8 -*-

import struct
import numpy as np
import sys
from pylab import imshow
from pylab import grid
from pylab import show
import matplotlib.cm as cm


class hgtFile(object):
    def __init__(self, filename):
        fi = open(filename, "rb")
        contents = fi.read()
        fi.close()

        z = struct.unpack(">1442401H", contents)
        zz = np.zeros((1201, 1201))
        for r in range(0, 1201):
            for c in range(0, 1201):
                va = z[(1201 * r) + c]
                if (va == 65535 or va < 0 or va > 2000):
                    va = 0.0
                zz[r][c] = float(va)

        self.data = np.log1p(zz)

    def show(self):
        imshow(self.data, interpolation='bilinear', cmap=cm.gray, alpha=1.0)
        grid(False)
        show()

    def saveas(self, png):
        pass


if __name__ == '__main__':
    hgt = hgtFile(sys.argv[1])
    hgt.show()
