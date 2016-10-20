#ifndef SCRIPTING_SYSTEM_H
#define SCRIPTING_SYSTEM_H
#include <chaiscript/chaiscript.hpp>
#include "script_object.hpp"

namespace Script {
  /**
   * @brief      Class for scripting system.
   */
  class ScriptingSystem {
    private:
      std::shared_ptr<chaiscript::ChaiScript> chai;
      std::string scripts_location;
      std::vector<std::string> getFileList(const std::string& location);

      std::vector<std::shared_ptr<ScriptObject>> scripts;

      std::string filenameToTitleCase(std::string s);
    public:
      ScriptingSystem() = delete;
      /**
       * @brief      ScriptingSystem constructor, this registers all modules
       *
       * @param[in]  scripts_location  The scripts location
       */
      ScriptingSystem(const std::string scripts_location);
      /**
       * @brief      Registers a chaiscript module
       *
       * @param[in]  module  The module
       */
      void registerModule(chaiscript::ModulePtr module);
      /**
       * @brief      Loads a script.
       *
       * @param[in]  name  The name
       */
      void loadScript(const std::string& name);
      /**
       * @brief      Loads all scripts in scripts location, recursively.
       */
      void loadScripts();

      /**
       * @brief      Adds an object of type to chaiscript
       *
       * @param[in]  object  The object
       * @param[in]  name    The name
       *
       * @tparam     T       Type of the object to add
       */
      template<class T>
      void addObject(std::shared_ptr<T> object, const std::string& name) {
        chai->add(chaiscript::var(object), name);
      }

      /**
       * @brief      Adds a global object of type to chaiscript.
       *
       * @param[in]  object  The object
       * @param[in]  name    The name
       *
       * @tparam     T       Type of the object to add
       */
      template<class T>
      void addGlobalObject(std::shared_ptr<T> object, const std::string& name) {
        chai->add_global(chaiscript::var(object), name);
      }

      /**
       * @brief      Calls onStart in all scripts
       */
      void start();
      /**
       * @brief      Calls onUpdate in all scripts
       *
       * @param[in]  delta  The delta
       */
      void update(const float delta);
      /**
       * @brief      Calls onDestroy in all scripts
       */
      void destroy();
  };
}

#endif
