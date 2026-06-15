# helios::engine::rendering

Core rendering system and entity rendering components for helios.

## Overview

This namespace provides the core rendering infrastructure including rendering
devices, render commands, queues, passes, and vertex definitions. It encompasses
asset, model, and shader management functionality for comprehensive 3D graphics
rendering, as well as components that link game logic entities to the rendering
system for visual representation.

Provides a platform-independent rendering abstraction that can be implemented by
various graphics APIs (OpenGL, Vulkan, DirectX).

## Scene Visibility Integration

`SceneMemberVisibilityRegistry` (declared in `helios.engine.scene.SceneMemberVisibilityRegistry`)
acts as the central, frame-local storage consumed by the `SceneRenderSystem`.

Per frame, the `SceneMemberVisibilitySystem` creates and fills this registry so
subsequent render operations can query precomputed visibility results:

- visible members per viewport and submission mode (`Instanced` / `NonInstanced`)
- culled members per viewport and submission mode
- scene render contexts used for submission

This decouples visibility determination from render submission and ensures the
render stage works on a deterministic visibility snapshot for the current frame.

## Key Types

| Item | Description |
|------|-------------|
| `RenderingDevice` | Platform-independent rendering device abstraction |
| `RenderPass` / `RenderPassFactory` | Render pass definitions and creation |
| `RenderQueue` | Ordered queue of render operations |
| `RenderPrototype` | Prototype for render configurations |
| `Renderable` | Visual representation of a renderable object |
| `Vertex` | Vertex data definition |
| `ClearFlags` | RenderTarget clear flag configuration |
| `RenderTargetToViewportMap` | Maps renderTargets to viewport regions |
| `RenderableComponent` | Links a GameObject to a Renderable for visual output |
| `ModelAabbComponent` | Stores the model-space AABB for the entity's mesh |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `asset/` | Asset management for rendering resources |
| `commands/` | Render command abstractions |
| `components/` | Rendering-related ECS components |
| `concepts/` | C++ concepts for rendering types |
| `renderTarget/` | RenderTarget management |
| `material/` | Material definitions and types |
| `mesh/` | Mesh data and mesh commands |
| `model/` | Model-space components (AABB in model space) |
| `renderable/` | Renderable reference components |
| `shader/` | Shader management, types, and systems |
| `text/` | Text rendering support |
| `viewport/` | Viewport abstractions and state-based viewport management |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::rendering
@brief Graphics rendering infrastructure and entity rendering components.
@details This namespace provides the core rendering system including rendering devices, render commands, queues, passes, and vertex definitions. Encompasses asset, model, and shader management functionality for comprehensive 3D graphics rendering. Provides a platform-independent rendering abstraction that can be implemented by various graphics APIs (OpenGL, Vulkan, DirectX). Also includes components that link game logic entities to the rendering system for visual representation.
</p></details>
