import unittest


if __name__ == "__main__":
    loader = unittest.TestLoader()
    suite = loader.discover('Tests')

    runner = unittest.TextTestRunner()
    runner.run(suite)
