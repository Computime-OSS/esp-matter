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

**Coverage (gcov / HTML) vs. ``../main``**

- Run ``scripts/run_host_coverage.sh`` from ``test_main_app/``. It builds with ``-DCT_HOST_COVERAGE=ON``,
  runs ``host_unit_test_runner all``, then optional ``lcov``/``genhtml``.
- Reports line coverage only for translation units **linked into that host binary** — today that is
  ``../main/driver/ev_charger/get_readable_time.cpp``, ``matter_schedule_allow.cpp``, and the
  harness under ``test_main_app/src/``. Other ``main/`` files (Matter, ESP-IDF, etc.) are **not**
  in this target, so they do not appear until you add them to ``CMakeLists.txt`` and execute them
  from the runner.
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
