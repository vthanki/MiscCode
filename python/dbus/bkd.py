#!/usr/bin/python

from pydbus import SessionBus

session_bus = SessionBus()
bkd_obj = session_bus.get('org.geekpur.vishal.BKD',
        '/org/geekpur/vishal/BKD')
print bkd_obj.GetBackspaceCount()
