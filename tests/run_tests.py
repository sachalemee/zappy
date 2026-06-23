##
## EPITECH PROJECT, 2026
## zappy
## File description:
## run_tests
##

import os
import sys
import unittest

# let this run both as `python3 tests/run_tests.py` and `python3 -m tests.run_tests`
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from tests.colors import ColorTextTestRunner

if __name__ == "__main__":
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    suite = unittest.TestLoader().discover(os.path.join(repo_root, "tests"), pattern="test_*.py", top_level_dir=repo_root)
    result = ColorTextTestRunner(verbosity=2).run(suite)
    sys.exit(0 if result.wasSuccessful() else 1)
