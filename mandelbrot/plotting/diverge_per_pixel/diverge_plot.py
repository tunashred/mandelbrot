import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt

with open('diverge_data_galaxy_4.csv', 'r') as file:
    data = [int(line.strip()) for line in file]

# Plot the histogram
plt.hist(data, bins=range(0, max(data) + 1), edgecolor='black')
plt.title("Histogram of Divergence Iteration Counts")
plt.xlabel("Iteration Count")
plt.ylabel("Number of Pixels")
plt.show()
