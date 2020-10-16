import serial
import serial.tools.list_ports
import threading
import time
from kivy.properties import BooleanProperty, StringProperty
from kivy.event import EventDispatcher

class Board(EventDispatcher):
    message_string = StringProperty('')
    connected = BooleanProperty(False)
    def __init__(self, **kwargs):
        self.port = None
        self.baudrate = None
        self.ser = serial.Serial()
        self.on_connect_callback = None
        self.is_connected = False
        thread = threading.Thread(target=self.find_port)
        thread.daemon = True
        try:
            thread.start()
        except:
            raise

    def find_port(self):
        device_found = False
        while (not device_found):
            ports = serial.tools.list_ports.comports()
            for port in ports:
                device_found = self.check_device(port.device)
                if (device_found):
                    self.port = port.device
                    self.baudrate = 9600
                    time.sleep(2)
                    try:
                        self.ser = serial.Serial(
                            port=self.port, baudrate=self.baudrate, write_timeout=0, timeout=2)
                        if (self.ser.is_open):
                            self.is_connected = True
                            self.message_string = f'Successfully connected to port {self.port}'
                            self.connected = True
                            break
                    except serial.SerialException:
                        self.message_string = f'Could not connect to port {self.port}'

    def check_device(self, port):
        try:
            self.message_string = f'Attempting to connect to {port}'
            ser = serial.Serial(port=port, baudrate=9600,
                                write_timeout=0, timeout=2)
            if (ser.is_open):
                ser.write(b'v')
                time.sleep(1)
                line = ''
                counter = 0
                if (ser.in_waiting):
                    while ("$$$" not in line):
                        received = ser.read(1).decode(
                            'utf-8', errors='replace')
                        line += received
                        counter += 1
                        if (counter == 50):
                            break

                ser.close()
                if ("RGB" in line):
                    self.message_string = f'Device found at port {port}'
                    return True
                else:
                    return False

        except serial.SerialException:
            return False
        except ValueError:
            return False


    def write_new_color(self, r, g, b):
        # We need the send the data in a format accepted by ser.write (e.g bytearray)
        self.message_string = f'Sending color: ({int(r)}, {int(g)}, {int(b)})'
        self.ser.write(
            bytearray([0xA0, int(r), int(g), int(b), 0xC0]))
