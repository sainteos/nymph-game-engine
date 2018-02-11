#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include <json/json.h>
#include <vector>

namespace Utility {
  /**
   * @brief      Class for configuration manager to load json configurations
   */
  class [[scriptable]] ConfigManager {
    private:
      Json::Value config_handle;
    public:
      /**
       * @brief      Constructs the configManager
       */
      ConfigManager();

      /**
       * @brief      Loads a configuration.
       *
       * @param[in]  file_path  The file path
       *
       * @return     True if successful
       */
      bool loadConfig(const std::string& file_path);

      /**
       * @brief      Gets an int from config.
       *
       * @param[in]  key   The key
       *
       * @return     The int.
       */
      [[scriptable]] int getInt(const std::string& key);
      /**
       * @brief      Gets an unsigned int from config
       *
       * @param[in]  key   The key
       *
       * @return     The unsigned int.
       */
      [[scriptable]] unsigned int getUnsignedInt(const std::string& key);
      /**
       * @brief      Gets a string from config
       *
       * @param[in]  key   The key
       *
       * @return     The string.
       */
      [[scriptable]] std::string getString(const std::string& key);
      /**
       * @brief      Gets a float from config
       *
       * @param[in]  key   The key
       *
       * @return     The float.
       */
      [[scriptable]] float getFloat(const std::string& key);
      /**
       * @brief      Gets a double from config
       *
       * @param[in]  key   The key
       *
       * @return     The double.
       */
      [[scriptable]] double getDouble(const std::string& key);
      /**
       * @brief      Gets a bool from config
       *
       * @param[in]  key   The key
       *
       * @return     The bool.
       */
      [[scriptable]] bool getBool(const std::string& key);
      /**
       * @brief      Gets a string vector from config
       *
       * @param[in]  key   The key
       *
       * @return     The string vector.
       */
      [[scriptable]] std::vector<std::string> getStringVector(const std::string& key);
      /**
       * @brief      Gets an unsigned int vector from config
       *
       * @param[in]  key   The key
       *
       * @return     The unsigned int vector.
       */
      [[scriptable]] std::vector<unsigned int> getUnsignedIntVector(const std::string& key);
  };
}

#endif
