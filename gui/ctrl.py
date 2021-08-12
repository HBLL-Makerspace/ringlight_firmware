from PyQt5 import QtWidgets, QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QWidget, QAction, QTabWidget,QVBoxLayout, QGroupBox, QLabel, QTextEdit, QLineEdit, QComboBox, QHBoxLayout, QSizePolicy, QStyleFactory, QPlainTextEdit, QSpinBox, QColorDialog, QFrame, QSpacerItem, QSlider, QCheckBox
from PyQt5.QtGui import QIcon, QColor
from PyQt5.QtCore import *
import sys
import serial
from serial.tools import list_ports
from app_state import AppState, TerminalSubject
from observer import Observer, Subject

class RingLightCtrlApp(QMainWindow):
    def __init__(self, state: AppState):
        super().__init__()
        self.setGeometry(400, 400, 640, 480)
        self.setWindowTitle("Ring Light Control")

        self.tabbed_widget = RingLightCntrlTabs(self, state)
        self.setCentralWidget(self.tabbed_widget)

        self.show()

class RingLightCntrlTabs(QWidget):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__(parent)
        self.layout = QVBoxLayout(self)
        self.tabs = QTabWidget()
        self.tab1 = SetupTab(self, state)
        self.tab2 = RingLightConfTab(self, state)
        self.tab3 = QWidget()
        self.tab4 = QWidget()
        # self.tabs.resize(300, 200)

        self.tabs.addTab(self.tab1, "Setup")
        self.tabs.addTab(self.tab2, "Configure Ring Lights")
        # self.tabs.addTab(self.tab3, "Multi Ctrl")
        self.tabs.addTab(self.tab4, "HDR picture")
        
        # Add tabs to widget
        self.layout.addWidget(self.tabs)
        self.setLayout(self.layout)

class RingLightConfTab(QWidget):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__()
        self.layout = QVBoxLayout(self)
        self.row = QHBoxLayout()
        self.selectrl = RLSelect(self, state)
        self.conf = RLConfigureGroup(self, state)

        self.sendButtonSingle = QPushButton("Send Data of Selected")
        self.sendButtonSingle.clicked.connect(state.sendDataSelected)

        self.sendButtonAll = QPushButton("Send Data All")
        self.sendButtonAll.clicked.connect(state.sendDataAll)

        self.layout.addWidget(self.selectrl)
        self.layout.addWidget(self.conf, stretch=1)
        self.layout.addWidget(self.sendButtonSingle)
        self.layout.addWidget(self.sendButtonAll)

        self.setLayout(self.layout)

class RLConfigureGroup(QGroupBox, Observer):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__("Configure Selected Ring Light")

        state.attach(self)
        self.state = state
        self.selectedrl = 0
        self.intensity_val = 0
        self.render_ui()

    def render_ui(self):
        self.layout = QHBoxLayout()

        self.frame = QFrame()
        self.frame.resize(10, 10)
        color = self.state.colors[self.state.selectedRingLight][self.state.selectedChannel]
        if color is None:
            color = QColor(0, 0, 0)
        self.frame.setStyleSheet("QWidget { background-color: %s }" % color.name())

        self.changeColorButton = QPushButton("Change Color", self)
        self.changeColorButton.clicked.connect(self.getColor)
        self.randomColorButton = QPushButton("Random Color", self)
        # self.randomColorButton.clicked.connect()

        self.colorLayout = QVBoxLayout()
        self.colorLayout.addWidget(self.frame)
        self.colorLayout.addWidget(self.changeColorButton)
        self.colorLayout.addWidget(self.randomColorButton)

        self.shutter_button = QPushButton("Enable Shutter", self)
        self.shutter_button.setCheckable(True)
        self.shutter_button.clicked.connect(self.toggle_shutter_btn)
        self.shutter_button.setStyleSheet("background-color: lightgrey")
        self.shutter_button.setChecked(False)
        self.focus_button = QPushButton("Enable focus", self)
        self.focus_button.setCheckable(True)
        self.focus_button.clicked.connect(self.toggle_focus_btn)
        self.focus_button.setStyleSheet("background-color: lightgrey")
        self.focus_button.setChecked(False)

        self.intensity = QSlider(Qt.Horizontal)
        self.intensity.setMinimum(0)
        self.intensity.setMaximum(100)
        self.intensity.setTickPosition(QSlider.TicksBelow)
        self.intensity.setTickInterval(1)
        self.intensity.valueChanged.connect(self.updateIntensity)

        self.intensity_label = QLabel("Intensity %s" % self.state.intensity)

        self.properties = QVBoxLayout()
        self.properties.addWidget(self.focus_button)
        self.properties.addWidget(self.shutter_button)
        # self.properties.addSpacerItem(QSpacerItem(150, 10, QSizePolicy.Expanding))
        self.properties.addStretch(1)
        self.properties.addWidget(self.intensity_label)
        self.properties.addWidget(self.intensity)
        self.properties.addStretch(1)

        self.useWhite = QCheckBox("Use white lights")
        self.useWhite.stateChanged.connect(self.toggle_use_white)
        self.properties.addWidget(self.useWhite)

        self.layout.addLayout(self.colorLayout)
        self.layout.addLayout(self.properties)
        self.setLayout(self.layout)

    def getColor(self):
        self.selectedColor = QColorDialog.getColor()
        if self.selectedColor.isValid():
            self.frame.setStyleSheet("QWidget { background-color: %s }" % self.selectedColor.name())
            self.state.updateRingLightColor(self.state.selectedRingLight, self.state.selectedChannel, self.selectedColor)

    def updateIntensity(self):
        self.state.updateIntensity(self.state.selectedRingLight, self.intensity.value())

    def toggle_focus_btn(self):
        self.state.updateFocus(self.state.selectedRingLight, self.focus_button.isChecked())
    
    def toggle_shutter_btn(self):
        self.state.updateShutter(self.state.selectedRingLight, self.shutter_button.isChecked())

    def toggle_use_white(self):
        self.state.updateUseWhite(self.state.selectedRingLight, self.state.selectedChannel, self.useWhite.isChecked())
        # print(self.state.useWhite)

    def update(self, state):
        # if self.selectedrl is not state.selectedRingLight:
        #     self.selectedrl = state.selectedRingLight
        selectedColor = self.state.colors[self.state.selectedRingLight][self.state.selectedChannel]
        if selectedColor is None:
            selectedColor = QColor(0, 0, 0)
        self.frame.setStyleSheet("QWidget { background-color: %s }" % selectedColor.name())
        
        if self.intensity_val != self.state.intensity[self.state.selectedRingLight]:
            self.intensity_val = self.state.intensity[self.state.selectedRingLight]
            self.intensity_label.setText("Intensity %s" % self.intensity_val)

            if self.intensity_val is not self.intensity.value():
                if self.intensity_val is not None:
                    self.intensity.setValue(self.intensity_val)
                else:
                    self.intensity_val = 0
                    self.intensity.setValue(self.intensity_val)
                    self.intensity_label.setText("Intensity %s" % self.intensity_val)

        if self.focus_button.isChecked() != self.state.focus_enabled[self.state.selectedRingLight]:
            self.focus_button.setChecked(self.state.focus_enabled[self.state.selectedRingLight])
        if self.focus_button.isChecked():
            self.focus_button.setStyleSheet("background-color: lightblue")
            self.focus_button.setText("Disable Focus")
            self.focus_button.setChecked(True)
        else:
            self.focus_button.setStyleSheet("background-color: lightgrey")
            self.focus_button.setText("Enable Focus")
            self.focus_button.setChecked(False)

        if self.shutter_button.isChecked() != self.state.shutter_enabled[self.state.selectedRingLight]:
            self.shutter_button.setChecked(self.state.shutter_enabled[self.state.selectedRingLight])
        if self.shutter_button.isChecked():
            self.shutter_button.setStyleSheet("background-color: lightblue")
            self.shutter_button.setText("Disable Shutter")
            self.shutter_button.setChecked(True)
        else:
            self.shutter_button.setStyleSheet("background-color: lightgrey")
            self.shutter_button.setText("Enable Shutter")
            self.shutter_button.setChecked(False)

        self.useWhite.setChecked(self.state.useWhite[self.state.selectedRingLight][self.state.selectedChannel])

class RLSelect(QGroupBox, Observer):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__("Select Ring Light")
        self.nmrl = 0
        self.state = state
        state.attach(self)
        self.layout = QVBoxLayout(self)

        self.ring_lights_cb = QComboBox()
        self.ring_lights_cb.currentIndexChanged.connect(self.updateSelectedRingLight)

        self.copy = QPushButton("Copy to all Ring Lights", self)
        self.copy.clicked.connect(self.copy_to_all)
        self.srl_row = QHBoxLayout()
        self.srl_row.addWidget(self.ring_lights_cb)
        self.srl_row.addWidget(self.copy)
        self.reset_rl = QPushButton("Reset Ring Light", self)
        self.reset_rl.clicked.connect(self.resetRingLight)
        self.srl_row.addWidget(self.reset_rl)
        self.layout.addLayout(self.srl_row)

        self.ring_lights_chn_cb = QComboBox()
        self.ring_lights_chn_cb.currentIndexChanged.connect(self.updateSelectedChannel)
        for x in range(self.state.numChannels):
            self.ring_lights_chn_cb.addItem(str(x))

        self.copy_chn = QPushButton("Copy to all channels", self)
        self.copy_chn.clicked.connect(self.copy_to_all_chns)
        self.schn_row = QHBoxLayout()
        self.schn_row.addWidget(self.ring_lights_chn_cb)
        self.schn_row.addWidget(self.copy_chn)
        self.reset_chn = QPushButton("Reset Channel", self)
        self.reset_chn.clicked.connect(self.resetChannel)
        self.schn_row.addWidget(self.reset_chn)
        self.layout.addLayout(self.schn_row)

        self.reset = QPushButton("Reset All", self)
        self.reset.clicked.connect(self.reset_all)
        self.layout.addWidget(self.reset)

        self.setLayout(self.layout)

    def reset_all(self):
        self.state.reset()

    def updateSelectedChannel(self, i):
        self.state.updateSelectedChannel(int(i))

    def resetChannel(self):
        self.state.reset_channel(self.state.selectedRingLight, self.state.selectedChannel)

    def resetRingLight(self):
        self.state.reset_ring_light(self.state.selectedRingLight)

    def copy_to_all_chns(self):
        self.state.copy_to_all_chns(self.state.selectedRingLight, self.state.selectedChannel)

    def copy_to_all(self):
        self.state.copy_to_all(self.state.selectedRingLight)

    def updateSelectedRingLight(self, i):
        self.state.updateSelectedRingLight(int(i))

    def update(self, subject: AppState):
        if self.nmrl is not subject.numRingLights:
            self.nmrl = subject.numRingLights
            self.ring_lights_cb.clear()
            for num in range(self.state.numRingLights):
                self.ring_lights_cb.addItem(str(num))


class SetupTab(QWidget):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__()
        self.layout = QVBoxLayout(self)
        self.group_ring_lights = ConfigureGroup(self, state)
        self.group_connection = ConnectionGroup(self, state)
        self.layout.addWidget(self.group_ring_lights)
        self.layout.addWidget(self.group_connection)
        self.setLayout(self.layout)

class ConfigureGroup(QGroupBox):
    def __init__(self, parent, state: AppState):
        super(QGroupBox, self).__init__("Ring Lights")
        self.state = state
        self.layout = QVBoxLayout(self)
        self.description = QLabel("Set how may ring lights are connected in series.")
        self.spinner = QSpinBox(self)
        self.setNumRingLigths = QPushButton("Set")
        self.setNumRingLigths.clicked.connect(self.setRingLightNumber)
        self.layout.addWidget(self.spinner)
        self.layout.addWidget(self.setNumRingLigths)

    def setRingLightNumber(self):
        self.state.updateNumRingLights(self.spinner.value())
        

class ConnectionGroup(QGroupBox, Observer):
    def __init__(self, parent, state: AppState):
        super(QGroupBox, self).__init__("Connection")
        self.render_ui()
        self.state = state;
        state.terminal.attach(self)

    def list_serial_ports(self):
        self.port.clear()
        for port in list_ports.comports():
            self.port.addItem(port.device)

    def render_ui(self):
        self.layout = QVBoxLayout(self)
        self.description = QLabel("Select port to connect to ring light controller.")
        self.layout.addWidget(self.description)
        row = QHBoxLayout()
        self.port = QComboBox()
        self.port.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Minimum)
        self.portLabel = QLabel("Port")
        self.portLabel.setBuddy(self.port) 
        self.refreshComs = QPushButton("Refresh")
        self.refreshComs.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        self.refreshComs.clicked.connect(self.list_serial_ports)
        self.baud = QComboBox()
        self.baud.addItem("1200")
        self.baud.addItem("2400")
        self.baud.addItem("4800")
        self.baud.addItem("9600")
        self.baud.addItem("19200")
        self.baud.addItem("38400")
        self.baud.addItem("57600")
        self.baud.addItem("115200")
        self.baud.setCurrentIndex(3)
        self.baud.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        row.addWidget(self.port)
        row.addWidget(self.baud)
        row.addWidget(self.refreshComs)
        self.layout.addLayout(row)

        row1 = QHBoxLayout()
        self.connect = QPushButton("Connect")
        self.connect.clicked.connect(self.connect_terminal)
        self.ping = QPushButton("Ping")
        row1.addWidget(self.connect)
        row1.addWidget(self.ping)
        self.layout.addLayout(row1)

        self.terminal = QPlainTextEdit(self)
        self.terminal.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.terminal.setReadOnly(True)
        self.layout.addWidget(self.terminal)

        # serial.
        for port in list_ports.comports():
            self.port.addItem(port.device)
        
        self.setLayout(self.layout)

    def update(self, subject: TerminalSubject) -> None:
        if subject._new_line is not None:
            self.terminal.insertPlainText(subject._new_line)
            self.terminal.moveCursor(QtGui.QTextCursor.End)
        if subject._isConnected is None:
            self.port.setDisabled(True)
            self.ping.setDisabled(True)
            self.baud.setDisabled(True)
            self.connect.setDisabled(True)
            self.refreshComs.setDisabled(True)
            self.connect.setText("Connecting...")
        elif subject._isConnected is False:
            self.ping.setDisabled(False)
            self.port.setDisabled(False)
            self.baud.setDisabled(False)
            self.connect.setDisabled(False)
            self.refreshComs.setDisabled(False)
            self.connect.setText("Connect")
            self.connect.clicked.disconnect()
            self.connect.clicked.connect(self.connect_terminal)
        else:
            self.ping.setDisabled(True)
            self.port.setDisabled(True)
            self.baud.setDisabled(True)
            self.connect.setDisabled(False)
            self.refreshComs.setDisabled(True)
            self.connect.setText("Disconnect")
            self.connect.clicked.disconnect()
            self.connect.clicked.connect(self.state.terminal.disconnect)

    def connect_terminal(self):
        self.state.terminal.connect(self.port.currentText(), baud=int(str(self.baud.currentText())))

    def __del__(self):
        self.state.terminal.detach(self)


if __name__ == "__main__":
    app  = QApplication(sys.argv)
    app.setWindowIcon(QIcon('gui/ring_light.png'))
    ex = RingLightCtrlApp(AppState())
    sys.exit(app.exec_())
