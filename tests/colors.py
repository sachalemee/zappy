##
## EPITECH PROJECT, 2026
## zappy
## File description:
## colors
##

import unittest


class _Colors:
    GREEN = "\033[92m"
    RED = "\033[91m"
    YELLOW = "\033[93m"
    RESET = "\033[0m"


class ColorTextTestResult(unittest.TextTestResult):
    def _write_status(self, test, status):
        color = {"ok": _Colors.GREEN, "ERROR": _Colors.RED, "FAIL": _Colors.RED}.get(
            status.split()[0], _Colors.YELLOW if status.startswith("skipped") else ""
        )
        colored = f"{color}{status}{_Colors.RESET}" if color else status
        super()._write_status(test, colored)

    def addSuccess(self, test):
        unittest.TestResult.addSuccess(self, test)
        if self.showAll:
            self._write_status(test, "ok")
        elif self.dots:
            self.stream.write(f"{_Colors.GREEN}.{_Colors.RESET}")
            self.stream.flush()

    def addError(self, test, err):
        unittest.TestResult.addError(self, test, err)
        if self.showAll:
            self._write_status(test, "ERROR")
        elif self.dots:
            self.stream.write(f"{_Colors.RED}E{_Colors.RESET}")
            self.stream.flush()

    def addFailure(self, test, err):
        unittest.TestResult.addFailure(self, test, err)
        if self.showAll:
            self._write_status(test, "FAIL")
        elif self.dots:
            self.stream.write(f"{_Colors.RED}F{_Colors.RESET}")
            self.stream.flush()

    def addSkip(self, test, reason):
        unittest.TestResult.addSkip(self, test, reason)
        if self.showAll:
            self._write_status(test, f"skipped {reason!r}")
        elif self.dots:
            self.stream.write(f"{_Colors.YELLOW}s{_Colors.RESET}")
            self.stream.flush()


class ColorTextTestRunner(unittest.TextTestRunner):
    resultclass = ColorTextTestResult
