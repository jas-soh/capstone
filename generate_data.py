import numpy as np
import matplotlib.pyplot as plt

t = np.linspace(0, 1, 1000, False)  # 1 second
sig = (np.sin(8*np.pi*10*t) + np.sin(2*np.pi*20*t) ) + 5
plt.plot(t,sig)
plt.show()

# print(sig)