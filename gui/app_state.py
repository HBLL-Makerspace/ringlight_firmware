from observer import Observer, Subject
from typing import List
from PyQt5 import QtCore, QtWidgets, QtSerialPort

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
    numRingLights = 0
    """
    List of subscribers. In real life, the list of subscribers can be stored
    more comprehensively (categorized by event type, etc.).
    """

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

class AppState():
    terminal: TerminalSubject = TerminalSubject()
    