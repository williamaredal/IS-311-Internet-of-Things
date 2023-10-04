from datetime import datetime
import csv
import socket
import struct


# Define the IP address and port to listen on
host = ''  # Replace with the actual IP address of your Mac (use 'ifconfic' in the terminal, and find the ip under 'en0')
port = 12345  # Replace with the desired port number

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the address and port
server_socket.bind((host, port))

# Listen for incoming connections (maximum 1 connection at a time)
server_socket.listen(1)

print(f"Listening for incoming connections on {host}:{port}")


current_datetime = datetime.now().strftime('%d-%m-%Y__%H-%M')
dataset_name = f'{current_datetime}_dataset.csv'
column_headers = [
    'X_DATA', 'Y_DATA', 'Z_DATA', 'GYRO_X_DATA', 'GYRO_Y_DATA', 'GYRO_Z_DATA', # Accelerometer and Gyro data
    'TEMP_DATA', # Temperature data
    'MIC_DATA', # Microphone data
    'LATITUDE', 'LONGITUDE', 'ALTITUDE', 'COURSE', 'SPEED', 'SATELLITES' # GPS data
]

with open(dataset_name, 'w+', newline='') as csvFile:
    csv_writer = csv.writer(csvFile)

    # Checks if the file has column headers
    csvFile.seek(0)
    first_character = csvFile.read(1)
    if not first_character:
        csv_writer.writerow(column_headers)

    while True:
        # Accept incoming connection
        client_socket, client_address = server_socket.accept()
        print("-----------------------")
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

        lat_bytes = client_socket.recv(8)
        long_bytes = client_socket.recv(8)
        alt_bytes = client_socket.recv(8)
        course_bytes = client_socket.recv(8)
        speed_bytes = client_socket.recv(8)
        sat_bytes = client_socket.recv(4)

        if (
            len(x_data) == len(y_data) == len(z_data) == len(gyro_x_data) ==
            len(gyro_y_data) == len(gyro_z_data) == len(temp_data) ==
            len(mic_data) == len(sat_bytes) == 4 and len(lat_bytes) == len(long_bytes) == len(alt_bytes) == len(course_bytes) ==
            len(speed_bytes) == 8
        ):
            x = struct.unpack('f', x_data)[0]
            y = struct.unpack('f', y_data)[0]
            z = struct.unpack('f', z_data)[0]
            gyro_x = struct.unpack('f', gyro_x_data)[0]
            gyro_y = struct.unpack('f', gyro_y_data)[0]
            gyro_z = struct.unpack('f', gyro_z_data)[0]
            temp = struct.unpack('f', temp_data)[0]

            mic = struct.unpack('I', mic_data)[0]

            lat = struct.unpack('d', lat_bytes)[0]
            long = struct.unpack('d', long_bytes)[0]
            alt = struct.unpack('d', alt_bytes)[0]
            course = struct.unpack('d', course_bytes)[0]
            speed = struct.unpack('d', speed_bytes)[0]
            sat = struct.unpack('I', sat_bytes)[0]

            # Get the current datetime with precision
            current_datetime = datetime.now().strftime('%d-%m-%Y__%H-%M-%S-%f')

            # Writes the data to a new row in the csv file
            data_row = [
                current_datetime,
                x, y, z, gyro_x, gyro_y, gyro_z, temp,
                mic,
                lat, long, alt, course, speed, sat
            ]
            csv_writer.writerow(data_row)

            print(f"Received Acceleration X: {x}, Y: {y}, Z: {z}")
            print(f"Received Gyro X: {gyro_x}, Y: {gyro_y}, Z: {gyro_z}")
            print(f"Received Temperature: {temp}")

            print(f"Received Microphone: {mic}")

            print(f"Received Latitude: {lat}")
            print(f"Received Longitude: {long}")
            print(f"Received Altitude: {alt}")
            print(f"Received Course: {course}")
            print(f"Received Speed: {speed}")
            print(f"Received Satellites: {sat}")

        # Close the client socket
        client_socket.close()
