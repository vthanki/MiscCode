#!/usr/bin/python

import gtk, sys
import socket

class PyApp(gtk.Window):
    def __init__(self):
        super(PyApp, self).__init__()

        self.connect("destroy", gtk.main_quit)
        self.set_size_request(250,150);
        self.set_position(gtk.WIN_POS_CENTER)
        self.set_title("Factory Test");

        nextButton = gtk.Button("send");
        nextButton.connect("clicked", self.sendOnClicked);

        self.cmd = gtk.Entry(50)

        self.img = gtk.Image()

        try:
            self.cpuimg = gtk.gdk.pixbuf_new_from_file("cpu.png")
            self.success = gtk.gdk.pixbuf_new_from_file("success.png")
            self.failure = gtk.gdk.pixbuf_new_from_file("failure.png")
        except Exception, e:
            sys.exit(1)

        self.img.set_from_pixbuf(self.cpuimg);

        fixed = gtk.Fixed();
        fixed.put(self.cmd, 20, 10);
        fixed.put(nextButton, 20, 50);
        fixed.put(self.img, 20, 100);

        self.add(fixed);
        self.show_all()

    def sendOnClicked(self, widget):
        print "Button clicked ", self.cmd.get_text()
        self.s.send(self.cmd.get_text())
        if (int(self.s.recv(1024)) == 0):
            self.img.set_from_pixbuf(self.success);
            self.show_all();
        else:
            self.img.set_from_pixbuf(self.failure);
            self.show_all();
        self.cmd.set_text("")

    def setupConnection(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
        self.s.connect((socket.gethostname(), 8153));
        print "connection established"


PyApp()
PyApp().setupConnection()
gtk.main()
