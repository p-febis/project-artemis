#include "Bowstring/Mesh.h"
#include "Bowstring/Logging.h"
#include "Bowstring/Renderer.h"
#include "Bowstring/pch.hpp"

bowstring::Mesh::Mesh(Renderer &renderer, std::vector<Vertex> vertices)
    : m_Renderer(renderer), m_Vertices(vertices) {
  VmaAllocator allocator = this->m_Renderer.getAllocator();
  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.size = sizeof(m_Vertices[0]) * vertices.size();
  bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
  bufferInfo.sharingMode = vk::SharingMode::eExclusive;

  VmaAllocationCreateInfo vmaAllocationInfo{};
  vmaAllocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
  vmaAllocationInfo.flags =
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  vmaCreateBuffer(allocator,
                  reinterpret_cast<VkBufferCreateInfo *>(&bufferInfo),
                  &vmaAllocationInfo,
                  reinterpret_cast<VkBuffer *>(&this->m_VertexBuffer.buffer),
                  &this->m_VertexBuffer.allocation, &this->m_VertexBuffer.info);

  vmaCopyMemoryToAllocation(allocator, this->m_Vertices.data(),
                            this->m_VertexBuffer.allocation, 0,
                            bufferInfo.size);
};

void bowstring::Mesh::bindBuffers(vk::CommandBuffer commandBuffer) {
  vk::Buffer vertexBuffers[] = {this->m_VertexBuffer.buffer};
  vk::DeviceSize offsets[] = {0};
  commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
}

void bowstring::Mesh::render(vk::CommandBuffer commandBuffer) {
  commandBuffer.draw(3, 1, 0, 0);
}
bowstring::Mesh::~Mesh() {
  vmaDestroyBuffer(this->m_Renderer.getAllocator(), this->m_VertexBuffer.buffer,
                   this->m_VertexBuffer.allocation);
}
