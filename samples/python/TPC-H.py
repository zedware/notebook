# https://pythonspot.com/matplotlib/
import numpy as np
import matplotlib.pyplot as plt

plt.rcdefaults()
# "S3" is b'Q1' etc.
x, y1, y2 = np.loadtxt('../../dot/TPC-H.dat',
                       dtype={'names': ('Query', 'DB1', 'DB2'),
                              'formats': ('U3', 'float', 'float')},
                       delimiter='\t',
                       unpack=True)

y_pos = np.arange(len(x))
w = 0.3
plt.bar(y_pos - w / 2, y1, width=w, align='center', alpha=0.5)
plt.bar(y_pos + w / 2, y2, width=w, align='center', alpha=0.5)
plt.xticks(y_pos, x, fontsize=6)
plt.ylabel('Seconds', fontsize=6)
plt.title('TPC-H')
plt.legend(('p1', 'p2'))

plt.show()
