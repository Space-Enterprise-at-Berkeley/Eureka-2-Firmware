import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

if __name__ == "__main__":
    print("Loading server...")
    remote = "10.0.0.42"
    port = 42069 
    
    # sock.bind(("0.0.0.0", 8888)) # bind address to 0.0.0.0
    # socket.connect((remote, port))
    print("Sending data to", remote, ":", port)
    tsent = 0
    while (True):
        msg = b'ereg'
        sent = sock.sendto(msg, (remote, port))
        if (sent == 0):
            raise RuntimeError("Socket disconnected")
        time.sleep(0.1)
        tsent+=1
