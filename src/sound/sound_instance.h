#ifndef SOUND_INSTANCE_H
#define SOUND_INSTANCE_H
//#include <yse/yse.hpp>
#include <memory>
#include "component.h"

namespace Sound {
  class SoundInstance : public Component {
    private:
      //std::shared_ptr<YSE::sound> sound;
    public:
      SoundInstance() = delete;
      virtual ~SoundInstance() = default;

      //SoundInstance(std::shared_ptr<YSE::sound> sound);

      void play();
      bool isPlaying() const noexcept;
      void stop();
      bool isStopped() const noexcept;
      void setLooping(const bool looping);
      bool isLooping() const noexcept;


      virtual void onStart() override;
      virtual bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

      virtual unsigned long long getValueForSorting() const noexcept override;
  };
}
#endif
