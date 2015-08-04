#include <easylogging++.h>
#include <catch.hpp>
#include <IL/il.h>
#include <memory>
#include "graphics/base_texture.h"
#include "graphics/base_sampler.h"
#include "exceptions/texture_not_loaded_exception.h"
#include "opengl_setup.h"

SCENARIO("a texture can be loaded with a valid filename") {
  setup_opengl();
  GIVEN("a constructed texture") {
    Graphics::BaseTexture texture("test", GL_TEXTURE_2D, 0);
    WHEN("load is called with a filename that's file exists") {
      auto success = texture.load("./test/textures/test1.png"); 
      THEN("it should return true") {
        REQUIRE(success == true);
      }
      THEN("is loaded should return true") {
        REQUIRE(texture.isLoaded() == true);
      }
      THEN("texture object should actually be a texture") {
        REQUIRE(glIsTexture(texture.getTextureObject()) == true);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("a texture cannot be loaded with an invalid filename") {
  setup_opengl();
  GIVEN("a constructed texture") {
    Graphics::BaseTexture texture("test", GL_TEXTURE_2D, 0);
    WHEN("load is called with a filename that's file doesn't exist") {
      auto success = texture.load("./test/textures/temt1.png"); 
      THEN("it should return false") {
        REQUIRE(success == false);
      }
      THEN("isLoaded should return false") {
        REQUIRE(texture.isLoaded() == false);
      }
      THEN("texture object should not be a texture") {
        REQUIRE(glIsTexture(texture.getTextureObject()) == false);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("a texture can be bound if it has been loaded") {
  setup_opengl();
  GIVEN("a loaded texture with no sampler") {
    Graphics::BaseTexture texture("test", GL_TEXTURE_2D, 0);
    texture.load("./test/textures/test1.png");
    WHEN("bind is called") {
      texture.bind();
      THEN("the currently bound texture should be the same id as the texture") {
        REQUIRE([]() {int tex1[1]; glGetIntegerv(GL_TEXTURE_BINDING_2D, tex1); return tex1[0];}() == texture.getTextureObject());
      }
    }
  }
  GIVEN("a loaded texture with a sampler") {
    Graphics::BaseTexture texture("test", GL_TEXTURE_2D, 0);
    texture.load("./test/textures/test1.png");
    std::shared_ptr<Graphics::BaseSampler> sampler = std::make_shared<Graphics::BaseSampler>();
    texture.setSampler(sampler);
    WHEN("bind is called") {
      texture.bind();
      THEN("the currently bound texture should be the same id as the texture") {
        REQUIRE([]() {int tex1[1]; glGetIntegerv(GL_TEXTURE_BINDING_2D, tex1); return tex1[0];}() == texture.getTextureObject());
      }
      THEN("the currently bound sampler should be the same id as the texture's sampler") {
        REQUIRE(texture.getSampler() != nullptr);
        REQUIRE([]() {int samp[1]; glGetIntegerv(GL_SAMPLER_BINDING, samp); return samp[0];}() == texture.getSampler()->getSamplerObject());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("texture cannot be bound if it has not been loaded") {
  setup_opengl();
  GIVEN("a constructed but not loaded texture") {
    Graphics::BaseTexture texture("test", GL_TEXTURE_2D, 0);
    texture.load("./test/textures/temt1.png");
    WHEN("bind is called") {
      THEN("it should throw a TextureNotLoaded exception") {
        REQUIRE_THROWS_AS(texture.bind(), Exceptions::TextureNotLoadedException);
      }
    }
  }
  destroy_opengl();
}