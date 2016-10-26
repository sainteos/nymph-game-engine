#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H
#include <easylogging++.h>
#include <vector>
#include "component.h"

namespace Physics {
  class CollisionData : public Component, virtual public el::Loggable {
    private:  
      std::vector<std::vector<int>> collision_table;
      unsigned int width;
      unsigned int height;
    public:
      CollisionData() = delete;
      CollisionData(const unsigned int map_width, const unsigned int map_height);

      void setIndexCollidable(const unsigned int x, const unsigned int y, const unsigned int collide_level);
      const int getCollideLevel(const int x, const int y);
      const unsigned int getWidth() const noexcept;
      const unsigned int getHeight() const noexcept;

      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      virtual const unsigned long long getValueForSorting() const;
      
      const std::string to_string() const noexcept;
      virtual void log(el::base::type::ostream_t& os) const override;

  };
}
#endif
