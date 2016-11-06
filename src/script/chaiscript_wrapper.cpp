#include "chaiscript_wrapper.h"
#include "generated/generated_registrations.h"
#include "glm_registrations.h"

namespace Script { 
  std::shared_ptr<chaiscript::ChaiScript> ChaiscriptWrapper::interpreter = nullptr;


  void ChaiscriptWrapper::createInstance() {
    interpreter = std::make_shared<chaiscript::ChaiScript>();

    //Register modules that were generated by the script registration generator
    try {
      generated::registerModules(interpreter);
    } catch(std::exception& e) {
      LOG(INFO)<<e.what();
    }

    interpreter->add(getGlmModule());
  }

  std::shared_ptr<chaiscript::ChaiScript> ChaiscriptWrapper::getInstance() {
    if(interpreter == nullptr)
      createInstance();
    return interpreter;
  }

  void ChaiscriptWrapper::destroyInstance() {
    interpreter.reset();
  }
}