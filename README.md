# Shadow Lantern

Shadow Lantern is a prototype scaffold for a low-level C++17 online multiplayer horror experience built without a heavyweight game engine. The project targets Windows (MSVC) and Linux (GCC/Clang) through a CMake build that produces separate headless server and graphical client binaries.

## Features

- Authoritative 30 Hz ENet server that owns gameplay state (players, monster, doors, pickups, generators).
- Prediction/interpolation-ready client with lightweight ECS, movement, and placeholder rendering/audio systems.
- Stub third-party integrations (GLFW, GLAD, stb_image, tinyobjloader, miniaudio, ENet, ImGui) designed for easy replacement with the actual libraries.
- Assets folder containing a seed map description (`assets/map.json`) that describes generator, door, and collision layout.

## Build Instructions

```bash
cmake -S . -B build
cmake --build build
```

On Windows use the "x64 Native Tools" command prompt and run the same commands to generate Visual Studio projects or Ninja builds.

### Dependencies

The repository ships with stub implementations of the required third-party libraries so the scaffold compiles out of the box. Replace the contents of `third_party/` with the real libraries (or hook them via `FetchContent`) before shipping. Each stub mirrors the header footprint used inside the project to keep the integration points clear.

## Running

### Server

```
./build/server/Server --port 27015 --maxPlayers 8
```

### Client

```
./build/client/Client --ip 127.0.0.1 --port 27015 --name Ryan
```

The client currently boots into a minimal render loop with placeholder systems. Networking, prediction, and interpolation hooks are in place for extending with real gameplay logic.

## Key Tunables

- **Port**: change via the `--port` command line argument on either binary. For hard-coded defaults see `server/include/server/server_app.hpp` and `client/include/client/client_app.hpp`.
- **Tick rate**: `common/include/common/config.hpp` controls the 30 Hz server update step.
- **Field of View (FOV)**: adjust inside the renderer once the projection matrix is implemented (see `client/src/render.cpp`).
- **Key bindings**: the input system maps WASD to locomotion. Modify `client/src/input.cpp` to customize bindings.

## Next Steps

- Replace stubbed third-party libraries with full upstream sources.
- Flesh out collision detection, prediction reconciliation, monster AI, and the full gameplay loop.
- Expand rendering with actual OpenGL resource management, forward lighting, and post-processing shaders.
- Integrate ImGui debug tools and author actual assets referenced by `map.json`.
