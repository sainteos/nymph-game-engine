#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H
#define DEBUG
#include <yse/yse.hpp>
#include "sound_instance.h"
//= SCRIPTABLE

namespace Sound {
  class SoundSystem {
    private:
      std::map<std::string, std::shared_ptr<SoundInstance>> sounds;
      std::string sounds_location;

    public:
      SoundSystem() = delete;
      SoundSystem(const std::string sounds_location);
      virtual ~SoundSystem() {}

      //= BEGIN SCRIPTABLE
      bool loadSound(const std::string filename);

      std::shared_ptr<SoundInstance> getSound(const std::string name);
      //= END SCRIPTABLE

      void update(const double delta);
      void destroy();
  };
}

#endif
