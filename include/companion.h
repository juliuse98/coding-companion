#pragma once

#include "ccobject.h"
namespace coco
{

class Companion : public CCObject
{
  public:
    Companion() {};
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    const std::string& GetName()
    {
        return m_name;
    }

  private:
    std::string m_name;
};

} // namespace coco
