module;

#include <utility>

export module helios.engine.runtime.Session;

import helios.ecs;

import helios.engine.state.components;
import helios.engine.state.types;

export namespace helios::engine::runtime {

    class Session {

        struct SessionDomainTag{};
        using SessionHandle = ecs::common::types::EntityHandle<SessionDomainTag>;
        using EntityManager = ecs::EntityManager<SessionHandle>;
        using SessionObject = ecs::Entity<ecs::EntityManager<SessionHandle>>;
        using ConstSessionObject = ecs::Entity<const ecs::EntityManager<SessionHandle>>;

        EntityManager entityManager_;

        SessionHandle sessionHandle_;
        
        [[nodiscard]] SessionObject sessionObject() noexcept {
            return SessionObject{sessionHandle_, &entityManager_};
        };

        [[nodiscard]] ConstSessionObject sessionObject() const noexcept {
            return ConstSessionObject{sessionHandle_, &entityManager_};
        }

    public:

        Session() : sessionHandle_(entityManager_.create()) {
            sessionObject().add<ecs::components::Uninitialized<SessionHandle>>();
        }

        [[nodiscard]] bool isInitialized() const noexcept {
            return !sessionObject().has<ecs::components::Uninitialized<SessionHandle>>();
        }

        [[nodiscard]] bool initialize() noexcept {
            return sessionObject().remove<ecs::components::Uninitialized<SessionHandle>>();
        }

        [[nodiscard]] bool isDestroyed() noexcept {
            return sessionObject().has<ecs::components::Destroyed<SessionHandle>>();
        }

        void destroy() noexcept {
            sessionObject().add<ecs::components::Destroyed<SessionHandle>>();
        }

        /**
         * @brief Resets the session state.
         */
        void reset() {

        }

        /**
         * @brief Updates state from a transition context.
         *
         * @details Called by StateManager after a successful transition.
         *
         * @tparam StateType The state enum type.
         *
         * @param stateTransitionContext The completed transition context.
         */
        template<typename StateType>
        void setStateFrom(const engine::state::types::StateTransitionContext<StateType> stateTransitionContext) noexcept {

            if (auto* msc = sessionObject().get<state::components::StateComponent<StateType>>()) {
                msc->setStateFromTransitionContext(stateTransitionContext);
            }
        }

        /**
         * @brief Returns the current state for a given state type.
         *
         * @tparam StateType The state enum type.
         *
         * @return The current state, or StateType::Undefined if not found.
         */
        template<typename StateType>
        [[nodiscard]] StateType state() const noexcept {
            auto* sc = sessionObject().get<state::components::StateComponent<StateType>>();

            return sc ? sc->state() : StateType::Undefined;
        }

        /**
         * @brief Returns the source state of the last transition.
         *
         * @tparam StateType The state enum type.
         *
         * @return The state that was transitioned from, or StateType::Undefined if not found.
         */
        template<typename StateType>
        [[nodiscard]] StateType stateFrom() const noexcept {
            auto* sc = sessionObject().get<state::components::StateComponent<StateType>>();

            return sc ? sc->from() : StateType::Undefined;
        }

        /**
         * @brief Returns the last transition ID for a given state type.
         *
         * @tparam StateType The state enum type.
         *
         * @return The transition ID, or Undefined if not found.
         */
        template<typename StateType>
        [[nodiscard]] state::types::StateTransitionIdType<StateType> stateTransitionId() const noexcept {
            auto* ms = sessionObject().get<state::components::StateComponent<StateType>>();

            return ms ? ms->transitionId() : state::types::StateTransitionIdType<StateType>::Undefined;
        }

        /**
         * @brief Lets this session track the specified StateType.
         *
         * @tparam StateType The state enum type.
         */
        template<typename StateType>
        void trackState() {
            sessionObject().getOrAdd<state::components::StateComponent<StateType>>();
        }



    };


}