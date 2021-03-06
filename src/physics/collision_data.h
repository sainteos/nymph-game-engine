#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H
#include <easylogging++.h>
#include <vector>
#include "../component.h"

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
      int getCollideLevel(const int x, const int y);
      unsigned int getWidth() const noexcept;
      unsigned int getHeight() const noexcept;

      virtual void onStart() override;
      virtual bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      virtual unsigned long long getValueForSorting() const override;

      virtual std::string to_string() const noexcept override;
      virtual void log(el::base::type::ostream_t& os) const override;

  };
}
#endif
