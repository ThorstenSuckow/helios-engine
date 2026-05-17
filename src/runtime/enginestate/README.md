# helios::engine::runtime::enginestate

Game state management for the helios engine.

This module provides domain-specific types and bindings for managing game states (Booted, Title, MatchReady, Running, Paused). It uses the generic `helios::engine::state` framework.

## Components

| Component | Description |
|-----------|-------------|
| `EngineStateManager` | Type alias for `StateManager<EngineState>` |
| `EngineState` | Enum defining application-level states |
| `EngineStateTransitionId` | Enum defining valid transition triggers |

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `rules/` | Default transition rules and guards |
| `systems/` | Systems for input-driven state changes |
| `types/` | Core types, enums, and type trait specializations |

## Architecture

`EngineStateManager` is a type alias for `StateManager<EngineState>`:

```cpp
using EngineStateManager = helios::engine::state::StateManager<types::EngineState>;
```

The generic `StateManager` provides:
- Rule-based transitions with optional guards
- Listener notifications on state changes
- Command-driven state changes via `StateCommand<EngineState>`

## State Flow

```
┌──────────┐  BootRequest  ┌──────────┐  TitleRequest  ┌──────────┐
│Undefined │ ────────────► │  Booted  │ ─────────────► │  Title   │
└──────────┘               └──────────┘                └──────────┘
                                                             │
                                                    ReadyMatchRequest
                                                             │
                                                             ▼
                                                       ┌────────────┐
                           ┌─── TogglePause ────────── │  Running   │
                           │                           └────────────┘
                           ▼                                 ▲
                     ┌──────────┐                            │
                     │  Paused  │ ─── TogglePause ───────────┘
                     └──────────┘
```

## Usage

```cpp
using namespace helios::engine::runtime::enginestate;
using namespace helios::engine::state;
using namespace helios::engine::state::commands;

// Create manager with rules
auto manager = std::make_unique<EngineStateManager>(engineStateRules);

// Add listeners
manager->addStateListener(std::make_unique<LambdaStateListener<types::EngineState>>(
    /* onExit, onTransition, onEnter callbacks */
));

// Submit transition via command
commandBuffer.add<StateCommand<types::EngineState>>(
    StateTransitionRequest<types::EngineState>{
        types::EngineState::Title,
        types::EngineStateTransitionId::ReadyMatchRequest
    }
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::enginestate
@brief Game state management for the helios engine.
@details Provides domain-specific types and bindings for managing game states using the generic helios::engine::state framework.
</p></details>
