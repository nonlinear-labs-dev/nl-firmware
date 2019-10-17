#include "Element.h"

namespace DescriptiveLayouts
{

  Element::Element(Element* parent, const std::string& n)
      : parent(parent)
      , name(n)
  {
  }

  const Element& Element::findChild(const Strings& path) const
  {
    if(path.empty())
      return *this;

    for(auto& c : children)
      if(c.name == path.front())
        return c.findChild(Strings(std::next(path.begin()), path.end()));

    throw std::runtime_error("Reference not found");
  }

  const Element& Element::findReference(const Strings& path) const
  {
    if(name == path.front())
      return findChild(Strings(std::next(path.begin()), path.end()));

    for(auto& c : children)
      if(c.name == path.front())
        return c.findChild(Strings(std::next(path.begin()), path.end()));

    if(!parent)
      throw std::runtime_error("Reference not found");

    return parent->findReference(path);
  }

  const Element& Element::findRelativeElement() const
  {
    Strings path;
    boost::split(path, refersTo, boost::is_any_of("/"));
    return findReference(path);
  }
}
