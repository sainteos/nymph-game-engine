#include <easylogging++.h>
#include "script_object.hpp"
#include "events/events.h"

namespace Script {

  ScriptObject::ScriptObject(const std::string& name, std::shared_ptr<chaiscript::ChaiScript> chai_interpreter) : class_name(name), interpreter(chai_interpreter) {
    try {
      script_object = interpreter->eval(class_name + "()");
    } catch(chaiscript::exception::bad_boxed_cast& e) {
      LOG(ERROR)<<e.what();
    } catch(chaiscript::exception::eval_error e) {
      std::stringstream error;
      error << "File: "<<e.filename<<" eval error at ("<<e.start_position.line<<", "<<e.start_position.column<<"): "<<e.reason;
      LOG(INFO)<<error.str();
    }
    
    script_on_start = interpreter->eval<std::function<void (chaiscript::Boxed_Value&)>>("onStart");
    script_on_update = interpreter->eval<std::function<void (chaiscript::Boxed_Value&, const float)>>("onUpdate");

    script_on_destroy = interpreter->eval<std::function<void (chaiscript::Boxed_Value&)>>("onDestroy");
    script_set_obj_handle = interpreter->eval<std::function<void (chaiscript::Boxed_Value&, std::shared_ptr<ScriptObject>)>>("setScriptObjectHandle");
  }

  ScriptObject::ScriptObject(const ScriptObject& other) {
    this->class_name = other.class_name;
    this->interpreter = other.interpreter;
    this->script_object = other.script_object;
  }

  void ScriptObject::setClassName(const std::string& name) {
    this->class_name = name;
  }

  template<class T> 
  typename std::enable_if<std::is_base_of<Events::Event, T>::value, std::function<void(chaiscript::Boxed_Value&, std::shared_ptr<T>)>>::type ScriptObject::selectEventHandler() {
    return interpreter->eval<std::function<void (chaiscript::Boxed_Value&, std::shared_ptr<T>)>>("handleEvent");
  }

  void ScriptObject::invokeScriptFunction(std::function<void ()> func) {
    try {
      func();
    }
    catch(chaiscript::exception::eval_error e) {
      std::stringstream error;
      error << "File: "<<e.filename<<" eval error at ("<<e.start_position.line<<", "<<e.start_position.column<<"): "<<e.reason;
      LOG(INFO)<<error.str();
    }
    catch(chaiscript::exception::dispatch_error& e) {
      std::stringstream error;
      error <<"Dispatch error: " << e.what() << "\nWith functions: \n";
      for(auto func : e.functions) {
        error << "Arity: "<<func->get_arity()<<" Expecting parameters: \n";
        for(auto ti : func->get_param_types()) {
          error << ti.name()<<", ";
        }
        error<<"\n";
      }
      error << "\nWith parameters:\n";
      for(auto func : e.parameters) {
        error << func.get_type_info().name()<<", ";
      }
      LOG(ERROR)<<error.str();
    }
  }

  void ScriptObject::onStart() {
    //Try to give out handle to c++ this to script
    try {
      script_set_obj_handle(script_object, shared_from_this());
    }
    //Be okay with it if we don't find it, but let it be known at an info level.
    catch(chaiscript::exception::dispatch_error& e) {
      LOG(INFO)<<"No C++ this setter for script class "<<class_name;
    }
    
    invokeScriptFunction(std::bind(script_on_start, script_object));
  }

  void ScriptObject::onUpdate(const float delta) {
    processEventQueue();
    invokeScriptFunction(std::bind(script_on_update, script_object, delta));
  }

  void ScriptObject::onDestroy() {
    invokeScriptFunction(std::bind(script_on_destroy, script_object));
  }

  void ScriptObject::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  void ScriptObject::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
    switch(event->getEventType()) {
      case Events::MOUSE_CURSOR: {
        auto handler = selectEventHandler<Input::MouseCursorEvent>();
        auto casted_event = std::static_pointer_cast<Input::MouseCursorEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::MOUSE_SCROLL: {
        auto handler = selectEventHandler<Input::MouseScrollEvent>();
        auto casted_event = std::static_pointer_cast<Input::MouseScrollEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::MOUSE_BUTTON_DOWN: {
        auto handler = selectEventHandler<Input::MouseButtonDownEvent>();
        auto casted_event = std::static_pointer_cast<Input::MouseButtonDownEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::MOUSE_BUTTON_UP: {
        auto handler = selectEventHandler<Input::MouseButtonUpEvent>();
        auto casted_event = std::static_pointer_cast<Input::MouseButtonUpEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::CURSOR_ENTER: {
        auto handler = selectEventHandler<Input::CursorEnterEvent>();
        auto casted_event = std::static_pointer_cast<Input::CursorEnterEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::CURSOR_LEAVE: {
        auto handler = selectEventHandler<Input::CursorLeaveEvent>();
        auto casted_event = std::static_pointer_cast<Input::CursorLeaveEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::KEY_DOWN: {
        auto handler = selectEventHandler<Input::KeyDownEvent>();
        auto casted_event = std::static_pointer_cast<Input::KeyDownEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::KEY_UP: {
        auto handler = selectEventHandler<Input::KeyUpEvent>();
        auto casted_event = std::static_pointer_cast<Input::KeyUpEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::KEY_REPEAT: {
        auto handler = selectEventHandler<Input::KeyRepeatEvent>();
        auto casted_event = std::static_pointer_cast<Input::KeyRepeatEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::CHARACTER_TYPED: {
        auto handler = selectEventHandler<Input::CharacterTypedEvent>();
        auto casted_event = std::static_pointer_cast<Input::CharacterTypedEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::SPRITE_MOVE: {
        auto handler = selectEventHandler<Game::SpriteMoveEvent>();
        auto casted_event = std::static_pointer_cast<Game::SpriteMoveEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::SET_ACTIVE: {
        auto handler = selectEventHandler<SetActiveEvent>();
        auto casted_event = std::static_pointer_cast<SetActiveEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::SET_ENTITY_ACTIVE: {
        auto handler = selectEventHandler<SetEntityActiveEvent>();
        auto casted_event = std::static_pointer_cast<SetEntityActiveEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::SET_UNIFORM: {
        auto handler = selectEventHandler<Graphics::SetUniformEvent>();
        auto casted_event = std::static_pointer_cast<Graphics::SetUniformEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::ANIMATION_TRIGGER: {
        auto handler = selectEventHandler<Game::AnimationTriggerEvent>();
        auto casted_event = std::static_pointer_cast<Game::AnimationTriggerEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::SUSPEND_KEY_INPUT: {
        auto handler = selectEventHandler<Graphics::UI::SuspendKeyInputEvent>();
        auto casted_event = std::static_pointer_cast<Graphics::UI::SuspendKeyInputEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::RESUME_KEY_INPUT: {
        auto handler = selectEventHandler<Graphics::UI::ResumeKeyInputEvent>();
        auto casted_event = std::static_pointer_cast<Graphics::UI::ResumeKeyInputEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::CHANGE_TEXT: {
        auto handler = selectEventHandler<Graphics::UI::ChangeTextEvent>();
        auto casted_event = std::static_pointer_cast<Graphics::UI::ChangeTextEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::DEBUG_COMMAND: {
        auto handler = selectEventHandler<Utility::DebugCommandEvent>();
        auto casted_event = std::static_pointer_cast<Utility::DebugCommandEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::LOAD_MAP: {
        auto handler = selectEventHandler<Utility::LoadMapEvent>();
        auto casted_event = std::static_pointer_cast<Utility::LoadMapEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::LOAD_CHARACTER: {
        auto handler = selectEventHandler<Utility::LoadCharacterEvent>();
        auto casted_event = std::static_pointer_cast<Utility::LoadCharacterEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::TOGGLE_FREE_CAMERA: {
        auto handler = selectEventHandler<Utility::ToggleFreeCameraEvent>();
        auto casted_event = std::static_pointer_cast<Utility::ToggleFreeCameraEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::LIST_MAPS: {
        auto handler = selectEventHandler<Utility::ListMapsEvent>();
        auto casted_event = std::static_pointer_cast<Utility::ListMapsEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::LIST_CHARACTERS: {
        auto handler = selectEventHandler<Utility::ListCharactersEvent>();
        auto casted_event = std::static_pointer_cast<Utility::ListCharactersEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::LIST_LAYERS: {
        auto handler = selectEventHandler<Utility::ListLayersEvent>();
        auto casted_event = std::static_pointer_cast<Utility::ListLayersEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::TOGGLE_LIGHTS: {
        auto handler = selectEventHandler<Utility::ToggleLightsEvent>();
        auto casted_event = std::static_pointer_cast<Utility::ToggleLightsEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::TOGGLE_LAYER: {
        auto handler = selectEventHandler<Utility::ToggleLayerEvent>();
        auto casted_event = std::static_pointer_cast<Utility::ToggleLayerEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      case Events::WINDOW_EXIT: {
        auto handler = selectEventHandler<Utility::WindowExitEvent>();
        auto casted_event = std::static_pointer_cast<Utility::WindowExitEvent>(event);
        invokeScriptFunction(std::bind(handler, script_object, casted_event));
        break;
      }
      default: {
        LOG(WARNING)<<"Trying to pass event to script that hasn't been implemented!";
        break;
      }
    }
  }
}