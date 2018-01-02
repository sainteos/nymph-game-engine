#include <easylogging++.h>
#include "sound_system.h"

namespace Sound {

  SoundSystem::SoundSystem(const std::string sounds_location) : sounds_location(sounds_location) {
    YSE::System().init();
  }

  bool SoundSystem::loadSound(const std::string filename) {
    std::shared_ptr<YSE::sound> new_sound = std::make_shared<YSE::sound>();

    new_sound->create((sounds_location + filename).c_str());

    LOG(INFO)<<"Loading sound: "<<(sounds_location + filename);

    if(new_sound->isValid()) {
      auto sound_ptr = std::make_shared<SoundInstance>(new_sound);

      auto stripped_filename = filename.substr(0, filename.find_first_of("."));

      sounds[stripped_filename] = sound_ptr;
      LOG(INFO)<<"Successfully loaded: "<<stripped_filename;
      return true;
    }
    else {
      LOG(WARNING)<<"Loading "<<filename<<" was unsuccessful!";
      return false;
    }
  }

  std::shared_ptr<SoundInstance> SoundSystem::getSound(const std::string name) {
    return sounds[name];
  }

  void SoundSystem::update(const double delta) {
    YSE::System().update();
  }

  void SoundSystem::destroy() {
    YSE::System().close();
  }
}
