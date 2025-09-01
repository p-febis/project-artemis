#pragma once

#include "Bowstring/pch.hpp"
#include "Mesh.h"
#include "Renderer.h"
#include "Window.h"
#include <cstdint>
#include <optional>

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
  std::optional<bowstring::Mesh> m_Mesh{};

public:
  Application(ApplicationConfig &config);
  void setClearColor(glm::vec4 clearColor);
  bowstring::Mesh &createMesh(bowstring::MeshType type,
                              std::vector<Vertex> vertices);
  void run();

  virtual void onInit();
  virtual void onUpdate(float deltaTime);
};
} // namespace bowstring
