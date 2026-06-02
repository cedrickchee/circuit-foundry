#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'USAGE'
Usage:
  arduino_cli_workflow.sh board-list
  arduino_cli_workflow.sh core-list
  arduino_cli_workflow.sh lib-list
  arduino_cli_workflow.sh compile <sketch-dir>
  arduino_cli_workflow.sh upload <sketch-dir>
  arduino_cli_workflow.sh compile-upload <sketch-dir>
  arduino_cli_workflow.sh monitor

Environment:
  FQBN        Board FQBN. Default: arduino:avr:uno
  PORT        Serial port for upload/monitor. Default: /dev/ttyACM0
  BAUD        Serial monitor baud rate. Default: 9600
  BUILD_ROOT  Build root. Default: /tmp/arduino-sketch-builds
  BUILD_PATH  Exact build path override.
USAGE
}

fqbn="${FQBN:-arduino:avr:uno}"
port="${PORT:-/dev/ttyACM0}"
baud="${BAUD:-9600}"
build_root="${BUILD_ROOT:-/tmp/arduino-sketch-builds}"

build_path_for() {
  local sketch_dir="$1"
  local sketch_name
  sketch_name="$(basename "$sketch_dir")"
  printf '%s\n' "${BUILD_PATH:-$build_root/$sketch_name}"
}

require_sketch() {
  if [ "$#" -ne 1 ]; then
    usage >&2
    exit 2
  fi

  if [ ! -d "$1" ]; then
    printf 'Sketch directory not found: %s\n' "$1" >&2
    exit 2
  fi
}

cmd="${1:-}"
if [ -z "$cmd" ]; then
  usage >&2
  exit 2
fi
shift || true

case "$cmd" in
  board-list)
    arduino-cli board list
    ;;
  core-list)
    arduino-cli core list
    ;;
  lib-list)
    arduino-cli lib list
    ;;
  compile)
    require_sketch "$@"
    build_path="$(build_path_for "$1")"
    mkdir -p "$build_path"
    arduino-cli compile --fqbn "$fqbn" --build-path "$build_path" "$1"
    printf 'BUILD_PATH=%s\n' "$build_path"
    ;;
  upload)
    require_sketch "$@"
    build_path="$(build_path_for "$1")"
    arduino-cli upload -p "$port" --fqbn "$fqbn" --input-dir "$build_path" "$1"
    ;;
  compile-upload)
    require_sketch "$@"
    build_path="$(build_path_for "$1")"
    mkdir -p "$build_path"
    arduino-cli compile --fqbn "$fqbn" --build-path "$build_path" "$1"
    arduino-cli upload -p "$port" --fqbn "$fqbn" --input-dir "$build_path" "$1"
    ;;
  monitor)
    arduino-cli monitor -p "$port" -b "$fqbn" -c "baudrate=$baud"
    ;;
  *)
    usage >&2
    exit 2
    ;;
esac
