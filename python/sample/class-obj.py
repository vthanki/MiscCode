#!/usr/bin/python

class Car:

    def __init__(self, typeOfCar, engineType="petrol"):
        self.carType = typeOfCar
        self.engineType = engineType

    def describeCar(self):
        print self.carType
        print self.engineType

maruti = Car("hatch back", "diesel")
maruti.describeCar()

