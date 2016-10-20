#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H
#include <memory>
#include <type_traits>
#include <chaiscript/chaiscript.hpp>
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Subject Observer

namespace Script {
  /**
   * @brief      Class for script object.
   */
  class ScriptObject : public Events::Subject, public Events::Observer, public std::enable_shared_from_this<ScriptObject> {
    private:
      std::string class_name;
      std::shared_ptr<chaiscript::ChaiScript> interpreter;

      chaiscript::Boxed_Value script_object;
      std::function<void (chaiscript::Boxed_Value&)> script_on_start;
      std::function<void (chaiscript::Boxed_Value&, const float)> script_on_update;
      std::function<void (chaiscript::Boxed_Value&)> script_on_destroy;
      std::function<void (chaiscript::Boxed_Value&, std::shared_ptr<ScriptObject>)> script_set_obj_handle;

      template<class T>
      typename std::enable_if<std::is_base_of<Events::Event, T>::value, std::function<void(chaiscript::Boxed_Value&, std::shared_ptr<T>)>>::type selectEventHandler();

      void invokeScriptFunction(std::function<void ()> func);

    public:
      ScriptObject() = delete;
      /**
       * @brief      ScriptObject constructor
       *
       * @param[in]  name              The name
       * @param[in]  chai_interpreter  The chaiscript interpreter
       */
      ScriptObject(const std::string& name, std::shared_ptr<chaiscript::ChaiScript> chai_interpreter);
      /**
       * @brief      Copy constructor for ScriptObject
       *
       * @param[in]  other  The other
       */
      ScriptObject(const ScriptObject& other);
      /**
       * @brief      Destroys the object.
       */
      ~ScriptObject() = default;

      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Sets the class name on the scripting object
       *
       * @param[in]  name  The name
       */
      void setClassName(const std::string& name);
      //= END SCRIPTABLE

      /**
       * @brief      Calls onStart in the attached script object
       */
      void onStart();
      /**
       * @brief      Calls onUpdate in the attached script object
       *
       * @param[in]  delta  The delta
       */
      void onUpdate(const float delta);
      /**
       * @brief      Calls onDestroy in the attached script object
       */
      void onDestroy();

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
  };
}
#endif
