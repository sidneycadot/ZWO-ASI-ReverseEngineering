#! /usr/bin/env python

import h5py
import numpy as np
from matplotlib import pyplot as plt

f = h5py.File("images.h5", "r")

data = np.array(f["images"])

data = np.std(data, axis = 0)
print data.shape, data.dtype

plt.imshow(data, cmap=plt.cm.gray, interpolation = "none")
plt.colorbar()
plt.show()

f.close()
