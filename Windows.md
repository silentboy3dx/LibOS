# Windows Build Tutorial (CLion + Visual Studio + vcpkg) — LibOS

Deze handleiding legt uit hoe je LibOS correct bouwt op Windows met:

- CLion
- Visual Studio 2022 Community
- MSVC v143
- Windows SDK (automatisch geïnstalleerd)
- vcpkg (x64-windows)
- Visual Studio generator (GEEN Ninja)

Dit is de enige configuratie die 100% stabiel werkt voor Windows-native OS functionaliteit.

------------------------------------------------------------
1. Installeer Visual Studio 2022 Community
------------------------------------------------------------

Open de Visual Studio Installer en vink slechts één workload aan:

    ✔ Desktop development with C++

Dit installeert automatisch:

- MSVC v143 compiler
- Windows SDK (10 of 11)
- CMake tools
- NMake Makefiles
- Visual Studio generator
- Alle benodigde headers en libs

Je hoeft geen individuele SDK’s meer te selecteren.

------------------------------------------------------------
2. Installeer vcpkg
------------------------------------------------------------

    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    bootstrap-vcpkg.bat

Installeer dependency:

    vcpkg install nlohmann-json:x64-windows

------------------------------------------------------------
3. Configureer CLion Toolchain
------------------------------------------------------------

Ga naar:

    File → Settings → Build, Execution, Deployment → Toolchains

Selecteer:

- Toolchain: Visual Studio
- C Compiler: cl.exe
- C++ Compiler: cl.exe
- Debugger: bundled
- Environment: leeg laten

CLion detecteert automatisch:

- Windows SDK
- MSVC toolchain

⚠ BELANGRIJK:
CLion gebruikt standaard Ninja, maar Ninja veroorzaakt op Windows timestamp-bugs.
Daarom gebruiken we de Visual Studio generator.

------------------------------------------------------------
4. Configureer CMake in CLion
------------------------------------------------------------

Ga naar:

    Settings → Build, Execution, Deployment → CMake

Stel in:

- Generator: Visual Studio 17 2022
- Toolchain: Visual Studio
- CMake options:

      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

------------------------------------------------------------
5. Verwijder oude build-mappen
------------------------------------------------------------

Verwijder:

    cmake-build-debug/
    cmake-build-release/
    cmake-build-debug-visual-studio/
    build/

------------------------------------------------------------
6. Builden
------------------------------------------------------------

Klik:

- Reload CMake Project
- Build

Je krijgt:

- ✔ nlohmann_json gevonden
- ✔ MSVC toolchain gevonden
- ✔ Windows SDK gevonden
- ✔ LibOS.dll gebouwd

------------------------------------------------------------
7. Install (Als Administrator)
------------------------------------------------------------

Ga naar de CLion build-map:

    cd LibOS/cmake-build-debug-visual-studio

Voer uit:

    cmake --install . --prefix "C:/Program Files/LibOS"

Dit installeert:

- C:/Program Files/LibOS/bin/LibOS.dll
- C:/Program Files/LibOS/lib/LibOS.lib
- C:/Program Files/LibOS/include/LibOS/...
- C:/Program Files/LibOS/lib/cmake/LibOS/LibOSConfig.cmake

------------------------------------------------------------
Klaar
------------------------------------------------------------

Je hoeft dus geen Windows SDK handmatig te kiezen.
Je hoeft geen Desktop SDK meer te zoeken.
Je hoeft geen Ninja te gebruiken (sterker nog: niet doen).
Je hoeft geen individuele componenten meer aan te vinken.

Alleen “Desktop development with C++” is genoeg.
