#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H
#include <chrono>
#include "../events/subject.h"

namespace Utility {
  /**
   * @brief      Class for fps counter.
   */
  class [[scriptable]] FPSCounter : public Events::Subject {
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
      /**
       * @brief      FPSCounter cunstructer
       *
       * @param[in]  max_fps  The maximum fps
       */
      FPSCounter(const float max_fps = 0.0f);
      /**
       * @brief      Destroys the object.
       */
      virtual ~FPSCounter() = default;

      /**
       * @brief      Calculates frame delta from FPS counter
       *
       * @return     frame delta
       */
      float assessCountAndGetDelta();

      /**
       * @brief      Gets the maximum fps.
       *
       * @return     The maximum fps.
       */
      [[scriptable]] float getMaxFPS() const noexcept;
      /**
       * @brief      Gets the current fps.
       *
       * @return     The current fps.
       */
      [[scriptable]] float getCurrentFPS() const noexcept;
      /**
       * @brief      Gets the average fps.
       *
       * @return     The average fps.
       */
      [[scriptable]] float getAverageFPS() const noexcept;
      /**
       * @brief      Resets FPS Average
       */
      [[scriptable]] void resetAverageFPS() noexcept;
  };
}

#endif
