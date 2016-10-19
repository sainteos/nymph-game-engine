#ifndef SCRIPTING_SYSTEM_H
#define SCRIPTING_SYSTEM_H
#include <chaiscript/chaiscript.hpp>
#include "script_object.hpp"

namespace Script {
  class ScriptingSystem {
    private:
      std::shared_ptr<chaiscript::ChaiScript> chai;
      std::string scripts_location;
      std::vector<std::string> getFileList(const std::string& location);

      std::vector<std::shared_ptr<ScriptObject>> scripts;

      std::string filenameToTitleCase(std::string s);
    public:
      ScriptingSystem() = delete;
      ScriptingSystem(const std::string scripts_location);
      void registerModule(chaiscript::ModulePtr module);
      void loadScript(const std::string& name);
      void loadScripts();

      template<class T>
      void addObject(std::shared_ptr<T> object, const std::string& name) {
        chai->add(chaiscript::var(object), name);
      }

      template<class T>
      void addGlobalObject(std::shared_ptr<T> object, const std::string& name) {
        chai->add_global(chaiscript::var(object), name);
      }

      void start();
      void update(const float delta);
      void destroy();
  };
}

#endif
