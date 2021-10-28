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

    #@QtCore.pyqtSlot()
    def receive(self):
        while self._serial_port.canReadLine():
            text = self._serial_port.readLine().data().decode()
            text = text.rstrip('\r')
            print(text, end= '')
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
        self.whiteColors = [[QColor(0, 0, 0)]]
        self.intensity = [None]
        self.shutter_enabled = False 
        self.focus_enabled = False
        self.dataToSend = 0

    def attach(self, observer: Observer):
        self.observers.append(observer)

    def detach(self, observer: Observer):
        self.observers.remove(observer)

    def updateNumRingLights(self, numRingLights):
        self.numRingLights = numRingLights
        self.colors = [[QColor(0, 0, 0) for i in range(self.numChannels)] for j in range(self.numRingLights)]
        self.whiteColors = [[QColor(0, 0, 0) for i in range(self.numChannels)] for j in range(self.numRingLights)]
        self.useWhite = [[False for i in range(self.numChannels)] for j in range(self.numRingLights)]
        self.intensity = [100 for j in range(self.numRingLights)]
        self.focus_enabled = [False for j in range(self.numRingLights)]
        self.shutter_enabled = [False for j in range(self.numRingLights)]
        self.notify()

    def updateDataToSend(self, index: int):
        self.dataToSend = index;
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

    def updateRingLightColorW(self, ringlight_id: int, channel_index: int, color: QColor):
        self.whiteColors[ringlight_id][channel_index] = color
        self.notify()


    def updateIntensity(self, ringlight_id: int, intensity: int):
        self.intensity[ringlight_id] = intensity
        self.notify()

    # updates focus_enabled boolean for the specified ringlight
    def updateFocus(self, ringlight_id: int, focus):
        self.focus_enabled[ringlight_id] = focus
        self.notify()

    # updates shutter_enabled boolean for the specified ringlight
    def updateShutter(self, ringlight_id: int, shutter):
        self.shutter_enabled[ringlight_id] = shutter
        self.notify()

    def sendCommand(self, cmd: Command):
        for i in cmd.command_to_binary():
            # print(type(cmd[i]))
            self.terminal.write(i.to_bytes(1, "little"))
            time.sleep(0.1)
        print(cmd.command_to_binary())

    def sendDataSelectedChannel(self):
        color = self.colors[self.selectedRingLight][self.selectedChannel]
        whiteColor = self.whiteColors[self.selectedRingLight][self.selectedChannel]
        cmd = None
        if self.dataToSend == 0:
            cmd = CmdSetChannelW(self.selectedRingLight, self.selectedChannel, max(max(whiteColor.red(), whiteColor.green()), whiteColor.blue()))
            self.sendCommand(cmd)
            cmd = CmdSetChannelRGB(self.selectedRingLight, self.selectedChannel, color.red(), color.green(), color.blue())
            self.sendCommand(cmd)
        elif self.dataToSend == 1:
            cmd = CmdSetChannelRGB(self.selectedRingLight, self.selectedChannel, color.red(), color.green(), color.blue())
            self.sendCommand(cmd)
        else:
            cmd = CmdSetChannelW(self.selectedRingLight, self.selectedChannel, max(max(whiteColor.red(), whiteColor.green()), whiteColor.blue()))
            self.sendCommand(cmd)

    def sendDataSelectedRingLight(self):
        print("Sending to ringlight")
        for i in range(self.numChannels):
            color = self.colors[self.selectedRingLight][i]
            whiteColor = self.whiteColors[self.selectedRingLight][i]
            cmd = None
            if self.dataToSend == 0:
                cmd = CmdSetChannelW(self.selectedRingLight, i, max(max(whiteColor.red(), whiteColor.green()), whiteColor.blue()))
                #self.sendCommand(cmd)
                cmd = CmdSetChannelRGB(self.selectedRingLight, i, color.red(), color.green(), color.blue())
                #self.sendCommand(cmd)
            elif self.dataToSend == 1:
                cmd = CmdSetChannelRGB(self.selectedRingLight, i, color.red(), color.green(), color.blue())
                #self.sendCommand(cmd)
            else:
                cmd = CmdSetChannelW(self.selectedRingLight, i, max(max(whiteColor.red(), whiteColor.green()), whiteColor.blue()))
                #self.sendCommand(cmd)
            # time.sleep(.05)
        cmd = CmdSetFocusShutter(self.selectedRingLight, int(self.focus_enabled[self.selectedRingLight]), int(self.shutter_enabled[self.selectedRingLight]))
        self.sendCommand(cmd)

    def sendDataAll(self):
        print("Sending data to all ringlights: NOT IMPLEMENTED")

    #copies current settings onto all ring lights
    def copy_to_all(self, ringlight_id: int):
        color = self.colors[ringlight_id]
        whiteColor = self.whiteColors[ringlight_id]
        focus_enabled = self.focus_enabled[ringlight_id]
        shutter_enabled = self.shutter_enabled[ringlight_id]
        intensity = self.intensity[ringlight_id]
        for x in range(self.numRingLights):
            self.intensity[x] = intensity
            self.colors[x] = color
            self.focus_enabled[x] = focus_enabled
            self.shutter_enabled[x] = shutter_enabled
        
        self.notify()

    # copies current channel to all other channels
    def copy_to_all_chns(self, ringlight_id: int, channel: int):
        color = self.colors[ringlight_id][channel]
        whiteColor = self.whiteColors[ringlight_id][channel]
        for x in range(self.numChannels):
            self.colors[ringlight_id][x] = color
            self.whiteColors[ringlight_id][x] = whiteColor
            
    # resets the selected channel
    def reset_channel(self, ringlight_id: int, channel: int, notify = True):
        self.colors[ringlight_id][channel] = QColor(0, 0, 0)
        self.whiteColors[ringlight_id][channel] = QColor(0, 0, 0)
        if notify:
            self.notify()

    # resets current ringlight
    def reset_ring_light(self, ringlight_id: int):
        for i in range(self.numChannels):
            self.reset_channel(ringlight_id, i, notify=False)
        
        self.intensity[ringlight_id] = 100
        self.shutter_enabled[ringlight_id] = False
        self.focus_enabled[ringlight_id] = False
        self.notify()

    #resets all ringlights
    def reset(self):
        for i in range(self.numRingLights):
            self.reset_ring_light(i)


    def notify(self) -> None:
        """
        Trigger an update in each subscriber.
        """
        for observer in self.observers:
            observer.update(self)
    