import unittest

from CSVhandling.CSVmanager import CSVmanager
from CSVhandling.DataGenerator import DataGenerator


if __name__ == "__main__":
    loader = unittest.TestLoader()
    suite = loader.discover('Tests')

    runner = unittest.TextTestRunner()
    runner.run(suite)

    csv = CSVmanager("data/SummerTiresData.csv")
    dataGenerator = DataGenerator("SUMMER", 2)

    csv.writeToCSV(dataGenerator.generateData())
