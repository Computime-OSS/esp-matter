"""Pytest fixtures: configure CMake host build for energy_evse main/ sources."""

from __future__ import annotations

import os
import shutil
import subprocess
import warnings
from pathlib import Path

import pytest


def pytest_addoption(parser) -> None:
    parser.addoption(
        "--host-cpp-cov-xml",
        action="store",
        metavar="FILE",
        default=None,
        dest="host_cpp_cov_xml",
        help=(
            "After the session: rebuild host_unit_test_runner with gcov, run suite 'all', "
            "and write Cobertura XML via gcovr (install gcovr; separate from pytest-cov Python XML)."
        ),
    )


def pytest_sessionfinish(session, exitstatus: int) -> None:
    xml_arg = session.config.getoption("host_cpp_cov_xml")
    if not xml_arg:
        return
    gcovr = shutil.which("gcovr")
    if gcovr is None:
        warnings.warn(
            "gcovr not found on PATH; install test_main_app/requirements.txt for --host-cpp-cov-xml",
            stacklevel=1,
        )
        return

    root = _project_root()
    build_dir = root / "build_host_pytest_cov"
    cmake = shutil.which("cmake")
    if cmake is None:
        warnings.warn("cmake not on PATH; cannot emit --host-cpp-cov-xml", stacklevel=1)
        return

    verbose_flag = "ON" if _host_unit_test_verbose_enabled() else "OFF"
    subprocess.run(
        [
            cmake,
            "-S",
            str(root),
            "-B",
            str(build_dir),
            "-DCMAKE_BUILD_TYPE=Debug",
            "-DCT_HOST_COVERAGE=ON",
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
    env = os.environ.copy()
    env["TZ"] = "UTC"
    subprocess.run([str(exe), "all"], check=True, env=env)

    xml_path = Path(xml_arg)
    if not xml_path.is_absolute():
        xml_path = Path(session.config.rootpath) / xml_path
    xml_path.parent.mkdir(parents=True, exist_ok=True)

    subprocess.run(
        [
            gcovr,
            str(build_dir),
            f"--root={root}",
            "--xml-pretty",
            "-o",
            str(xml_path),
        ],
        check=True,
    )


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
