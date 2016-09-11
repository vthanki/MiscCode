#!/usr/bin/python

class Square:
    num = 0

    def __init__(self, initial):
        self.num = initial

    def doSquare(self):
        self.num = self.num ** 2

    def returnSquare(self):
        self.doSquare()
        return self.num


mynum = Square(12)
print mynum.returnSquare()

