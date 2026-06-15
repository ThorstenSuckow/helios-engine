# helios::engine

Runtime and integration layer for the helios C++23 engine modules.

## Overview

`helios::engine` provides the core runtime, world orchestration, messaging,
state, rendering abstractions, platform contracts, and utility infrastructure
used by the split helios repositories.

The former monorepo [`thorstensuckow/helios`](https://github.com/thorstensuckow/helios)
is kept as a legacy reference. Active development happens in the split repositories.

## Features

- Phase/pass based game loop orchestration
- Typed command buffers and command-handler routing with move-consuming submit semantics
- Type-indexed event buses for pass, phase, and frame communication
- Resource and manager registries for runtime services
- Generic state-management utilities
- Scene extraction and rendering support explicit instancing flow via `RenderInstanceBatchCommand` and `InstanceRenderBatchContext`
- Engine-level rendering, scene, platform, spatial, input, tooling, and utility modules

## Module surface

| Area | Public modules / APIs |
|------|------------------------|
| Runtime | `helios.engine.runtime`, `GameLoop`, `UpdateContext`, `GameWorld`, `ResourceRegistry` |
| Messaging | `TypedCommandBuffer`, `CommandHandlerRegistry`, `GameLoopEventBus` |
| State | `StateManager`, `StateCommand`, state listeners, state-to-ID maps |
| Rendering | `helios.engine.rendering`, render targets, viewports, meshes, shaders, materials |
| Platform | `helios.engine.platform`, environment/window/lifecycle contracts |
| Scene/spatial | `helios.engine.scene`, `helios.engine.spatial` |
| Utilities | `helios.engine.core`, `helios.engine.input`, `helios.engine.util`, `helios.engine.tooling` |

## Additional documentation

- Rendering details: `src/rendering/README.md`
  - includes the frame-flow contract for `SceneMemberVisibilityRegistry`
    (created each frame by `SceneMemberVisibilitySystem`, consumed by `SceneRenderSystem` as central visible/culled storage)

## Usage

### C++ module

```cpp
import helios.engine;
```

### CMake

Build and install:

```bash
cmake -S . -B build -DHELIOS_ENGINE_ENABLE_PACKAGE=ON -DCMAKE_INSTALL_PREFIX="$PWD/install"
cmake --build build
cmake --install build
```

Consume from another project:

```cmake
find_package(helios-engine CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE helios::engine)
```

Configure a consumer against an installed prefix:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/helios-prefix"
cmake --build build
```

## Development

Run the regular CMake build from the repository root:

```bash
cmake -S . -B build
cmake --build build
```

## Related repositories

- [`helios-ecs`](https://github.com/thorstensuckow/helios-ecs)
- [`helios-math`](https://github.com/thorstensuckow/helios-math)
- [`helios-opengl`](https://github.com/thorstensuckow/helios-opengl)
- [`helios-glfw`](https://github.com/thorstensuckow/helios-glfw)
