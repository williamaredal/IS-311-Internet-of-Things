import socket

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
    
    # Receive and print data from the client
    data = client_socket.recv(1024).decode('utf-8')
    print(f"Received data: {data}")
    
    # Close the client socket
    client_socket.close()

