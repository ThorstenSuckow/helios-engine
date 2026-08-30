module;

#include <algorithm>
#include <memory>
#include <span>

export module helios.engine.state.StateTransitionRules;

import helios.engine.state.types.StateTransitionRule;

export namespace helios::engine::state {

    /**
     * @brief Provides a set of state transition rules for a given state type.
     *
     * @tparam TState The state type for which to define transition rules.
     */
    template<typename TState>
    class StateTransitionRules {

        using StateTransitionRule = types::StateTransitionRule<TState>;

        class Concept {

        public:
            virtual ~Concept() = default;

            virtual std::span<const StateTransitionRule> rules() const noexcept = 0;
        };

        template<typename TConcreteRules>
        class Model final : public Concept{

            TConcreteRules concreteRules_;

        public:

            explicit Model(TConcreteRules&& concreteRules) :
            concreteRules_(std::move(concreteRules)) {}

            std::span<const StateTransitionRule> rules() const noexcept override {
                return concreteRules_.rules();
            }
        };

        std::unique_ptr<Concept> model_;

    public:

        template<typename TConcreteRules>
        requires std::is_rvalue_reference_v<TConcreteRules&&>
        explicit StateTransitionRules(TConcreteRules&& concreteRules) :
        model_(std::make_unique<Model<std::remove_cvref_t<TConcreteRules>>>(std::move(concreteRules))) {}

        [[nodiscard]] std::span<const StateTransitionRule> rules() const noexcept {
            return model_->rules();
        }

        [[nodiscard]] auto begin() const noexcept {
            return model_->rules().begin();
        }
        [[nodiscard]] auto end() const noexcept {
            return model_->rules().end();
        }

    };

}
