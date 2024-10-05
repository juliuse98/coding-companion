#pragma once
#include <nano_signal_slot.hpp>
namespace coco
{
class TimeAwareObject
{
  protected:
    virtual void update(float deltaTime) = 0;

    // Define TimeController as a friend so it can call update
    friend class TimeController;
};
class TimeController
{
  private:
    Nano::Signal<void(float)> update;

  public:
    void addObject(TimeAwareObject& object)
    {
        update.connect<&TimeAwareObject::update>(object);
    }
    void Update(float deltaTime)
    {
        update.fire(deltaTime);
    }
};
} // namespace coco
