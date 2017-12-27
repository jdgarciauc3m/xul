#ifndef TIME_METER_OPTIONAL_METER_H
#define TIME_METER_OPTIONAL_METER_H

namespace xul {

namespace time_meter {

template <class M, bool COND>
class optional_meter_base : private M
{
public:
  void start() { M::start(); }
  void stop() { M::stop(); }
  template <typename U>
    unsigned int count() { return M::template count<U>(); }
  constexpr static bool is_active() { return true; }
};

template <class M>
class optional_meter_base<M, false> {
public:
  void start() { }
  void stop() { }
  template <typename U>
    unsigned int count() { return {}; }
  constexpr static bool is_active() { return false; }
};

#if defined(XUL_TIME_METER_ENABLED) && !defined(XUL_TIMER_DISABLED)
template <class M>
using optional_meter = optional_meter_base<M,true>;
#elif defined(XUL_TIME_METER_DISABLED) && !defined(XUL_TIMER_ENABLED)
template <class M>
using optional_meter = optional_meter_base<M,false>;
#else
static_assert(false, "Must set -DXUL_TIME_METER_ENABLED xor -DXUL_TIME_METER_DISABLED in compiler flags");
#endif

}

}

#endif
