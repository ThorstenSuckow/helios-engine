# helios::engine::core

Core-level utilities shared across the helios engine.

This module provides foundational infrastructure including double-buffering for thread-safe message passing, spatial transformations, type definitions, hash algorithms, and data structures used by higher-level subsystems.

## Modules

### algorithms

The `helios.engine.core.types.FuncDefs` module provides core algorithms used throughout the engine.

#### FNV-1a Hash

A fast, compile-time capable hash function for generating identifiers from strings:

```cpp
import helios.engine.core.types.FuncDefs;

// Compile-time hash
constexpr uint32_t id = helios::ecs::types::fnv1a_hash("enemy_spawn");

// Used internally by strongly-typed ID constructors
constexpr EntityPoolId POOL{"bullets"};    // calls fnv1a_hash
constexpr SpawnProfileId PROFILE{"enemies"};   // calls fnv1a_hash
constexpr SpawnRuleId RULE{"wave_spawn"};      // calls fnv1a_hash
```

> **Note:** FNV-1a is not a cryptographic hash. Do not use for security purposes.

### types

The `helios.engine.core.types` module provides core type definitions and tag types used throughout the engine.

#### Tag Types

| Type | Description |
|------|-------------|
| `no_init_t` | Tag type to skip default initialization |
| `no_init` | Convenience constant for uninitialized construction |

#### Uninitialized Construction

The `no_init` tag enables performance-critical code to skip default initialization when objects will be immediately overwritten:

```cpp
import helios.engine.util.Guid;
import helios.engine.core.types;

// Generate a new unique Guid
auto id = helios::engine::util::Guid::generate();

// Declare uninitialized Guid for later assignment
helios::engine::util::Guid deferredId{helios::ecs::types::no_init};

// Assign later when the value is known
deferredId = helios::engine::util::Guid::generate();
```

> **Warning:** Objects constructed with `no_init` are in an indeterminate state. Reading from them before assignment is undefined behavior.

#### Supporting no_init in Custom Types

The `helios::engine::util::Guid` class demonstrates how to add `no_init` support:

```cpp
import helios.engine.core.types;

class Guid final {
    uint64_t value_{};

public:
    // Skip initialization for deferred assignment
    explicit Guid(helios::ecs::types::no_init_t) {}

    // Factory method for generating valid Guids
    static Guid generate() noexcept;
};
```

### concepts

The `helios.engine.core.concepts` module provides shared compile-time constraints used
across multiple domains.

| Concept | Purpose |
|---------|---------|
| `IsStrongIdLike<T>` | Constrains strong-id style types used across ECS and core modules |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::core
@brief Core utilities shared across the helios engine.
@details This module provides foundational infrastructure including double-buffering, spatial transformations, type definitions, hash algorithms, and data structures used by higher-level subsystems.
</p></details>



