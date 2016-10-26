#ifndef SOUND_INSTANCE_H
#define SOUND_INSTANCE_H
#include <yse/yse.hpp>
#include <memory>
#include "component.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Component


namespace Sound {
  class SoundInstance : public Component {
    private:
      std::shared_ptr<YSE::sound> sound;
    public:
      SoundInstance() = delete;
      virtual ~SoundInstance() = default;

      //= BEGIN SCRIPTABLE
      SoundInstance(std::shared_ptr<YSE::sound> sound);

      void play();
      const bool isPlaying() const noexcept;
      void stop();
      const bool isStopped() const noexcept;
      void setLooping(const bool looping);
      const bool isLooping() const noexcept;

      //= END SCRIPTABLE
      
      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

      virtual const unsigned long long getValueForSorting() const noexcept override;
  };
}
#endif
