import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd

# Gyro data
gyro_measurement_limit = 8.7266
accelerometer_measurement_limit = 10
dataFrame = pd.read_csv('./12-10-2023__19-45_dataset.csv')
gyro_x = dataFrame['GYRO_X_DATA']
gyro_y = dataFrame['GYRO_Y_DATA']
gyro_z = dataFrame['GYRO_Z_DATA']
x_data = dataFrame['X_DATA']
y_data = dataFrame['Y_DATA']
z_data = dataFrame['Z_DATA']

# 3D plots for data visualization
fig = plt.figure()
fig.suptitle('Real-Time Data Visualization')
ax1 = fig.add_subplot(121, projection='3d')
ax2 = fig.add_subplot(122, projection='3d')

# Function to update the gyro plot in the animation
def update_gyro(frame):
    ax1.cla()  
    ax1.set_xlabel('Gyro X')
    ax1.set_ylabel('Gyro Y')
    ax1.set_zlabel('Gyro Z')
    ax1.set_xlim([-gyro_measurement_limit, gyro_measurement_limit])  # X-axis limits
    ax1.set_ylim([-gyro_measurement_limit, gyro_measurement_limit])  # Y-axis limits
    ax1.set_zlim([-gyro_measurement_limit, gyro_measurement_limit])  # Z-axis limits

    # Plot the new gyro data as an orientation from 0, 0, 0
    ax1.plot([0.0, gyro_x[frame]], [0.0, gyro_y[frame]], [0.0, gyro_z[frame]], marker='o', markersize=5)


# Function to update the accelerometer plot in the animation
def update_accelerometer(frame):
    ax2.cla()  
    ax2.set_xlabel('Accelerometer X')
    ax2.set_ylabel('Accelerometer Y')
    ax2.set_zlabel('Accelerometer Z')
    ax2.set_xlim([-accelerometer_measurement_limit, accelerometer_measurement_limit])  # X-axis limits
    ax2.set_ylim([-accelerometer_measurement_limit, accelerometer_measurement_limit])  # Y-axis limits
    ax2.set_zlim([-accelerometer_measurement_limit, accelerometer_measurement_limit])  # Z-axis limits

    # Plot the new gyro data as an orientation from 0, 0, 0
    ax2.plot([0.0, x_data[frame]], [0.0, y_data[frame]], [0.0, z_data[frame]], marker='o', markersize=5)



# Creates the gyro and accelerometer animation
gyro_ani = FuncAnimation(fig, update_gyro, interval=100, cache_frame_data=False)  # Interval in milliseconds
accelerometer_ani = FuncAnimation(fig, update_accelerometer, interval=100, cache_frame_data=False)  # Interval in milliseconds

# Display the real-time animation
plt.show()