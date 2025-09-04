#pragma once

#include <flecs.h>

namespace bowstring {
class Entity {
private:
  flecs::entity m_Entity;

public:
  Entity(flecs::entity entity);

  template <typename T> void addComponent(T &&component) {
    this->m_Entity.set<std::decay_t<T>>(std::forward<T>(component));
  }
};
} // namespace bowstring
