#include "utility/fps_counter.h"
#include <thread>

namespace Utility {
  using clock = std::chrono::high_resolution_clock;
  using microseconds = std::chrono::microseconds;

  FPSCounter::FPSCounter(const float max_fps = 0.0f) :
    last_time(clock::now()), current_time(clock::now()),
    delta(0.0f), delta_accum(0.0f), fps_accum(0.0f), frame_count(0) {
  }

  const float FPSCounter::getMaxFPS() const noexcept {
    return max_fps;
  }

  const float FPSCounter::getCurrentFPS() const noexcept {
    return current_fps;
  }

  const float FPSCounter::getAverageFPS() const noexcept {
    if(frame_count == 0)
      return 0.0f;

    return fps_accum / (float)frame_count;
  }

  void FPSCounter::resetAverageFPS() noexcept {
    fps_accum = 0.0f;
    frame_count = 0;
  }

  const float FPSCounter::assessCountAndGetDelta() {    
    //if we have a max fps
    if(max_fps > 0.0) {
      //keep sleeping until we hit the right time
      while(std::chrono::duration_cast<microseconds>(clock::now() - last_time).count() < 1.0 / (max_fps / 1000000.0f)) {
        std::this_thread::sleep_for(microseconds(1));
      }
    }
    //update the current fps
    current_time = clock::now();

    delta_accum += delta;
    if(delta_accum > 1000.0f) {
      delta_accum = 0.0f;
      current_fps = 1.0f / (std::chrono::duration_cast<microseconds>(current_time - last_time).count() / 1000000.0f);
      frame_count++;
      fps_accum += current_fps;
    }
    //set last time to the now time
    delta = std::chrono::duration_cast<microseconds>(current_time - last_time).count() / 1000.0f;
    last_time = current_time;
    return delta;
  }
}