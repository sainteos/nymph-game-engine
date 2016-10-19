#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include <json/json.h>
#include <vector>

//= SCRIPTABLE
namespace Utility {
  class ConfigManager {
    private:
      Json::Value config_handle;
    public:
      ConfigManager();
      
      const bool loadConfig(const std::string& file_path);
      //= BEGIN SCRIPTABLE
      const int getInt(const std::string& key);
      const unsigned int getUnsignedInt(const std::string& key);
      const std::string getString(const std::string& key);
      const float getFloat(const std::string& key);
      const double getDouble(const std::string& key);
      const bool getBool(const std::string& key);
      const std::vector<std::string> getStringVector(const std::string& key);
      const std::vector<unsigned int> getUnsignedIntVector(const std::string& key);
      //= END SCRIPTABLE
  };
}

#endif
