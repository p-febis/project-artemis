#pragma once

#include "Bowstring/AllocatedData.h"
#include "Bowstring/Renderer.h"
#include "Bowstring/Vertex.h"
#include "vk_mem_alloc.h"
#include <vector>

namespace bowstring {

class Mesh {
private:
  Renderer &m_Renderer;
  std::vector<Vertex> m_Vertices;
  std::vector<uint32_t> m_Indices;
  AllocatedBuffer m_VertexBuffer;
  AllocatedBuffer m_IndexBuffer;

  void createVertexBuffer();
  void createIndexBuffer();

public:
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;
  Mesh(Mesh &&) = default;
  Mesh &operator=(Mesh &&) = delete;

  Mesh(Renderer &renderer, const std::vector<Vertex> &vertices);
  Mesh(Renderer &renderer, const std::vector<Vertex> &vertices,
       const std::vector<uint32_t>& indices);
  void bindBuffers(vk::CommandBuffer commandBuffer);
  void render(vk::CommandBuffer commandBuffer);
  ~Mesh();
};
} // namespace bowstring
