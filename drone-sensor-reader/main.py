"""
    Title: Main.py
    Desc: Read Serial and make it available at /serial
"""


import os
import sys
import time
import threading

from flask import Flask, jsonify
import serial # This is actual pyserial

# Flask Info
IP = os.getenv('flask_ip') or '0.0.0.0'
PORT = os.getenv('flask_port') or 5000
PORT = int(PORT)

app = Flask(__name__)

# Serial Info
last_read_json = ""

def flask_thread():
    app.run(debug=True, use_reloader=False, host=IP, port=PORT)

@app.route('/serial', methods=['GET'])
def get_serial():
    return jsonify(last_read_json)

if __name__ == '__main__':
    conn = serial.Serial(
            port='/dev/ttyUSB0',
            baudrate=9600)

    if not conn.isOpen():
        conn.open()

    if not conn.isOpen():
        sys.exit(1)

    # Start Webserver on another thread
    thread = threading.Thread(target=flask_thread)
    thread.start()

    while True:
        last_read = conn.readline().decode().strip()
        last_read_list = last_read.split(',')
        last_read_json = {
                'x': last_read_list[0],
                'y': last_read_list[1],
                'z': last_read_list[2]
                }
