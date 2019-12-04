#pragma once

template <typename tCollection>
inline ParameterId getIdOfAdvancedParameter(const tCollection &c, ParameterId currentSelection, int inc)
{
  auto found = find_if(c.begin(), c.end(), [&](auto e) { return e->getID() == currentSelection; });

  if(found == c.end())
    found = c.begin();

  auto dist = distance(c.begin(), found);
  dist += inc;

  while(dist >= (int) c.size())
    dist -= c.size();

  while(dist < 0)
    dist += c.size();

  found = c.begin();
  advance(found, dist);

  return (*found)->getID();
}
