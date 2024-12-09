import unittest
import os

from TPMsHandling.Tires import Tires
from TPMsHandling.Tire import Tire


class TestTires(unittest.TestCase):
    def setUp(self):
        self.__tiresSet = "SUMMER"
        self.__initial_pressures = {"FL": 2.2, "FR": 2.2, "RR": 2.4, "RL": 2.4}
        
        if os.path.exists(self.__tiresSet.upper() + ".json"):
            os.remove(self.__tiresSet.upper() + ".json")

        self.__tires = Tires(self.__tiresSet, self.__initial_pressures)

    def tearDown(self):
        if os.path.exists(self.__tiresSet.upper() + ".json"):
            os.remove(self.__tiresSet.upper() + ".json")

    def test_initialization(self):
        self.assertEqual(len(self.__tires.getTires()), 4)

        for tire in self.__tires.getTires().values():
            self.assertIsInstance(tire, Tire)
            self.assertEqual(tire.getTireCondition(), 100.0)

    def test_saveAndLoaddata(self):
        self.__tires.addValues("FL", 2.0, 25.5, 15)
        self.__tires.reduceTiresCondition(30)
        self.__tires.saveData()

        loadedTires = Tires(self.__tiresSet, self.__initial_pressures)
        
        self.assertNotEqual(loadedTires.getTires()["FL"].getTireCondition(), 100.0)
        for position in ["FR", "RR", "RL"]:
            self.assertEqual(loadedTires.getTires()[position].getTireCondition(), 100.0)

    def test_reduceTiresCondition(self):
        initialConditions = {position: tire.getTireCondition() for position, tire in self.__tires.getTires().items()}

        for position in ["FL", "FR", "RR", "RL"]:
            self.__tires.addValues(position, 2.1, 27, 20)
        self.__tires.reduceTiresCondition(30)

        reducedConditions = {position: tire.getTireCondition() for position, tire in self.__tires.getTires().items()}

        for position in initialConditions:
            self.assertLess(reducedConditions[position], initialConditions[position])

    def test_addValues(self):
        self.__tires.addValues("FL", 1.8, 40, 100)

        self.assertEqual(str(self.__tires.getTires()["FL"]), "\n(FL) - Condition: 100%. Traveled distance: 100 km. Average tire pressure: 2.0 bars. Average tire temperature: 40.0 \u00b0C.")

    def test_clearTiresData(self):
        self.__tires.addValues("FL", 2.0, 35, 50)
        self.__tires.clearTiresData()

        self.assertEqual(str(self.__tires.getTires()["FL"]), "\n(FL) - Condition: 100%. Traveled distance: 0 km. Average tire pressure: 2.2 bars. Average tire temperature: 0.0 \u00b0C.")

    def test_strRepresentation(self):
        representation = str(self.__tires)

        self.assertTrue(self.__tiresSet.upper() in representation)

        for position in ["FL", "FR", "RR", "RL"]:
            self.assertTrue(position in representation)
