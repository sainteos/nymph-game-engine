#define TINYDIR_USE_READDIR_R 0
#include <easylogging++.h>
#include <tinydir.h>
#include <regex>
#include <cctype>
#include <fstream>
#include "game/scene.h"
#include "scripting_system.h"
#include "chaiscript_wrapper.h"

namespace Script {

  ScriptingSystem::ScriptingSystem(const std::string scripts_location) : scripts_location(scripts_location) {
    ChaiscriptWrapper::getInstance()->add(chaiscript::fun([&](std::shared_ptr<ScriptObject> so, std::string name) {
      so->setSerializable(name);
    }), "set_serializable");
  }

  std::string ScriptingSystem::filenameToTitleCase(std::string s) {
    std::string new_string = s;

    //drop full location specification
    auto last_slash = new_string.rfind('/');

    new_string.erase(0, last_slash + 1);

    //drop file extension
    new_string.erase(new_string.find('.'));

    new_string[0] = toupper(new_string[0]);

    while(new_string.find('_') != std::string::npos) {
      auto pos = new_string.find('_');
      new_string[pos+1] = toupper(new_string[pos+1]);
      new_string.erase(pos, 1);
    }

    return new_string;
  }

  void ScriptingSystem::registerModule(chaiscript::ModulePtr module) {
    ChaiscriptWrapper::getInstance()->add(module);
  }

  void ScriptingSystem::loadScript(const std::string& name) {
    LOG(INFO)<<"Loading script: "<<name;

    try {
      ChaiscriptWrapper::getInstance()->use(name);
    } catch(chaiscript::exception::eval_error e) {
      LOG(INFO)<<e.pretty_print();
    }

    auto class_name = filenameToTitleCase(name);

    auto script = std::make_shared<ScriptObject>(class_name);

    scripts.push_back(script);
    LOG(INFO)<<"Added scripting class: "<<class_name;
  }

  std::vector<std::string> ScriptingSystem::getFileList(const std::string& location) {
    std::vector<std::string> files;

    tinydir_dir dir;
    tinydir_open(&dir, location.c_str());

    while(dir.has_next) {
      tinydir_file file;
      tinydir_readfile(&dir, &file);

      std::regex chai_match("[^[:space:]]+[.]chai");

      if(!file.is_dir && std::regex_match(std::string(file.name), chai_match) ) {
        files.push_back(std::string(location + file.name));
      }
      else if(file.is_dir && std::string(file.name) != "." && std::string(file.name) != "..") {
        auto dir_files = getFileList(location + file.name + "/");
        files.insert(files.end(), dir_files.begin(), dir_files.end());
      }

      tinydir_next(&dir);
    }

    return files;
  }

  void ScriptingSystem::loadScripts() {
    for(auto file : getFileList(scripts_location)) {
      loadScript(file);
    }

    for(auto script1 : scripts) {
      script1->sendThisToScript();
      for(auto script2 : scripts) {
        if(script1 != script2) {
          script1->addObserver(script2);
          script2->addObserver(script1);
        }
      }
    }
  }

  void ScriptingSystem::save(const std::string& filename) {
    std::ofstream file(filename);
    Json::Value save_file;

    for(auto script : scripts) {
      if(script->shouldBeSerialized()) {
        save_file[script->getClassName()] = script->getSaveData();
      }
    }

    file << save_file;

    file.close();
  }

  void ScriptingSystem::load(const std::string& filename) {
    std::ifstream file(filename);
    LOG(INFO)<<"Loading save file: "<<filename;
    if(file.good()) {
      Json::Value save_file;

      file >> save_file;
      for(auto &script : scripts) {
        if(script->shouldBeSerialized()) {
          script->loadSavedData(save_file[script->getClassName()]);
        }
      }
      file.close();
      LOG(INFO)<<"Save file load successful!";
    }
    else {
      LOG(INFO)<<"Save file not loaded!";
    }
  }

  void ScriptingSystem::start() {
    ChaiscriptWrapper::getInstance()->add(chaiscript::bootstrap::standard_library::map_type<std::map<std::shared_ptr<Game::Scene>, bool>>("SceneMap"));
    ChaiscriptWrapper::getInstance()->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::string>>("StringVector"));
    for(auto script : scripts) {
      LOG(INFO)<<script->getClassName()<<".onStart()";
      try {
        script->onStart();
      }
      catch (const chaiscript::exception::eval_error &ee) {
        LOG(INFO)<<ee.pretty_print();
      }
    }

  }

  void ScriptingSystem::update(const float delta) {
    for(auto script : scripts) {
      LOG(INFO)<<script->getClassName()<<".onUpdate()";
      try {
        script->onUpdate(delta);
      } catch(chaiscript::exception::eval_error e) {
        LOG(INFO)<<e.pretty_print();
      }
    }
  }

  void ScriptingSystem::destroy() {
    for(auto script : scripts) {
      LOG(INFO)<<script->getClassName()<<".onDestroy()";
      try {
        script->onDestroy();
      } catch(chaiscript::exception::eval_error e) {
        LOG(INFO)<<e.pretty_print();
      }
    }
    ChaiscriptWrapper::destroyInstance();
  }
}
