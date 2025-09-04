#pragma once

#include "Bowstring/AllocatedData.h"
#include "Bowstring/Renderer.h"
#include "Bowstring/Vertex.h"
#include "vk_mem_alloc.h"
#include <memory>
#include <vector>

namespace bowstring {

struct MeshComponent {
  std::shared_ptr<Mesh> mesh;
};

class Mesh {
private:
  Renderer *m_pRenderer;
  std::vector<Vertex> m_Vertices;
  std::vector<uint32_t> m_Indices;
  AllocatedBuffer m_VertexBuffer;
  AllocatedBuffer m_IndexBuffer;

  bowstring::AllocatedBuffer
  createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
               VmaAllocationCreateFlags allocationFlags);

  void copyBuffer(vk::Buffer sourceBuffer, vk::Buffer destinationBuffer,
                  vk::DeviceSize size);
  void createVertexBuffer();
  void createIndexBuffer();

public:
  Mesh(Renderer *pRenderer, const std::vector<Vertex> &vertices);
  Mesh(Renderer *pRenderer, const std::vector<Vertex> &vertices,
       const std::vector<uint32_t> &indices);
  void bindBuffers(vk::CommandBuffer commandBuffer);
  void render(vk::CommandBuffer commandBuffer);
  ~Mesh();
};
} // namespace bowstring
