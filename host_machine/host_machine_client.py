import time
import socket

while 1:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(1.0)
    message = b'test'
    addr = ("192.168.1.119", 10000)

    start = time.time()
    client_socket.sendto(message, addr)
    try:
        data, server = client_socket.recvfrom(1024)
        end = time.time()
        elapsed = end - start
        print("{}".format(data.decode("utf-8")))
        time.sleep(.2)
    except socket.timeout:
        print('REQUEST TIMED OUT')