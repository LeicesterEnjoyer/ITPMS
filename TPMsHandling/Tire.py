import numpy as np


class Tire:
    def __init__(self, position: str, initialPressure: float, tireLife: int, condition: float = 100): 
        self.__position = position
        self.__initialPressure = initialPressure
        self.__tireLife = tireLife
        self.__tireCondition = condition

        self.clearDailyData()

    def clearDailyData(self):
        self.__dailyTemp = []
        self.__dailyPressure = []
        self.__dailyDistance = 0

        self.__dailyPressure.append(self.__initialPressure)

    def reduceTireCondition(self, avgDailyAmbientTemp: float):
        coef = .0

        coef = self.__dailyDistance / self.__tireLife * 100
        coef *= (1.1 + 0.005 * (self.__calculateAVG(self.__dailyTemp) - avgDailyAmbientTemp) / 5)
        
        dPressure = (self.__calculateAVG(self.__dailyPressure) - self.__initialPressure) / self.__initialPressure * 100
        coef *= (1.1 + 0.01 * dPressure / 10)

        self.__tireCondition -= coef

    def __calculateRMS(self, data):
        return np.sqrt(np.mean(np.square(data)))

    def __calculateAVG(self, data):
        return len(data) > 0 and float(sum(data) / len(data))

    def addDailyTemp(self, temp: float):
        self.__dailyTemp.append(temp)

    def addDailyPressure(self, pressure: float):
        self.__dailyPressure.append(pressure)

    def setDailyDistance(self, distance: int):
        if distance > self.__dailyDistance:
            self.__dailyDistance = distance

    def addDaily(self, pressure: float, temp: float, distance: int):
        self.addDailyPressure(pressure)
        self.addDailyTemp(temp)
        self.setDailyDistance(distance)

    def getTireCondition(self):
        return self.__tireCondition

    def __str__(self):
        return f'\n({self.__position}) - Condition: {self.__tireCondition}%. Traveled distance: {self.__dailyDistance} km. Average tire pressure: {self.__calculateAVG(self.__dailyPressure)} bars. Average tire temperature: {self.__calculateAVG(self.__dailyTemp)} °C.'
