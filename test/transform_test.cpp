#include <easylogging++.h>
#include <catch.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext.hpp>
#include <list>
#include <algorithm>
#include "transform.h"
#include "exceptions/child_does_not_exist_exception.h"

SCENARIO("transform can have a child transform added to it") {
  GIVEN("Constructed transform with no children") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    WHEN("addChild is called") {
      transform->addChild(std::make_shared<Transform>());
      THEN("getTreeSize on the transform that had the child added should be 2") {
        REQUIRE(transform->getTreeSize() == 2);
      }
    }
  }

  GIVEN("Constructed transform with one children") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    transform->addChild(std::make_shared<Transform>());
    WHEN("addChild is called") {
      transform->addChild(std::make_shared<Transform>());
      THEN("getTreeSize on the transform that had the child added should be 3") {
        REQUIRE(transform->getTreeSize() == 3);
      }
    }
  }

  GIVEN("Constructed transform with 100 children") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    for(int i = 0; i < 100; i++)
      transform->addChild(std::make_shared<Transform>());
    WHEN("addChild is called") {
      transform->addChild(std::make_shared<Transform>());
      THEN("getTreeSize on the transform that had the child added should be 102") {
        REQUIRE(transform->getTreeSize() == 102);
      }
    }
  }
}

SCENARIO("transform with child transform will propogate transformations to children") {
  GIVEN("Constructed transform with one child") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    std::shared_ptr<Transform> child = std::make_shared<Transform>();

    transform->addChild(child);
    WHEN("translate is called") {
      transform->translate(glm::vec3(1.0, 0.0, 0.0));
      THEN("the child is also translated this amount") {
        REQUIRE(child->getAbsoluteTranslation() == glm::vec3(1.0, 0.0, 0.0));
      }
    }
    WHEN("rotate is called") {
      transform->rotate(57.0, glm::vec3(0.0, 1.0, 0.0));
      THEN("the child is also rotated this amount") {
        REQUIRE(child->getAbsoluteRotation() == glm::angleAxis(57.0f, glm::vec3(0.0, 1.0, 0.0)));
      }
    }
    WHEN("scale is called") {
      transform->scale(glm::vec3(1.0, 3.0, 1.0));
      THEN("the child is also scaled this amount") {
        REQUIRE(child->getAbsoluteScale() == glm::vec3(1.0, 3.0, 1.0));
      }
    }
  }
  GIVEN("Constructed transform with 100 children") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    std::list<std::shared_ptr<Transform>> children;
    for(int i = 0; i < 100; i++) {
      children.push_back(std::make_shared<Transform>());
      transform->addChild(children.back());
    }

    WHEN("translate is called") {
      transform->translate(glm::vec3(1.0, 0.0, 1.0));
      THEN("the children are also translated this amount") {
        for(auto& child : children) {
          REQUIRE(child->getAbsoluteTranslation() == glm::vec3(1.0, 0.0, 1.0));
        }
      }
    }
    WHEN("rotate is called") {
      transform->rotate(44.0, glm::vec3(1.0, 1.0, 0.0));
      THEN("the children are also rotated this amount") {
        for(auto& child : children) {
          REQUIRE(child->getAbsoluteRotation() == glm::angleAxis(44.0f, glm::vec3(1.0, 1.0, 0.0)));
        }
      }
    }
    WHEN("scale is called") {
      transform->scale(glm::vec3(1.0, 3.4, 0.3));
      THEN("the children are also scaled this amount") {
        for(auto& child : children) {
          REQUIRE(child->getAbsoluteScale() == glm::vec3(1.0, 3.4, 0.3));
        }
      }
    }
  }
}

SCENARIO("transform with child, when child is transformed, it doesnt propogate to parent") {
  GIVEN("Constructed transform with a parent") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    std::shared_ptr<Transform> child = std::make_shared<Transform>();

    transform->addChild(child);
    WHEN("translate is called on the child") {
      auto mat = transform->getAbsoluteTransformationMatrix();
      child->translate(glm::vec3(10.0, 0.0, -0.5));

      THEN("the parent's transform should not have changed") {
        REQUIRE(transform->getAbsoluteTransformationMatrix() == mat);
      }
    }
  }
}

SCENARIO("when destroy is called on transform, it is reset to its newly constructed state") {
  GIVEN("a Constructed transform") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    std::shared_ptr<Transform> transform2 = std::make_shared<Transform>();
    WHEN("destroy is called") {
      transform->destroy();
      THEN("it should equal a newly constructed transform") {
        REQUIRE(*transform == *transform2);
      }
    }
  }
  GIVEN("a constructed transform that has been translated, rotated, and scaled") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    std::shared_ptr<Transform> transform2 = std::make_shared<Transform>();
    transform->translate(glm::vec3(1.0, 0.0, 3.0));
    transform->rotate(90.0f, glm::vec3(0.0, 0.0, 1.0));
    transform->scale(glm::vec3(1.0, 0.4, 10.3));
    WHEN("destroy is called") {
      transform->destroy();
      THEN("it should equal a newly constructed transform") {
        REQUIRE(*transform == *transform2);
      }
    }
  }
}

SCENARIO("when destroy is called on transform that has a parent, the parent stays intact") {
  GIVEN("a constructed transform with a parent") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    std::shared_ptr<Transform> parent = std::make_shared<Transform>();
    parent->addChild(transform);
    WHEN("destroy is called") {
      transform->destroy();
      THEN("the parent should not equal nullptr") {
        REQUIRE(parent != nullptr);
      }
    }
    WHEN("getChildren is called") {
      transform->destroy();
      auto children = parent->getChildren();
      THEN("the destroyed transform should not appear in it") {
        auto count = std::count(children.begin(), children.end(), transform);
        REQUIRE(count == 0);
      }

      THEN("there shouldn't be nullptr in it") {
        auto count = std::count(children.begin(), children.end(), nullptr);
        REQUIRE(count == 0);
      }
    }
  }
}

SCENARIO("when destroy is called on a transform with children, the children are destroyed too") {
  GIVEN("a transform with children") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();

    std::vector<std::shared_ptr<Transform>> children;
    for(int i = 0; i < 20; i++) {
      children.push_back(std::make_shared<Transform>());
      transform->addChild(children.back());
    }

    WHEN("destroy is called") {
      transform->destroy();
      THEN("all of the children should equal to newly constructed transforms") {
        for(auto& child : children) {
          REQUIRE(*child == *std::make_shared<Transform>());
        }
      }
      THEN("getChildren should return an empty list") {
        REQUIRE(transform->getChildren().size() == 0);
      }
    }
  }
}

SCENARIO("when addChild is called, the child should gain the absolute transformation of the parent") {
  GIVEN("a transform with no children but rotated, translated, scaled") {
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    transform->scale(glm::vec3(0.5, 0.5, 0.5));
    transform->rotate(35., 42., -32.);
    transform->translate(glm::vec3(0.2, 1.0, 3.5));
    WHEN("addChild is called") {
      transform->addChild(std::make_shared<Transform>());
      THEN("the childs transform should have an absolute transformation the same as the parent's") {
        REQUIRE(transform->getAbsoluteTransformationMatrix() == transform->getChildren().back()->getAbsoluteTransformationMatrix());
      }
    }
  }
}
