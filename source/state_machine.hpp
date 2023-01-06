// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#ifdef __cplusplus

#pragma once

template <typename Ret, typename... Args>
using Func = Ret (*)(Args...);

template <typename TContext>
class State final {
  using Handler = Func<State, TContext &>;

  Handler handler_;

 public:
  constexpr explicit State(Handler handler) : handler_{handler} {}
  constexpr explicit State() : State(nullptr) {}
  constexpr State(State &&) noexcept = default;
  constexpr State(const State &) = default;
  constexpr auto operator=(State &&) noexcept -> State & = default;
  constexpr auto operator=(const State &) -> State & = default;

  auto operator()(TContext &ctx) const -> State { return handler_(ctx); }

  operator bool() { return handler_ != nullptr; }
};

template <typename TContext>
class StateMachine {
 public:
  using State = State<TContext>;

 private:
  const State initial_state_;

 public:
  constexpr explicit StateMachine(const State &initialState)
      : initial_state_(initialState) {}

  constexpr auto run(TContext &context) -> void {
    auto state = initial_state_;
    while (state) {
      state = state(context);
    }
  }
};

template <typename T>
class Enumerator {
 public:
  virtual auto current() -> const T & = 0;
  virtual auto move_next() -> bool = 0;
};

template <typename T>
using Predicate = Func<bool, T>;

template <typename TContext, typename TEnumType>
class EnumeratingContext : public Enumerator<TEnumType> {
  using State = State<TContext>;
  using Predicate = Predicate<TEnumType>;

 public:
  struct MatchCondition final {
    Predicate pred;
    State state;
  };

  auto move_next_state(State success, State end_of_file) -> State {
    return this->move_next() ? success : end_of_file;
  }

  auto move_next_state(Predicate pred, State match, State fail, State end)
      -> State {
    if (!this->move_next()) {
      return end;
    }
    return pred(this->current()) ? match : fail;
  }

  template <typename TEnumerable>
  auto move_next_state(State fail, State end, const TEnumerable &enumerable) {
    if (!this->move_next()) {
      return end;
    }

    for (const auto &condition : enumerable) {
      if (condition.pred(this->current())) {
        return condition.state;
      }
    }

    return fail;
  }
};

#endif  // __cplusplus
