#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H
#include <chrono>
#include "events/subject.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Subject

namespace Utility {
  /**
   * @brief      Class for fps counter.
   */
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
      /**
       * @brief      FPSCounter cunstructer
       *
       * @param[in]  max_fps  The maximum fps
       */
      FPSCounter(const float max_fps);
      /**
       * @brief      Destroys the object.
       */
      virtual ~FPSCounter() = default;
      
      /**
       * @brief      Calculates frame delta from FPS counter
       *
       * @return     frame delta
       */
      const float assessCountAndGetDelta();

      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Gets the maximum fps.
       *
       * @return     The maximum fps.
       */
      const float getMaxFPS() const noexcept;
      /**
       * @brief      Gets the current fps.
       *
       * @return     The current fps.
       */
      const float getCurrentFPS() const noexcept;
      /**
       * @brief      Gets the average fps.
       *
       * @return     The average fps.
       */
      const float getAverageFPS() const noexcept;
      /**
       * @brief      Resets FPS Average
       */
      void resetAverageFPS() noexcept;
      //= END SCRIPTABLE
  };  
}

#endif
