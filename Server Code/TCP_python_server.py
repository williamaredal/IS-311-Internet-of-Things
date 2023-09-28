import socket
import struct


# Define the IP address and port to listen on
host = '172.20.10.3'  # Replace with the actual IP address of your Mac
port = 12345  # Replace with the desired port number

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the address and port
server_socket.bind((host, port))

# Listen for incoming connections (maximum 1 connection at a time)
server_socket.listen(1)

print(f"Listening for incoming connections on {host}:{port}")
 
while True:
    # Accept incoming connection
    client_socket, client_address = server_socket.accept()
    print(f"Accepted connection from {client_address}")

    # Receive and unpack the sensor data
    x_data = client_socket.recv(4)
    y_data = client_socket.recv(4)
    z_data = client_socket.recv(4)
    gyro_x_data = client_socket.recv(4)
    gyro_y_data = client_socket.recv(4)
    gyro_z_data = client_socket.recv(4)
    temp_data = client_socket.recv(4)

    mic_data = client_socket.recv(4)

    gps_lat_data = client_socket.recv(4)
    gps_long_data  = client_socket.recv(4)
    gps_velocity_data = client_socket.recv(4)
    gps_sattelites = client_socket.recv(4)

    if (
        len(x_data) == len(y_data) == len(z_data) == len(gyro_x_data) ==
        len(gyro_y_data) == len(gyro_z_data) == len(temp_data) == 
        len(mic_data) == 4
    ):
        x = struct.unpack('f', x_data)[0]
        y = struct.unpack('f', y_data)[0]
        z = struct.unpack('f', z_data)[0]
        gyro_x = struct.unpack('f', gyro_x_data)[0]
        gyro_y = struct.unpack('f', gyro_y_data)[0]
        gyro_z = struct.unpack('f', gyro_z_data)[0]
        temp = struct.unpack('f', temp_data)[0]
        mic = struct.unpack('f', mic_data)[0]


        print(f"Received Acceleration X: {x}, Y: {y}, Z: {z}")
        print(f"Received Gyro X: {gyro_x}, Y: {gyro_y}, Z: {gyro_z}")
        print(f"Received Temperature: {temp}")
        print(f"Received Microphone: {mic}")


    # Close the client socket
    client_socket.close()
