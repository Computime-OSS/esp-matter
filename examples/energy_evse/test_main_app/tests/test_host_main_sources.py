"""Run native unit tests that link production sources from ../main (no firmware flash).

Each **pytest** function runs one **suite** of the host binary so ``pytest -v`` shows one line per
suite (e.g. ``test_native_get_readable_time ... PASSED``), not a single lumped ``[100%]`` for
everything.

The host executable accepts a suite name::

    ./host_unit_test_runner [get_readable_time|matter_schedule|all]

**Adding another suite**

1. Add ``run_your_suite_tests()`` in ``src/host_unit_test_runner.cpp`` and a branch in ``main()``.
2. Add a new ``def test_native_...`` below that calls ``_run_host_native_suite(..., suite=\"your_suite\")``.

Verbose logs: set ``CT_HOST_UNIT_TEST_VERBOSE=1`` and use ``pytest -rP`` or ``-s`` as before.

**Coverage from pytest**

- **Python** (``tests/*.py``): use **pytest-cov**, e.g. ``--cov=tests --cov-report=xml:coverage-python.xml``.
- **C++** linked from ``../main``: ``pytest-cov`` does not instrument C++. Use **``--host-cpp-cov-xml=FILE``**
  (requires **gcovr** on PATH). After the session, a separate gcov build runs and writes **Cobertura** XML
  (same family of report many CI tools consume as ``coverage.xml``).
  Only ``.cpp`` files listed in ``test_main_app/CMakeLists.txt`` under ``../main/`` appear there until you add more.

Example (JUnit + both XML reports)::

    pytest tests \\
      --junitxml=pytest-report.xml \\
      --cov=tests --cov-report=xml:coverage-python.xml \\
      --host-cpp-cov-xml=coverage-cpp.xml

Shell-only C++ HTML remains: ``scripts/run_host_coverage.sh``.
"""

from __future__ import annotations

import os
import subprocess
from pathlib import Path


def _run_host_native_suite(binary: Path, *, verbose: bool, suite: str) -> None:
    env = os.environ.copy()
    env["TZ"] = "UTC"
    cmd = [str(binary), suite]
    if verbose:
        result = subprocess.run(cmd, check=True, env=env, capture_output=True, text=True)
        if result.stdout:
            print(result.stdout, end="", flush=True)
        if result.stderr:
            print(result.stderr, end="", flush=True)
        print(f"OK: suite={suite!r} binary={binary}", flush=True)
    else:
        subprocess.run(cmd, check=True, env=env)


def test_native_get_readable_time(host_unit_test_binary: Path, host_unit_test_verbose: bool) -> None:
    """Host checks for ``../main/.../get_readable_time.cpp`` (via ``GetReadableTime``)."""
    _run_host_native_suite(host_unit_test_binary, verbose=host_unit_test_verbose, suite="get_readable_time")


def test_native_matter_schedule_allow(host_unit_test_binary: Path, host_unit_test_verbose: bool) -> None:
    """Host checks for ``../main/.../matter_schedule_allow.cpp`` (MatterManager schedule gate)."""
    _run_host_native_suite(host_unit_test_binary, verbose=host_unit_test_verbose, suite="matter_schedule")
