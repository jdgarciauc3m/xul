#ifndef TIME_METER_SYSTEM_METER_H
#define TIME_METER_SYSTEM_METER_H

#include <chrono>

namespace xul {

namespace time_meter {

template <class CLK>
class system_meter {
public:
	using time_point = typename CLK::time_point;
	using duration = typename CLK::duration;

private:
	CLK clock;
	time_point t1, t2;

public:
	void start() { t1 = t2 = CLK::now(); }
	void stop() { t2 = CLK::now(); }

        template <typename U>
        unsigned int count() {
          using namespace std::chrono;
          auto dif = t2-t1;
          return duration_cast<U>(dif).count();
        }
};

} // namespace time_meter

} // namespace xul

#endif
