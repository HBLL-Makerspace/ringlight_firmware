from observer import Observer, Subject
from typing import List
from PyQt5 import QtCore, QtWidgets, QtSerialPort
from PyQt5.QtGui import QColor

class TerminalSubject(Subject):
    """
    The Subject owns some important state and notifies observers when the state
    changes.
    """

    _isConnected: bool = False
    _port: str = None
    _new_line: str = None
    _observers: List[Observer] = []
    _serial_port = None
    """
    List of subscribers. In real life, the list of subscribers can be stored
    more comprehensively (categorized by event type, etc.).
    """

    def __init__(self):
        self.numRingLights = 0

    def attach(self, observer: Observer) -> None:
        print("Terminal: Attached observer: " + observer.__str__())
        self._observers.append(observer)

    def detach(self, observer: Observer) -> None:
        self._observers.remove(observer)

    """
    The subscription management methods.
    """

    def notify(self) -> None:
        """
        Trigger an update in each subscriber.
        """
        for observer in self._observers:
            observer.update(self)

    # @QtCore.pyqtSlot()
    def receive(self):
        while self._serial_port.canReadLine():
            text = self._serial_port.readLine().data().decode()
            text = text.rstrip('\r')
            self._new_line = text
            self.notify()
            self._new_line = None
            
    def connect(self, port: str, baud=9600) -> None:
        self._new_line = "Connecting to " + port + "\n"
        self._isConnected = None
        self.notify()
        self._new_line = None
        try:
            self._serial_port = QtSerialPort.QSerialPort(port,
            baudRate=baud,
            readyRead=self.receive
            )
            self._serial_port.open(QtCore.QIODevice.ReadWrite)
            self._isConnected = True
        except:
            self._isConnected = False
        self.notify()

    def disconnect(self):
        self._new_line = "Disconnecting...\n"
        self._serial_port.close()
        self._isConnected = False
        self.notify()

class AppState(Subject):
    terminal: TerminalSubject = TerminalSubject()
    def __init__(self):
        self.numRingLights = 0
        self.selectedRingLight = 0
        self.observers = []
        self.numRingLights
        self.colors = [None]
        self.intensity = [None]
        self.shutter_enabled = [None]
        self.focus_enabled = [None]

    def attach(self, observer: Observer):
        self.observers.append(observer)

    def detach(self, observer: Observer):
        self.observers.remove(observer)

    def updateNumRingLights(self, numRingLights):
        self.numRingLights = numRingLights
        self.colors = [None] * numRingLights
        self.intensity = [None] * numRingLights
        self.focus_enabled = [False] * numRingLights
        self.shutter_enabled = [False] * numRingLights
        self.notify()

    def updateSelectedRingLight(self, num):
        self.selectedRingLight = num
        self.notify()

    def updateRingLightColor(self, ringlight_id: int, color: QColor):
        self.colors[ringlight_id] = color
        self.notify()

    def updateIntensity(self, ringlight_id: int, intensity: int):
        self.intensity[ringlight_id] = intensity
        self.notify()

    def updateFocus(self, ringlight_id: int, focus):
        self.focus_enabled[ringlight_id] = focus
        self.notify()

    def updateShutter(self, ringlight_id: int, shutter):
        self.shutter_enabled[ringlight_id] = shutter
        self.notify()

    def sendDataSelected(self):
        print("Sending data to ringlight")

    def sendDataAll(self):
        print("Sending data to all ringlights")

    def copy_to_all(self, ringlight_id: int):
        color = self.colors[ringlight_id]
        focus_enabled = self.focus_enabled[ringlight_id]
        shutter_enabled = self.shutter_enabled[ringlight_id]
        intensity = self.intensity[ringlight_id]
        for x in range(self.numRingLights):
            self.intensity[x] = intensity
            self.colors[x] = color
            self.focus_enabled[x] = focus_enabled
            self.shutter_enabled[x] = shutter_enabled
            self.intensity[x] = intensity
        
        self.notify()

    def notify(self) -> None:
        """
        Trigger an update in each subscriber.
        """
        for observer in self.observers:
            observer.update(self)
    