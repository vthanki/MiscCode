#!/usr/bin/python

import gtk, sys
import socket
import pango

class PyApp(gtk.Window):
    def __init__(self):
        super(PyApp, self).__init__()

        self.connect("destroy", gtk.main_quit)
#        self.set_size_request(250,150);
        self.set_position(gtk.WIN_POS_CENTER)
        self.set_title("Factory Test");

        mainBox = gtk.HBox(True, 0)
        headerBox = gtk.HBox(True, 0);
        testInfoBox = gtk.HBox(True, 0);
        footerBox = gtk.HBox(True, 0);

        # Setup Header Box
        hwTypeLabel = gtk.Label()
        hwTypeLabel.modify_font(pango.FontDescription('sans bold 26'))
        hwTypeLabel.set_text("Raumfeld Connector")

        headerSeparator = gtk.VSeparator()

        fwVersionLabel = gtk.Label()
        fwVersionLabel.modify_font(pango.FontDescription('sans bold 20'))
        fwVersionLabel.set_text("1.55")

        headerBox.pack_start(hwTypeLabel, True, True, 0)
        headerBox.pack_start(headerSeparator, True, True, 0)
        headerBox.pack_end(fwVersionLabel, True, True, 0)
        mainBox.pack_start(headerBox, True, True, 0);

        # Setup the Test Info box */



      #  button1 = gtk.Button("OK")
      #  button2 = gtk.Button("Cancel")
      #  mainBox.pack_start(button1, True, True, 0);
      #  mainBox.pack_start(button2, True, True, 0);
        self.add(mainBox)
        self.show_all()

PyApp()
gtk.main()
