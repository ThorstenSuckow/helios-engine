/**
 * @file StateManager.ixx
 * @brief Generic state manager and transition orchestrator.
 */
module;

#include <memory>
#include <span>
#include <vector>
#include <cassert>

export module helios.engine.state.StateManager;


import helios.engine.state.StateTransitionListener;
import helios.engine.state.types;
import helios.engine.state.components;
import helios.engine.state.commands;

import helios.engine.runtime.world.GameObject;

import helios.ecs.common.types;
import helios.ecs.common.concepts;

import helios.engine.runtime.world.UpdateContext;

import helios.ecs.command.CommandHandlerRegistry;
import helios.engine.runtime.world.Session;
import helios.engine.runtime.concepts;

import helios.engine.runtime.world.Session;

import helios.ecs.manager;
import helios.ecs.EntityManager;

using helios::engine::state::types::StateTransitionIdType;
using helios::engine::state::types::StateTransitionContext;
using helios::engine::state::types::StateTransitionRule;
using helios::engine::state::commands::StateCommand;
using helios::engine::state::commands::DelayedStateCommand;

export namespace helios::engine::state {


    /**
     * @brief Manages state transitions using a rule-based system.
     *
     * @details Implements both Manager and TypedStateCommandHandler interfaces.
     * Processes state transition commands by matching against registered rules,
     * executing guards, and notifying listeners.
     *
     * ## Transition Flow
     *
     * 1. Commands are submitted via submit() and queued
     * 2. During flush(), the last pending command is processed
     * 3. Rules are checked to find a matching transition
     * 4. If a guard is present, it must return true
     * 5. Listeners are notified: onStateExit -> onStateTransition
     * 6. Session state is updated via StateComponent
     * 7. Listeners are notified: onStateEnter (session already reflects new state)
     *
     * @tparam StateType The state enum type.
     *
     * @see StateTransitionRule
     * @see StateTransitionListener
     * @see StateCommand
     */
    template<typename StateType, typename TInitContext, typename TExecutionContext>
requires ecs::common::concepts::ProvidesCommandHandlerRegistry<TInitContext, ecs::command::CommandHandlerRegistry>
    && engine::runtime::concepts::ProvidesUpdateContext<TExecutionContext, runtime::world::UpdateContext>
    class StateManager {

        /**
         * @brief Queue of pending state commands.
         */
        std::vector<StateCommand<StateType>> pending_;

        /**
         * @brief Registered transition listeners.
         */
        std::vector<std::unique_ptr<StateTransitionListener<StateType>>> listeners_;

        /**
         * @brief Transition rules defining valid state changes.
         */
        std::vector<StateTransitionRule<StateType>> rules_;

        /**
         * @brief Notifies listeners of state exit.
         *
         * @param from The state being exited.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current frame's update context.
         */
        void signalExit(
            const StateType from,
            const StateType to,
            const StateTransitionIdType<StateType> transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onStateExit(updateContext, from);
            }
        }

        /**
         * @brief Notifies listeners of the transition.
         *
         * @param from The source state.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current frame's update context.
         */
        void signalTransition(
           const StateType from,
           const StateType to,
           const StateTransitionIdType<StateType> transitionId,
           helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onStateTransition(
                    updateContext,
                    StateTransitionContext<StateType>{from, to, transitionId}
                );
            }
        }

        /**
         * @brief Notifies listeners of state entry.
         *
         * @param from The source state.
         * @param to The state being entered.
         * @param transitionId The transition identifier.
         * @param updateContext The current frame's update context.
         */
        void signalEnter(
           const StateType from,
           const StateType to,
           const StateTransitionIdType<StateType> transitionId,
           helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onStateEnter(updateContext, to);

            }
        }


    public:
        using EcsRoleTag = helios::ecs::manager::tags::ManagerRole;

        using ExecutionContextType = TExecutionContext;
        using InitContextType = TInitContext;

        /**
         * @brief Constructs a state manager with transition rules.
         *
         * @param rules Span of valid transition rules.
         */
        explicit StateManager(std::span<const StateTransitionRule<StateType>> rules)
        : rules_(rules.begin(), rules.end()) {}

        /**
         * @brief Registers a state transition listener.
         *
         * @param listener The listener to add.
         *
         * @return Reference to this manager for chaining.
         */
        StateManager& addStateListener(std::unique_ptr<StateTransitionListener<StateType>> listener) noexcept {
            listeners_.push_back(std::move(listener));
            return *this;
        }

        /**
         * @brief Processes pending state commands.
         *
         * @details Processes the last pending command, finds matching rules,
         * executes guards, and triggers the transition if valid.
         *
         * @param updateContext The current frame's update context.
         */
        bool executeCommands(TExecutionContext& executionContext) noexcept {

            auto& updateContext = executionContext.updateContext();

            if (pending_.empty()) {
                return true;
            }

            auto command = pending_.back();

            auto transitionRequest = command.transitionRequest();

            auto& session = updateContext.session();
            auto currentFrom = session.template state<StateType>();
            auto from = transitionRequest.from();
            auto transitionId = transitionRequest.transitionId();


            if (currentFrom != from) {
                pending_.clear();
                return true;
            }


            for (auto& rule : rules_) {
                if (rule.from() == from && rule.transitionId() == transitionId) {

                    if (rule.guard()) {
                        if (!rule.guard()(updateContext, transitionRequest)) {
                            break;
                        }
                    }

                    signalExit(from, rule.to(), transitionId, updateContext);
                    signalTransition(from, rule.to(), transitionId, updateContext);
                    session.template setStateFrom<StateType>(StateTransitionContext<StateType>{rule.from(), rule.to(), transitionId});
                    signalEnter(from, rule.to(), transitionId, updateContext);
                }
            }

            pending_.clear();

            return true;
        }

        /**
         * @brief Submits a state command for processing.
         *
         * @param stateCommand The command to queue.
         *
         * @return True (always accepts commands).
         */
        bool submit(
            StateCommand<StateType>&& stateCommand
        ) noexcept {
            pending_.push_back(std::move(stateCommand));
            return true;
        };

        /**
         * @brief Submits a delayed state command for processing.
         *
         * @details Extracts the transition request from the delayed command
         * and queues it as a regular StateCommand. The timer ID is not
         * retained by the manager.
         * The delayed command is guaranteed to be ready for processing when
         * submitted here.
         *
         * @param stateCommand The delayed command to queue.
         *
         * @return True (always accepts commands).
         */
        bool submit(
            DelayedStateCommand<StateType>&& stateCommand
        ) noexcept {
            pending_.push_back(StateCommand<StateType>(std::move(stateCommand.transitionRequest())));
            return true;
        };

        /**
         * @brief Initializes the manager and registers command handlers.
         *
         * @param commandHandlerRegistry The command-handler registry to register with.
         */
        bool init(TInitContext& initContext) {
            auto& commandHandlerRegistry = initContext.commandHandlerRegistry();

            commandHandlerRegistry.template registerHandler<StateCommand<StateType>>(*this);
            commandHandlerRegistry.template registerHandler<DelayedStateCommand<StateType>>(*this);

            return true;
        }

        /**
         * @brief Clears all pending commands.
         */
        void reset() {
            // intentionally left empty. Clearing the pending queue would also mean
            // that any pending state transisions **required by the reset** are nuked.
        }

    };

}
