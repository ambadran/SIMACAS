import network
import socket
import time
import ure  # Import ure for parsing URLs
from machine import Pin

led = Pin("LED", Pin.OUT)

def connect():
    ssid = "iPhone 15 Pro"
    password = "dnjdjcnd"
    
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    
    while not wlan.isconnected() and wlan.status() == network.STAT_CONNECTING:
        print("Connecting to Wi-Fi...")
        time.sleep(1)
    
    if wlan.isconnected():
        print("Connected to Wi-Fi")
        print("IP Address:", wlan.ifconfig()[0])
        return wlan.ifconfig()[0]
    else:
        print("Failed to connect to Wi-Fi")
        return None

def web_page():
    with open('index.html', 'r') as file:
        html = file.read()
    return html

def server(processing_func):
    ip = connect()
    if ip is None:
        return

    addr = socket.getaddrinfo(ip, 80)[0][-1]
    
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(addr)
    s.listen(5)
    
    print(f"Listening on {ip}")
    
    while True:

        cl, addr = s.accept()
        print(f"Client connected from {addr}")
        try:
            led.toggle()

            request = cl.recv(1024)
            request = request.decode('utf-8')
            # print(f"Request: {request}")
            
            if 'GET /update' in request:
                string = request[:request.find('\n')]
                print(f"Processing Axe")

                str1 = string.split("GET /update?slider=slider")
                str2 = str1[1].split("&value=")
                axe_num = int(str2[0])
                str3 = str2[1].split(" HTTP/1.1")
                value = int(str3[0])


                print(f"Processing Axe {axe_num} with value {value}\n\n")
                processing_func(axe_num, int(value/10))

            else:
                response = web_page()
                cl.send(b"HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n")
                cl.send(response.encode('utf-8'))

        except Exception as e:
            print(f"Error handling request: {e}")

        finally:
            cl.close()


