"""***************************************************************************************************************
interface.py
-----------------
This class creates an interface
***************************************************************************************************************"""

"""++++++++++++++++++++++++++++++
            Imports
++++++++++++++++++++++++++++++"""
import pandas as pd

from PyQt5 import Qt
from PyQt5 import QtWidgets, QtCore
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg

import sys  # We need sys so that we can pass argv to QApplication
import time
import numpy as np

"""++++++++++++++++++++++++++++++
           Constants
++++++++++++++++++++++++++++++"""




"""++++++++++++++++++++++++++++++
            Classes
++++++++++++++++++++++++++++++"""
class MainWindow(QtWidgets.QMainWindow):
    """++++++++++++++++++++++++++++++
               Constants
    ++++++++++++++++++++++++++++++"""

    """++++++++++++++++++++++++++++++
               Functions
    ++++++++++++++++++++++++++++++"""

    '''*********************************************************************
    *Name: __init__
    *Funcition: Initialize the class. Creates the first interface
    *Parameters: self:
    *            args:
    *            kwargs:
    *Return: -
    *********************************************************************'''
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        self.setGeometry(300, 300, 600, 600)
        # Load the .ui file
        uic.loadUi('./Interface/ip_port_interface.ui', self)

        self.bt_ok.clicked.connect(self.bt_ok_pressed)
        self.bt_close.clicked.connect(self.close)

    def bt_ok_pressed(self):
        self.ip = self.txt_ip.toPlainText()
        self.port = self.txt_port.toPlainText()
        self.receiver(self.ip, self.port)

    def set_receiver(self, receiver):
        self.receiver = receiver


"""++++++++++++++++++++++++++++++
           Functions
++++++++++++++++++++++++++++++"""
def create_interface(receiver):
    app = QtWidgets.QApplication(sys.argv)

    window = MainWindow()
    window.set_receiver(receiver)

    sys.exit(app.exec_())
