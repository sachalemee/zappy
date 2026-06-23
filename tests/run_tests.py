##
## EPITECH PROJECT, 2026
## zappy
## File description:
## run_tests
##

import sys
import unittest

from tests.colors import ColorTextTestRunner

if __name__ == "__main__":
    suite = unittest.TestLoader().discover("tests", pattern="test_*.py")
    result = ColorTextTestRunner(verbosity=2).run(suite)
    sys.exit(0 if result.wasSuccessful() else 1)
