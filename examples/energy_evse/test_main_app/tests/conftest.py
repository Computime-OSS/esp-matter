"""Pytest fixtures: configure CMake host build for energy_evse main/ sources."""

from __future__ import annotations

import os
import shutil
import subprocess
from pathlib import Path

import pytest


def _project_root() -> Path:
    return Path(__file__).resolve().parent.parent


def _host_unit_test_verbose_enabled() -> bool:
    return os.environ.get("CT_HOST_UNIT_TEST_VERBOSE", "").strip().lower() in (
        "1",
        "yes",
        "true",
        "on",
    )


@pytest.fixture(scope="session")
def host_unit_test_verbose() -> bool:
    """True when CT_HOST_UNIT_TEST_VERBOSE is set (enables C++ + pytest success logs)."""
    return _host_unit_test_verbose_enabled()


@pytest.fixture(scope="session")
def host_unit_test_binary(host_unit_test_verbose: bool) -> Path:
    """Build host_unit_test_runner once; return path to the executable."""
    root = _project_root()
    build_dir = root / "build_host_pytest"
    build_dir.mkdir(parents=True, exist_ok=True)
    cmake = shutil.which("cmake")
    if cmake is None:
        pytest.skip("cmake not on PATH")
    verbose_flag = "ON" if host_unit_test_verbose else "OFF"
    subprocess.run(
        [
            cmake,
            "-S",
            str(root),
            "-B",
            str(build_dir),
            "-DCMAKE_BUILD_TYPE=Debug",
            f"-DCT_HOST_UNIT_TEST_VERBOSE={verbose_flag}",
        ],
        check=True,
    )
    subprocess.run(
        [cmake, "--build", str(build_dir), "--target", "host_unit_test_runner"],
        check=True,
    )
    exe = build_dir / "host_unit_test_runner"
    if not exe.is_file() and os.name == "nt":
        exe = build_dir / "Debug" / "host_unit_test_runner.exe"
    if not exe.is_file():
        pytest.fail(f"built binary missing: {exe}")
    return exe
