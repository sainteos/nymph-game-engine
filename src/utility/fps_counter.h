#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H
#include <chrono>
#include "events/subject.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Subject

namespace Utility {
  class FPSCounter : public Events::Subject {
    private:
      float max_fps;
      float current_fps;
      std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
      std::chrono::time_point<std::chrono::high_resolution_clock> current_time;
      float delta;
      float delta_accum;
      unsigned int frame_count;
      float fps_accum;
      void sleepFor(const int milliseconds);

    public:
      FPSCounter(const float max_fps);
      virtual ~FPSCounter() = default;
      
      const float assessCountAndGetDelta();

      //= BEGIN SCRIPTABLE
      const float getMaxFPS() const noexcept;
      const float getCurrentFPS() const noexcept;
      const float getAverageFPS() const noexcept;
      void resetAverageFPS() noexcept;
      //= END SCRIPTABLE
  };  
}

#endif
