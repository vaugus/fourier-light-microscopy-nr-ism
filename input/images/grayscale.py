import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 1, 5)
image = np.tile(x, (5, 1)).T

print(image.shape)
print(image.size)
print(image)

np.savetxt('10.txt', image, fmt='%.4f', delimiter=',')

plt.imshow(image, cmap='gray')
plt.show()
