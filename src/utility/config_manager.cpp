#include <fstream>
#include <easylogging++.h>
#include "config_manager.h"
#include "exceptions/config_not_loaded_exception.h"

namespace Utility {
  ConfigManager::ConfigManager() : config_handle(Json::ValueType::objectValue) {

  }

  const bool ConfigManager::loadConfig(const std::string& file_path) {
    LOG(INFO)<<"Loading config: "<<file_path;
    std::ifstream config_doc(file_path.c_str());
    if(!config_doc.is_open())
      return false;
    
    config_handle.clear();
    config_doc >> config_handle;
    return true;
  }

  const int ConfigManager::getInt(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return (int)config_handle[key].asInt();
  }

  const unsigned int ConfigManager::getUnsignedInt(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return (unsigned int)config_handle[key].asUInt();
  }

  const std::string ConfigManager::getString(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asString();
  }

  const float ConfigManager::getFloat(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asFloat();
  }

  const double ConfigManager::getDouble(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asDouble();
  }

  const bool ConfigManager::getBool(const std::string& key) {
    if(!config_handle)
      throw Exceptions::ConfigNotLoadedException();
    return config_handle[key].asBool();
  }
}