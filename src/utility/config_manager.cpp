#include <fstream>
#include <easylogging++.h>
#include "config_manager.h"
#include "exceptions/config_not_loaded_exception.h"

namespace Utility {
  ConfigManager::ConfigManager() : config_handle(Json::ValueType::objectValue) {

  }

  bool ConfigManager::loadConfig(const std::string& file_path) {
    LOG(INFO)<<"Loading config: "<<file_path;
    std::ifstream config_doc(file_path.c_str());
    if(!config_doc.is_open())
      return false;

    config_handle.clear();
    config_doc >> config_handle;
    return true;
  }

  std::vector<std::string> ConfigManager::getStringVector(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    std::vector<std::string> strings;

    if(config_handle[key].isArray()) {
      for(unsigned int i = 0; i < config_handle[key].size(); i++) {
        strings.push_back(config_handle[key][i].asString());
      }
    }
    return strings;
  }

  std::vector<unsigned int> ConfigManager::getUnsignedIntVector(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();

    std::vector<unsigned int> uints;

    if(config_handle[key].isArray()) {
      for(unsigned int i = 0; i < config_handle[key].size(); i++) {
        uints.push_back(config_handle[key][i].asUInt());
      }
    }

    return uints;
  }

  int ConfigManager::getInt(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return (int)config_handle[key].asInt();
  }

  unsigned int ConfigManager::getUnsignedInt(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return (unsigned int)config_handle[key].asUInt();
  }

  std::string ConfigManager::getString(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asString();
  }

  float ConfigManager::getFloat(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asFloat();
  }

  double ConfigManager::getDouble(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asDouble();
  }

  bool ConfigManager::getBool(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asBool();
  }
}
