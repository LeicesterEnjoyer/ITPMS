import csv


class CSVmanager:
    def __init__(self, path : str):
        self.__path = path

    def readFromCSV(self):
        with open(self.__path, mode='r') as file:
            return list(csv.reader(file))

    def writeToCSV(self, data):
        with open(self.__path, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerows(data)
