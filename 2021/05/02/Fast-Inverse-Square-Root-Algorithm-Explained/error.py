import struct
import numpy as np
import matplotlib.pyplot as plt

sz = 1 << 20

with open("./output.bin", "rb") as f:
    form = "f" * sz
    xs = np.array(struct.unpack(form, f.read(struct.calcsize(form))))
    results = np.array(struct.unpack(form, f.read(struct.calcsize(form))))
    grounds = np.array(struct.unpack(form, f.read(struct.calcsize(form))))

plt.loglog(xs, np.abs(grounds - results), base=2)
plt.show()