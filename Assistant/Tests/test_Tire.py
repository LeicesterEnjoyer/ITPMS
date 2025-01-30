import unittest
from TPMsHandling.Tire import Tire


class TestTire(unittest.TestCase):
    def setUp(self):
        self.__tire = Tire("FL", 2.2, 50000)

    def test_initialization(self):
        self.assertEqual(self.__tire.getTireCondition(), 100)
        self.assertEqual(str(self.__tire), "\n(FL) - Condition: 100%. Traveled distance: 0 km. Average tire pressure: 2.2 bars. Average tire temperature: 0.0 °C.")

    def test_clearDailyData(self):
        self.__tire.addDaily(2, 35, 15)
        self.__tire.clearDailyData()
        
        self.assertEqual(str(self.__tire), "\n(FL) - Condition: 100%. Traveled distance: 0 km. Average tire pressure: 2.2 bars. Average tire temperature: 0.0 °C.")

    def test_addDaily(self):
        self.__tire.addDaily(1.8, 30, 100)
        
        self.assertEqual(str(self.__tire), "\n(FL) - Condition: 100%. Traveled distance: 100 km. Average tire pressure: 2.0 bars. Average tire temperature: 30.0 °C.")

    def test_reduceTireCondition(self):
        self.__tire.addDaily(2.8, 30, 40)
        self.__tire.addDaily(2.3, 33, 50)

        initialCondition = self.__tire.getTireCondition()
        self.__tire.reduceTireCondition(25)
        
        self.assertLess(self.__tire.getTireCondition(), initialCondition)

    def test_calculateAvgAndRMS(self):
        avgTemp = self.__tire._Tire__calculateAVG([39.0, 41.5, 42.5, 43.0, 41.0, 45.0])
        rmsTemp = self.__tire._Tire__calculateRMS([40.0, 43.0, 42.0, 44.0, 41.0, 42.0])

        self.assertEqual(avgTemp, 42)
        self.assertAlmostEqual(rmsTemp, 42, places=1)

    def test_tireConditionWithoutData(self):
        initialCondition = self.__tire.getTireCondition()
        self.__tire.reduceTireCondition(0)
        
        self.assertEqual(self.__tire.getTireCondition(), initialCondition)

    def test_addDailyDistance(self):
        self.__tire.setDailyDistance(50)
        self.__tire.setDailyDistance(40) # Should not decrease.
        
        self.assertEqual(self.__tire._Tire__dailyDistance, 50)
