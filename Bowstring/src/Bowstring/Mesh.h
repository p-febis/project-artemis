#pragma once

#include "Bowstring/Renderer.h"
#include "Bowstring/Vertex.h"
#include "vk_mem_alloc.h"

namespace bowstring {

struct AllocatedBuffer {
  vk::Buffer buffer;
  VmaAllocation allocation;
  VmaAllocationInfo info;
};

class Mesh {
private:
  Renderer &m_Renderer;
  std::vector<Vertex> m_Vertices;
  AllocatedBuffer m_VertexBuffer;

public:
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;
  Mesh(Mesh &&) = default;
  Mesh &operator=(Mesh &&) = delete;

  Mesh(Renderer &renderer, std::vector<Vertex> vertices);
  void bindBuffers(vk::CommandBuffer commandBuffer);
  void render(vk::CommandBuffer commandBuffer);
  ~Mesh();
};
} // namespace bowstring
