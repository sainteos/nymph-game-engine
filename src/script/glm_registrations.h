#ifndef GLM_REGISTRATIONS_H
#define GLM_REGISTRATIONS_H
#include <glm/glm.hpp>

namespace Script {
  chaiscript::ModulePtr getGlmModule() {
    chaiscript::ModulePtr module = std::make_shared<chaiscript::Module>();

    chaiscript::utility::add_class<glm::vec3>(*module, "vec2", {
       chaiscript::constructor<glm::vec2(const float, const float)>(),
       chaiscript::constructor<glm::vec2(const glm::vec2)>()
      }, 
      {{chaiscript::fun(&glm::vec2::x), "x"}, 
       {chaiscript::fun(&glm::vec2::y), "y"}, 
       {chaiscript::fun([](glm::vec2 a, glm::vec2 b) { return a + b; }), "+"}
      }
    );

    chaiscript::utility::add_class<glm::vec3>(*module, "vec3", {
       chaiscript::constructor<glm::vec3(const float, const float, const float)>(),
       chaiscript::constructor<glm::vec3(const glm::vec3)>()
      }, 
      {{chaiscript::fun(&glm::vec3::x), "x"}, 
       {chaiscript::fun(&glm::vec3::y), "y"}, 
       {chaiscript::fun(&glm::vec3::z), "z"},
       {chaiscript::fun([](glm::vec3 a, glm::vec3 b) { return a + b; }), "+"}
      }
    );

    chaiscript::utility::add_class<glm::vec4>(*module, "vec4", {
       chaiscript::constructor<glm::vec4(const float, const float, const float, const float)>(),
       chaiscript::constructor<glm::vec4(const glm::vec4)>()
      }, 
      {{chaiscript::fun(&glm::vec4::x), "x"}, 
       {chaiscript::fun(&glm::vec4::y), "y"}, 
       {chaiscript::fun(&glm::vec4::z), "z"},
       {chaiscript::fun(&glm::vec4::w), "w"},
       {chaiscript::fun([](glm::vec4 a, glm::vec4 b) { return a + b; }), "+"}
      }
    );

    return module;
  }
}
#endif
