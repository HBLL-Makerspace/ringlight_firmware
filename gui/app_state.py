from observer import Observer, Subject
from typing import List
from PyQt5 import QtCore, QtWidgets, QtSerialPort
from PyQt5.QtGui import QColor
from command import *
import time

class TerminalSubject(Subject):
    """
    The Subject owns some important state and notifies observers when the state
    changes.
    """

    """
    List of subscribers. In real life, the list of subscribers can be stored
    more comprehensively (categorized by event type, etc.).
    """

    def __init__(self):
        self._isConnected: bool = False
        self._port: str = None
        self._new_line: str = None
        self._observers: List[Observer] = []
        self._serial_port = None

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

    def write(self, data):
        print(data)
        self._serial_port.write(data)
        self._serial_port.flush()

class AppState(Subject):
    terminal: TerminalSubject = TerminalSubject()
    def __init__(self):
        self.numRingLights = 0
        self.selectedRingLight = 0
        self.selectedChannel = 0
        self.numChannels = 3
        self.observers = []
        self.numRingLights
        self.colors = [[QColor(0, 0, 0)]]
        self.intensity = [None]
        self.shutter_enabled = [None]
        self.focus_enabled = [None]
        self.useWhite = [[False]]

    def attach(self, observer: Observer):
        self.observers.append(observer)

    def detach(self, observer: Observer):
        self.observers.remove(observer)

    def updateNumRingLights(self, numRingLights):
        self.numRingLights = numRingLights
        self.colors = [[QColor(0, 0, 0) for i in range(self.numChannels)] for j in range(self.numRingLights)]
        self.useWhite = [[False for i in range(self.numChannels)] for j in range(self.numRingLights)]
        self.intensity = [100 for j in range(self.numRingLights)]
        self.focus_enabled = [False for j in range(self.numRingLights)]
        self.shutter_enabled = [False for j in range(self.numRingLights)]
        self.notify()

    def updateUseWhite(self, ringlight_id: int, channel_index: int, useWhite: bool):
        self.useWhite[ringlight_id][channel_index] = useWhite;
        self.notify()

    def updateSelectedRingLight(self, num):
        self.selectedRingLight = num
        self.notify()

    def updateSelectedChannel(self, num):
        self.selectedChannel = num
        self.notify()

    def updateRingLightColor(self, ringlight_id: int, channel_index: int, color: QColor):
        self.colors[ringlight_id][channel_index] = color
        self.notify()
        # for i in range(self.numRingLights):
        #     for j in range(self.numChannels):
        #         colorP = self.colors[i][j]
        #         print(str(colorP.name()), end=", ")
        #     print()


    def updateIntensity(self, ringlight_id: int, intensity: int):
        self.intensity[ringlight_id] = intensity
        self.notify()

    def updateFocus(self, ringlight_id: int, focus):
        self.focus_enabled[ringlight_id] = focus
        self.notify()

    def updateShutter(self, ringlight_id: int, shutter):
        self.shutter_enabled[ringlight_id] = shutter
        self.notify()

    def sendCommand(self, cmd: Command):
        for i in cmd.command_to_binary():
            # print(type(cmd[i]))
            self.terminal.write(i.to_bytes(1, "little"))
            # time.sleep(0.1);
        # print(cmd.command_to_binary())

    def sendDataSelected(self):
        color = self.colors[self.selectedRingLight][self.selectedChannel]
        cmd = None
        if self.useWhite[self.selectedRingLight][self.selectedChannel]:
            cmd = CmdSetChannelW(self.selectedRingLight, self.selectedChannel, max(max(color.red(), color.green()), color.blue()))
        else:
            cmd = CmdSetChannelRGB(self.selectedRingLight, self.selectedChannel, color.red(), color.green(), color.blue())
        self.sendCommand(cmd)
        print("Sending data to ringlight")

    def sendDataAll(self):
        print("Sending data to all ringlights")

    def copy_to_all(self, ringlight_id: int):
        color = self.colors[ringlight_id]
        focus_enabled = self.focus_enabled[ringlight_id]
        shutter_enabled = self.shutter_enabled[ringlight_id]
        intensity = self.intensity[ringlight_id]
        useWhite = self.useWhite[ringlight_id]
        for x in range(self.numRingLights):
            self.intensity[x] = intensity
            self.colors[x] = color
            self.focus_enabled[x] = focus_enabled
            self.shutter_enabled[x] = shutter_enabled
        
        self.notify()

    def copy_to_all_chns(self, ringlight_id: int, channel: int):
        color = self.colors[ringlight_id][channel]
        useWhite = self.useWhite[ringlight_id][channel]
        for x in range(self.numChannels):
            self.colors[ringlight_id][x] = color
            self.useWhite[ringlight_id][x] = useWhite
            
    
    def reset_channel(self, ringlight_id: int, channel: int, notify = True):
        self.colors[ringlight_id][channel] = QColor(0, 0, 0)
        self.useWhite[ringlight_id][channel] = False
        if notify:
            self.notify()

    def reset_ring_light(self, ringlight_id: int):
        for i in range(self.numChannels):
            self.reset_channel(ringlight_id, i, notify=False)
        
        self.intensity[ringlight_id] = 100
        self.shutter_enabled[ringlight_id] = False
        self.focus_enabled[ringlight_id] = False
        self.notify()

    def reset(self):
        for i in range(self.numRingLights):
            self.reset_ring_light(i)


    def notify(self) -> None:
        """
        Trigger an update in each subscriber.
        """
        for observer in self.observers:
            observer.update(self)
    