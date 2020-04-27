#jeden argument- wynik z gry w cpp

import matplotlib.pyplot as plt
import numpy as np
import sys

def moving_average(a, n=3) :
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n

with open (sys.argv[1], "r") as myfile:
    data=eval(myfile.read())

#scores = [i[0] for i in data]
won_games = [1 if i[1] >= 2048 else 0 for i in data]

plt.plot(moving_average(np.array(won_games), 100))
plt.ylim(0,1)
plt.show()