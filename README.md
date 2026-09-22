# STM32 CMake template

A CMake-only starting point for the STM32L476RG / NUCLEO-L476RG board. It uses
only the fixed board defaults (including SWD); STM32CubeMX is not part of the
workflow.

## New project

Copy this directory, excluding `.git` and `build`, and rename the copy. The
CMake executable name is derived from the directory name, so no project-name
files need editing.

```sh
rsync -a --exclude .git --exclude build stm32-cmake-template/ A2_blinky/
git -C A2_blinky init
```

## Build

```sh
cmake --preset Debug
cmake --build --preset Debug
```

The application entry point is `Core/Src/main.c`. Add peripheral initialization
and any necessary HAL driver sources in `cmake/platform/CMakeLists.txt`.

## Constraints

The startup file, linker script, compile definitions, and platform source list
are specific to the STM32L476RG. Changing MCU, board, clock setup, or pin
assignment is a source/CMake change; there is intentionally no `.ioc` file.
