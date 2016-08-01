#!/usr/bin/python

import dbus

bus = dbus.SystemBus()
hci0_obj = bus.get_object('org.bluez', '/org/bluez/hci0')
hci0_properties = dbus.Interface(hci0_obj, 'org.freedesktop.DBus.Properties')
print "Address:", hci0_properties.Get('org.bluez.Adapter1', 'Address')
print "Name:", hci0_properties.Get('org.bluez.Adapter1', 'Name')
