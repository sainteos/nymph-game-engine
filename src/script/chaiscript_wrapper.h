#ifndef CHAISCRIPT_WRAPPER_H
#define CHAISCRIPT_WRAPPER_H
#include <memory>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

namespace Script {
  class ChaiscriptWrapper {
    private:
      static std::shared_ptr<chaiscript::ChaiScript> interpreter;
      static void createInstance();
    public:
      static std::shared_ptr<chaiscript::ChaiScript> getInstance();
      static void destroyInstance();
  };
}

#endif
