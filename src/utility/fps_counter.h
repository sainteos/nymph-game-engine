#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H
#include <chrono>

namespace Utility {
  class FPSCounter {
    private:
      float max_fps;
      float current_fps;
      std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
      std::chrono::time_point<std::chrono::high_resolution_clock> current_time;
      float delta;
      float delta_accum;
      unsigned int frame_count;
      float fps_accum;

    public:
      FPSCounter(const float max_fps);
      const float getMaxFPS() const noexcept;
      const float getCurrentFPS() const noexcept;
      const float getAverageFPS() const noexcept;
      void resetAverageFPS() noexcept;
      const float assessCountAndGetDelta();
  };  
}

#endif
