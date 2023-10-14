import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd

# Gyro data
gyro_measurement_limit = 8.7266
accelerometer_measurement_limit = 2
dataFrame = pd.read_csv('./12-10-2023__19-45_dataset.csv')
gyro_x = dataFrame['GYRO_X_DATA']
gyro_y = dataFrame['GYRO_Y_DATA']
gyro_z = dataFrame['GYRO_Z_DATA']
x_data = dataFrame['X_DATA']
y_data = dataFrame['Y_DATA']
z_data = dataFrame['Z_DATA']

# 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Set labels for the axes
ax.set_xlabel('Gyro X')
ax.set_ylabel('Gyro Y')
ax.set_zlabel('Gyro Z')


# Function to update the gyro plot in the animation
def update_gyro(frame):
    ax.cla()  
    ax.set_xlabel('Gyro X')
    ax.set_ylabel('Gyro Y')
    ax.set_zlabel('Gyro Z')
    ax.set_xlim([-gyro_measurement_limit, gyro_measurement_limit])  # X-axis limits
    ax.set_ylim([-gyro_measurement_limit, gyro_measurement_limit])  # Y-axis limits
    ax.set_zlim([-gyro_measurement_limit, gyro_measurement_limit])  # Z-axis limits

    # Plot the new gyro data as an orientation from 0, 0, 0
    ax.plot([0.0, gyro_x[frame]], [0.0, gyro_y[frame]], [0.0, gyro_z[frame]], marker='o', markersize=5)
    plt.title(f'Real-Time Data Stream')

# Function to update the accelerometer plot in the animation
def update_accelerometer(frame):
    ax.cla()  
    ax.set_xlabel('Accelerometer X')
    ax.set_ylabel('Accelerometer Y')
    ax.set_zlabel('Accelerometer Z')
    ax.set_xlim([-accelerometer_measurement_limit, accelerometer_measurement_limit])  # X-axis limits
    ax.set_ylim([-accelerometer_measurement_limit, accelerometer_measurement_limit])  # Y-axis limits
    ax.set_zlim([-accelerometer_measurement_limit, accelerometer_measurement_limit])  # Z-axis limits

    # Plot the new gyro data as an orientation from 0, 0, 0
    ax.plot([0.0, x_data[frame]], [0.0, y_data[frame]], [0.0, z_data[frame]], marker='o', markersize=5)
    plt.title(f'Real-Time Data Stream')



# Creates the gyro and accelerometer animation
gyro_ani = FuncAnimation(fig, update_gyro, interval=100, cache_frame_data=False)  # Interval in milliseconds
#accelerometer_ani = FuncAnimation(fig, update_accelerometer, interval=100, cache_frame_data=False)  # Interval in milliseconds

# Display the real-time animation
plt.show()