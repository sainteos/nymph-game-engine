#ifndef FSM_H
#define FSM_H
#include <memory>
#include <stdexcept>
#include <type_traits>
#include "../subject.h"

namespace Events {
  namespace FSM {
    /**
     * @brief      Class for state reactor.
     *
     * @tparam     DataType        The type of data that will be mutated by the reactor.
     * @tparam     StateType       The type of state used by the reactor and fsm.
     * @tparam     TransitionType  The type of transitions used by the reactor and fsm.
     */
    template<class DataType, typename StateType, typename TransitionType>
    class StateReactor {
      protected:
        std::shared_ptr<DataType> data;
        Subject* subject;
        StateType state_type;
      public:
        StateReactor() = delete;
        /**
         * @brief      StateReactor constructor. This sets which state this
         *             reactor is for and sets the data to be mutated.
         *
         * @param[in]  state_type      The state type
         * @param[in]  data_to_mutate  The data to mutate
         * @param[in]  subject         The subject
         */
        StateReactor(const StateType& state_type, const std::shared_ptr<DataType>& data_to_mutate, Subject* subject) : data(data_to_mutate), subject(subject), state_type(state_type) { }

        constexpr StateType getStateType() const noexcept {
          return state_type;
        }

        /**
         * @brief      enterState is called upon triggering of this state
         */
        virtual void enterState() {}
        /**
         * @brief      updateState is called once per engine loop for timed
         *             updates
         *
         * @param[in]  delta            The frame delta
         * @param      transition_type  An optional output transition type
         *
         * @return     True if a new transition is provided, false otherwise
         */
        virtual bool updateState(const double delta, TransitionType& transition_type) { return false; }
        /**
         * @brief      leaveState is called upon triggering of a new state from this state.
         */
        virtual void leaveState() {}

        /**
         * @brief      This function is used to set up reactions for different
         *             types of transitions for this state.
         *
         * @param[in]  transition  The transition type
         *
         * @return     The new state to transition into
         */
        virtual StateType react(const TransitionType transition) { return (StateType)0; }
    };

    /**
     * @brief      Generic Finite State Machine
     *
     * @tparam     DataType        The Data to be mutated by this finite state machine.
     * @tparam     StateType       The Enum that holds all possible states for the FSM.
     * @tparam     TransitionType  The Enum that holds all possible transitions for the FSM.
     * @tparam     Reactor         Variadic. One reactor for each State in StateType
     */
    template<typename DataType, typename StateType, typename TransitionType, typename... Reactor>
    class FSM : public std::enable_shared_from_this<FSM<DataType, StateType, TransitionType, Reactor...>> {
      private:
        std::shared_ptr<DataType> data;
        Subject* subject;
        StateType current_state;
        StateType end_state;
        bool is_terminated;

        std::map<StateType, std::shared_ptr<StateReactor<DataType, StateType, TransitionType>>> states;
        bool running;

        template<typename T>
        void unpackReactorAndMakeShared(const std::shared_ptr<T>& t) {
          states[t->getStateType()] = t;
        }

        template<typename T, typename... R>
        void unpackReactorAndMakeShared(const std::shared_ptr<T>& t, const std::shared_ptr<R>&... reactors) {
          states[t->getStateType()] = t;

          unpackReactorAndMakeShared(reactors...);
        }

        template<typename... R>
        void unpackAndConstructReactors() {
          unpackReactorAndMakeShared<R...>(std::make_shared<R>(data, subject)...);
        }

      public:
        FSM() = delete;
        /**
         * @brief      Finite state machine constructor
         *
         * @param[in]  data_to_mutate  The data to mutate
         * @param[in]  subject         The subject
         * @param[in]  begin_state     The begin state
         */
        FSM(const std::shared_ptr<DataType>& data_to_mutate, const StateType& begin_state, Subject* subject) : data(data_to_mutate), subject(subject), current_state(begin_state), is_terminated(false), running(false)  {
          if(data_to_mutate == nullptr) {
            throw std::invalid_argument("FSM::data_to_mutate cannot be nullptr");
          }

          unpackAndConstructReactors<Reactor...>();
        }

        /**
         * @brief      Finite state machine constructor for a terminated FSM
         *
         * @param[in]  data_to_mutate  The data to mutate
         * @param[in]  subject         The subject
         * @param[in]  begin_state     The begin state
         * @param[in]  end_state       The end state
         */
        FSM(const std::shared_ptr<DataType>& data_to_mutate, const StateType& begin_state, const StateType& end_state, Subject* subject) : data(data_to_mutate), current_state(begin_state), end_state(end_state), is_terminated(true), subject(subject) {
          if(data_to_mutate == nullptr) {
            throw std::invalid_argument("FSM::data_to_mutate cannot be nullptr");
          }

          unpackAndConstructReactors<Reactor...>();
        }

        /**
         * @brief      Sends a transition value to the FSM, causing the current
         *             state to react, and possible transition.
         *
         * @param[in]  transition_type  The transition type
         */
        void transition(const TransitionType transition_type) {
          auto new_state_type = states[current_state]->react(transition_type);
          if(new_state_type != current_state) {
            states[current_state]->leaveState();
            current_state = new_state_type;
            states[current_state]->enterState();
          }
          if(is_terminated && current_state == end_state) {
            running = false;
          }
        }

        /**
         * @brief      Check to see if the FSM is in it's final state.
         *
         * @return     Returns true if this is a terminated FSM and it is
         *             in its final state. False otherwise.
         */
        constexpr bool inFinalState() const noexcept {
          return is_terminated && current_state == end_state;
        }

        /**
         * @brief      Starts the FSM.
         */
        void start() {
          running = true;
        }

        /**
         * @brief      Update is called once per update loop in the engine.
         *
         * @param[in]  delta  The frame delta
         */
        void update(const double delta) {
          if(running) {
            TransitionType t;
            if(states[current_state]->updateState(delta, t)) {
              transition(t);
            }
          }
        }

        /**
         * @brief      Stops the FSM.
         */
        void stop() {
          running = false;
        }
    };
  }
}

#endif
