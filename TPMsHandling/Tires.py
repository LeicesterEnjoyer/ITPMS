import json
import os

from TPMsHandling.Tire import Tire


class Tires:
    def __init__(self, tiresSet: str, initialPressures: dict):
        self.__tiresSet = tiresSet.upper()
        self.__initialPressures = initialPressures
        
        self.__tires = {}
        self.__initTires()

    def __saveData(self, tiresConditionDict: dict):
        with open(self.__tiresSet + ".json", "w") as file:
            json.dump(tiresConditionDict, file)

    def __loadData(self):
        if os.path.exists(self.__tiresSet + ".json"):
            with open(self.__tiresSet + ".json", "r") as file:
                return json.load(file)
        return ""

    def __initTires(self):
        tireConditionDict = self.__loadData()
        
        if tireConditionDict == "":
            tireConditionDict = {"FL": 100, "FR": 100, "RR": 100, "RL": 100}
            self.__saveData(tireConditionDict)
        
        for position, condition in tireConditionDict.items():
            self.__tires[position] = Tire(position, self.__initialPressures[position], {"SUMMER": 5e4, "WINTER": 4e4, "ALL": 5.5e4}[self.__tiresSet], condition)

    def saveData(self):
        tireConditionDict = {}
        
        for position, tier in self.__tires.items():
            tireConditionDict[position] = tier.getTireCondition()
        
        self.__saveData(tireConditionDict)

    def reduceTiresCondition(self, avgDailyAmbientTemp: float):
        for tire in self.__tires.values():
            tire.reduceTireCondition(avgDailyAmbientTemp)

    def addValues(self, position: str, pressure: float, temp: float, distance: int):
        self.__tires[position].addDaily(pressure, temp, distance)

    def clearTiresData(self):
        for tire in self.__tires.values():
            tire.clearDailyData()

    def getTires(self):
        return self.__tires

    def __str__(self):
        tires = f'{self.__tiresSet} tires: '

        for tire in self.__tires.values():
            tires += str(tire)

        return tires
