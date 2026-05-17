# helios::engine::runtime::enginestate::types

Core types for game state management.

## Types

| Type | Description |
|------|-------------|
| `EngineState` | Enum of possible game states (bitmask) |
| `EngineStateTransitionContext` | Context for a state transition |
| `EngineStateTransitionId` | Enum of transition identifiers |
| `EngineStateTransitionRequest` | Request to transition from a state |
| `EngineStateTransitionRule` | Definition of a valid transition |
| `EngineStateTransitionType` | Categorization of transitions |

## EngineState

Bitmask enum with values:
- `Undefined`, `Booted`, `Title`, `MatchReady`, `Running`, `Paused`, `Any`

Supports bitwise operators for combining states.

## EngineStateTransitionId

Transition identifiers:
- `Undefined`, `BootRequest`, `ReadyMatchRequest`, `StartMatchRequest`, `TogglePause`, `TitleRequest`

## EngineStateTransitionRule

Defines a valid transition with:
- Source state (`from`)
- Transition identifier (`transitionId`)
- Target state (`to`)
- Optional guard callback

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::enginestate::types
@brief Core types for game state management.
@details Contains enums, structs, and classes for state machine operations.
</p></details>
