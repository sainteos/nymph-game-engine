#ifndef BASE_TEXTURE_H
#define BASE_TEXTURE_H
#include <memory>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <string>
//= SCRIPTABLE

namespace Graphics {
  /**
   * @brief      Class for base texture.
   */
  class BaseTexture {
    private:
      unsigned int texture_object;
      GLenum texture_type;
      bool loaded;
      unsigned int width;
      unsigned int height;
      std::string name;
    public:
      BaseTexture() = delete;
      //= BEGIN SCRIPTABLE

      /**
       * @brief      BaseTexture constructor
       *
       * @param[in]  texture_type  The open gl texture type
       */
      BaseTexture(const GLenum texture_type);
      /**
       * @brief      Destroys the object.
       */
      ~BaseTexture();

      /**
       * @brief      Gets the width.
       *
       * @return     The width.
       */
      unsigned int getWidth() const noexcept;
      /**
       * @brief      Gets the height.
       *
       * @return     The height.
       */
      unsigned int getHeight() const noexcept;

      /**
       * @brief      Loads the texture
       *
       * @param[in]  filename  The filename
       *
       * @return     True on success
       */
      virtual bool load(const std::string& filename);
      /**
       * @brief      Binds texture to opengl on the texture unit.
       *
       * @param[in]  texture_unit  The texture unit
       */
      virtual void bind(const unsigned int texture_unit);
      /**
       * @brief      Determines if loaded.
       *
       * @return     True if loaded, False otherwise.
       */
      virtual bool isLoaded() const noexcept;

      /**
       * @brief      Sets the name.
       *
       * @param[in]  name  The name
       */
      void setName(const std::string name) noexcept;
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      std::string getName() const noexcept;

      /**
       * @brief      Returns a string representation of the object.
       *
       * @return     String representation of the object.
       */
      std::string to_string() const noexcept;
      //= END SCRIPTABLE

      /**
       * @brief      Gets the texture object.
       *
       * @return     The texture object.
       */
      virtual unsigned int getTextureObject() const noexcept;
  };
}

#endif
