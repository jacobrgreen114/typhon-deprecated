// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

class Timer final {
  using clock = chrono::steady_clock;

  std::string_view name_;
  clock::time_point start_time_;

 public:
  explicit Timer(std::string_view name = "")
      : name_{name},
        start_time_{clock::now()} {}

  ~Timer() {
    auto end_time = clock::now();
    auto duration = end_time - start_time_;
    auto secs     = static_cast<double>(duration.count()) * clock::period::num / clock::period::den;

    std::cout << "Timer " << name_ << " : " << std::fixed;
    //if (secs > 1) {
    std::cout << secs << " secs" << std::endl;
    //} else {
    //  std::cout << secs * 1000 << " ms" << std::endl;
    //}
  }
};

#ifdef TRACE
#define TRACE_TIMER(name) const auto _trace_timer_ = Timer{name};
#else
#define TRACE_TIMER(name)
#endif