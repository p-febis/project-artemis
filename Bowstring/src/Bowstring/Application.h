#pragma once

#include "Bowstring/Entity.h"
#include "Renderer.h"
#include "Window.h"
#include <cstdint>
#include <flecs.h>

namespace bowstring {
struct ApplicationConfig {
  uint32_t width = 0;
  uint32_t height = 0;
  const char *title;
};

class Application {
private:
  ApplicationConfig m_Config;
  Window m_Window;
  Renderer m_Renderer;
  flecs::world m_World;

public:
  Application(ApplicationConfig &config);
  Entity createEntity();
  void setClearColor(glm::vec4 clearColor);
  Renderer *getRenderer() { return &this->m_Renderer; };
  void run();

  virtual void onInit();
  virtual void onUpdate(float deltaTime);
};
} // namespace bowstring
