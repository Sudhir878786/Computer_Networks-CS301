# importing the required module
import matplotlib.pyplot as plt

# x axis values
x = [2,10,18,5,11]
# corresponding y axis values
y = [4196.4,4171.44,4146.77,4187,4168.34]

# plotting the points
plt.plot(x, y)

# naming the x axis
plt.xlabel('Delay')
# naming the y axis
plt.ylabel('Through Put')

# giving a title to my graph
plt.title('Through Put V/S Delay')

# function to show the plot
plt.show()

