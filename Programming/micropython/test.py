import time
import socket
import network
import ujson as json  # MicroPython JSON module

# Function to connect to Wi-Fi
def connect():
    ssid = "your_wifi_ssid"
    password = "your_wifi_password"
    
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

# WebSocket handler
def handle_websocket(client, addr):
    print("WebSocket client connected from:", addr)
    try:
        while True:
            msg = client.recv(1024)
            if not msg:
                break
            data = json.loads(msg)
            print("Received message:", data)
            # Process received data here (e.g., control other devices)
    except Exception as e:
        print("WebSocket error:", e)
    finally:
        client.close()
        print("WebSocket client disconnected")

# Main server function
def server():
    ip = connect()
    if ip is None:
        return
    
    addr = socket.getaddrinfo(ip, 8080)[0][-1]
    
    s = socket.socket()
    s.bind(addr)
    s.listen(1)
    
    print(f"Listening on ws://{ip}:8080/ws")
    
    while True:
        client, addr = s.accept()
        try:
            # Check if it's a WebSocket handshake
            request = client.recv(1024)
            if b"GET /ws HTTP/1.1" in request:
                client.send(b"HTTP/1.1 101 Switching Protocols\r\n"
                            b"Upgrade: websocket\r\n"
                            b"Connection: Upgrade\r\n"
                            b"Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n"
                            b"\r\n")
                handle_websocket(client, addr)
            else:
                client.close()
        except Exception as e:
            print("Server error:", e)
            client.close()

# Run the server
server()
