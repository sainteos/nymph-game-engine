#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H
#include <memory>
#include <type_traits>
#include <json/json.h>
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"
#include "chaiscript_wrapper.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Subject Observer

namespace Script {
  class ScriptingSystem;
  /**
   * @brief      Class for script object.
   */
  class ScriptObject : public Events::Subject, public Events::Observer, public std::enable_shared_from_this<ScriptObject> {
    private:
      friend class ScriptingSystem;

      std::string class_name;
      bool saved;
      bool sent_this;
      unsigned int num_saved_fields;
      std::vector<std::string> serialized_var_names;

      std::shared_ptr<chaiscript::dispatch::Dynamic_Object> script_object;

      std::function<void (std::shared_ptr<chaiscript::dispatch::Dynamic_Object>)> script_on_start;
      std::function<void (std::shared_ptr<chaiscript::dispatch::Dynamic_Object>, const float)> script_on_update;
      std::function<void (std::shared_ptr<chaiscript::dispatch::Dynamic_Object>)> script_on_destroy;
      std::function<void (std::shared_ptr<chaiscript::dispatch::Dynamic_Object>, std::shared_ptr<ScriptObject>)> script_set_obj_handle;

      template<class T>
      typename std::enable_if<std::is_base_of<Events::Event, T>::value, std::function<void(std::shared_ptr<chaiscript::dispatch::Dynamic_Object>, std::shared_ptr<T>)>>::type selectEventHandler();

      void invokeScriptFunction(const std::function<void ()>& func);

      void setSerializable(const std::string& name);

      void create();
      void sendThisToScript();

    public:
      /**
       * @brief      ScriptObject constructor
       *
       * @param[in]  name              The name
       */
      ScriptObject(const std::string& name);
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

      std::string getClassName() const noexcept;
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

      bool shouldBeSerialized() const noexcept;

      Json::Value getSaveData() const noexcept;
      void loadSavedData(Json::Value& save_data);

      bool representsDynamicObject(const chaiscript::dispatch::Dynamic_Object& obj) const noexcept;

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
  };
}
#endif
