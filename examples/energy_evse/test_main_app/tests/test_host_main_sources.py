"""Run native unit tests that link production sources from ../main (no firmware flash).

Verbose success logs (runner + ``../main`` ``GetReadableTime``) are **off** by default.

Enable with env ``CT_HOST_UNIT_TEST_VERBOSE=1`` (or ``yes`` / ``true`` / ``on``), then e.g.:

.. code-block:: bash

   CT_HOST_UNIT_TEST_VERBOSE=1 pytest tests -v -rP

``-rP`` shows captured stdout for passed tests; ``-s`` prints logs live without capture.
"""

from __future__ import annotations

import os
import subprocess
from pathlib import Path


def test_host_unit_runner_exercises_main_sources(
    host_unit_test_binary: Path, host_unit_test_verbose: bool
) -> None:
    env = os.environ.copy()
    env["TZ"] = "UTC"
    if host_unit_test_verbose:
        result = subprocess.run(
            [str(host_unit_test_binary)],
            check=True,
            env=env,
            capture_output=True,
            text=True,
        )
        if result.stdout:
            print(result.stdout, end="", flush=True)
        if result.stderr:
            print(result.stderr, end="", flush=True)
        print(f"OK: pytest ran host binary {host_unit_test_binary}", flush=True)
    else:
        subprocess.run([str(host_unit_test_binary)], check=True, env=env)
