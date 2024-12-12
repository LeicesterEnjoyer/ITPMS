import unittest

from CSVhandling.CSVmanager import CSVmanager
from CSVhandling.DataGenerator import DataGenerator


if __name__ == "__main__":
    loader = unittest.TestLoader()
    suite = loader.discover('Tests')

    runner = unittest.TextTestRunner()
    runner.run(suite)

    csv = CSVmanager("Data/WINTER.csv")
    dataGenerator = DataGenerator("WINTER", 2, {"FL": 2.2, "FR": 2.2, "RR": 2.2, "RL": 2.2})

    csv.writeToCSV(dataGenerator.generateData())
