#ifndef SOUND_INSTANCE_H
#define SOUND_INSTANCE_H
#include <yse/yse.hpp>
#include <memory>
#include "../component.h"

namespace Sound {
  class [[scriptable]] SoundInstance : public Component {
    private:
      std::shared_ptr<YSE::sound> sound;
    public:
      SoundInstance() = delete;
      virtual ~SoundInstance() = default;

      [[scriptable]] SoundInstance(std::shared_ptr<YSE::sound> sound);

      [[scriptable]] void play();
      [[scriptable]] bool isPlaying() const noexcept;
      [[scriptable]] void stop();
      [[scriptable]] bool isStopped() const noexcept;
      [[scriptable]] void setLooping(const bool looping);
      [[scriptable]] bool isLooping() const noexcept;

      virtual void onStart() override;
      virtual bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

      virtual unsigned long long getValueForSorting() const noexcept override;
  };
}
#endif
