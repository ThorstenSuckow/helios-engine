# helios::engine::runtime::enginestate::systems

Systems for game state management.

## Systems

| System | Description |
|--------|-------------|
| `EngineStateInputResponseSystem` | Handles input for state transitions |

## EngineStateInputResponseSystem

Listens for the gamepad Start button and issues appropriate `StateCommand<EngineState>` based on the current state:

- **Title** + Start → `ReadyMatchRequest` transition
- **Running** + Start → `TogglePause` transition

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::enginestate::systems
@brief Systems for game state management.
@details Contains ECS systems that handle game state logic.
</p></details>
