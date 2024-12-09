import random


class DataGenerator:
    def __init__(self, season: str, avgDailyDrive: int):
        self.__season = "ALL"
        if season.upper() in ["SUMMER", "WINTER"]:
            self.__season = season.upper()

        self.__avgDailyDrive = avgDailyDrive
        
    def generateData(self):
        data = [['Day', 'Tire', 'Pressure', 'TireTemp', 'DailyDistance', 'AmbientTemp']]
        
        daysToGenerate = {"SUMMER": 228, "WINTER": 137, "ALL": 365}[self.__season]
        ambientTempRange = {"SUMMER": [24, 34], "WINTER": [-4, 10], "ALL": [-4, 34]}[self.__season]
        tireTempRange = {"SUMMER": [30, 40], "WINTER": [1, 15], "ALL": [1, 40]}[self.__season]

        for day in range(1, daysToGenerate + 1):
            dailyDistance = random.randint(7, 45)

            for i in range(self.__avgDailyDrive * 2):
                ambientTemp = random.uniform(ambientTempRange[0], ambientTempRange[1])
                tireTemp = random.uniform(tireTempRange[0], tireTempRange[1])
            
                for tire in ["FL", "FR", "RR", "RL"]:
                    data.append([str(day) + '.' + str(i + 1), tire, random.uniform(2.17, 2.23), tireTemp, dailyDistance, ambientTemp])
        
        return data
