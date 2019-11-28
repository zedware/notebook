# https://pythonspot.com/matplotlib/
import numpy as np
import matplotlib.pyplot as plt

plt.rcdefaults()
objects = ('Python', 'C++', 'Java', 'Perl', 'Scala', 'Lisp')
y_pos = np.arange(len(objects))
performance = [10, 8, 6, 4, 2, 1]
performance2 = [10, 6, 7, 8, 5, 9]

w = 0.3
plt.bar(y_pos - w/2, performance, width=w, align='center', alpha=0.5)
plt.bar(y_pos + w/2, performance2, width=w, align='center', alpha=0.5)
plt.xticks(y_pos, objects)
plt.ylabel('Usage')
plt.title('Language usage')
plt.legend(('p1', 'p2'))

plt.show()