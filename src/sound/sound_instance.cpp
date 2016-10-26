#include "sound_instance.h"

namespace Sound {
  SoundInstance::SoundInstance(std::shared_ptr<YSE::sound> sound) : sound(sound) {
  }

  void SoundInstance::play() {
    sound->play();
  }

  const bool SoundInstance::isPlaying() const noexcept {
    return sound->isPlaying();
  }

  void SoundInstance::stop() {
    sound->stop();
  }

  const bool SoundInstance::isStopped() const noexcept {
    return sound->isStopped();
  }

  void SoundInstance::setLooping(const bool looping) {
    sound->setLooping(looping);
  }

  const bool SoundInstance::isLooping() const noexcept {
    return sound->isLooping();
  }

  void SoundInstance::onStart() {

  }

  const bool SoundInstance::onUpdate(const double delta) {
    return true;
  }

  void SoundInstance::onDestroy() {

  }

  void SoundInstance::onNotifyNow(std::shared_ptr<Events::Event> event) {

  }

  void SoundInstance::handleQueuedEvent(std::shared_ptr<Events::Event> event) {

  }  

  const unsigned long long SoundInstance::getValueForSorting() const noexcept {
    return 43434;
  }
}