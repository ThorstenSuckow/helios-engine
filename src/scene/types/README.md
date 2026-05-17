# helios::engine::scene::types

Type layer for scene-domain handles and render/camera contexts.

## Overview

`helios::engine::scene::types` contains strong-id aliases and immutable context payloads
used across scene extraction, culling, and rendering integration.

## Types

| Type | Purpose |
|------|---------|
| `SceneDomainTag` | Shared domain tag for scene IDs and handles |
| `SceneId` | Strong id for scene entities |
| `SceneHandle` | ECS handle alias for scene entities |
| `CullingContext<THandle>` | Culling payload containing matrices, bounds, and member handle |
| `PerspectiveCameraContext` | Perspective projection parameters used by culling/rendering |
| `SceneMemberRenderContext<THandle>` | Render payload for one scene member in one viewport |

## Related Modules

- `helios.engine.scene.Scene`
- `helios.engine.scene.SceneNode`
- `helios.engine.scene.SceneToViewportMap`

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::scene::types
@brief Type layer for scene-domain handles and render/camera contexts.
</p></details>

