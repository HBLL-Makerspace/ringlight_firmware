from PyQt5 import QtWidgets, QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QWidget, QAction, QTabWidget,QVBoxLayout, QGroupBox, QLabel, QTextEdit, QLineEdit, QComboBox, QHBoxLayout, QSizePolicy, QStyleFactory, QPlainTextEdit, QSpinBox
from PyQt5.QtGui import QIcon
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
        self.tab1 = ConfigureTab(self, state)
        self.tab2 = SingleRLTab(self, state)
        self.tab3 = QWidget()
        self.tab4 = QWidget()
        # self.tabs.resize(300, 200)

        self.tabs.addTab(self.tab1, "Configure")
        self.tabs.addTab(self.tab2, "Single")
        self.tabs.addTab(self.tab3, "Multi Ctrl")
        self.tabs.addTab(self.tab4, "HDR picture")
        
        # Add tabs to widget
        self.layout.addWidget(self.tabs)
        self.setLayout(self.layout)

class SingleRLTab(QWidget):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__()
        self.layout = QVBoxLayout(self)
        self.selectrl = SingleRLTabSelect(self, state)
        self.layout.addWidget(self.selectrl)
        self.setLayout(self.layout)

class SingleRLTabSelect(QGroupBox):
    def __init__(self, parent, state: AppState):
        super(QWidget, self).__init__("Select Ring Light")
        self.layout = QVBoxLayout(self)
        self.ring_lights_cb = QComboBox(self)
        self.layout.addWidget(self.ring_lights_cb)
        self.setLayout(self.layout)

class ConfigureTab(QWidget):
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
        self.state.numRingLights = self.spinner.value()
        print(self.state.numRingLights)
        

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
    app.setWindowIcon(QIcon('ring_light.png'))
    ex = RingLightCtrlApp(AppState())
    sys.exit(app.exec_())
