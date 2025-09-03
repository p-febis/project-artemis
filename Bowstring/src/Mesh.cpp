#include "Bowstring/Mesh.h"
#include "Bowstring/Logging.h"
#include "Bowstring/Renderer.h"

bowstring::Mesh::Mesh(Renderer &renderer, const std::vector<Vertex> &vertices)
    : m_Renderer(renderer), m_Vertices(vertices) {

  BS_LOG_DEBUG("[Mesh] vertices={} (bytes={}) allocator={}", m_Vertices.size(),
               m_Vertices.size() * sizeof(Vertex),
               (void *)m_Renderer.getAllocator());

  this->createVertexBuffer();
}

bowstring::Mesh::Mesh(Renderer &renderer, const std::vector<Vertex> &vertices,
                      const std::vector<uint32_t> &indices)
    : m_Renderer(renderer), m_Vertices(vertices), m_Indices(indices) {

  BS_LOG_DEBUG(
      "[Mesh] vertices={} (bytes={}) indices={} (bytes={}) allocator={}",
      m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), m_Indices.size(),
      m_Indices.size() * sizeof(uint32_t), (void *)m_Renderer.getAllocator());

  this->createVertexBuffer();
  this->createIndexBuffer();
}

void bowstring::Mesh::createVertexBuffer() {
  VmaAllocator allocator = this->m_Renderer.getAllocator();

  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.size = sizeof(this->m_Vertices[0]) * this->m_Vertices.size();
  bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
  bufferInfo.sharingMode = vk::SharingMode::eExclusive;

  VmaAllocationCreateInfo vmaAllocationInfo{};
  vmaAllocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
  vmaAllocationInfo.flags =
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  BS_LOG_DEBUG(
      "[Mesh::createVertexBuffer] size={} usage=VertexBuffer host-write",
      (uint64_t)bufferInfo.size);

  VkResult result = vmaCreateBuffer(
      allocator, reinterpret_cast<const VkBufferCreateInfo *>(&bufferInfo),
      &vmaAllocationInfo,
      reinterpret_cast<VkBuffer *>(&this->m_VertexBuffer.buffer),
      &this->m_VertexBuffer.allocation, &this->m_VertexBuffer.info);

  if (result != VK_SUCCESS) {
    BS_LOG_ERROR("[Mesh::createVertexBuffer] vmaCreateBuffer failed: {}",
                 (int)result);
    throw std::runtime_error("vmaCreateBuffer (vertex) failed");
  }

  BS_LOG_DEBUG(
      "[Mesh::createVertexBuffer] buffer={} alloc={} memoryType={} size={}",
      (void *)static_cast<VkBuffer>(this->m_VertexBuffer.buffer),
      (void *)this->m_VertexBuffer.allocation,
      this->m_VertexBuffer.info.memoryType,
      (uint64_t)this->m_VertexBuffer.info.size);

  result = vmaCopyMemoryToAllocation(allocator, this->m_Vertices.data(),
                                     this->m_VertexBuffer.allocation, 0,
                                     bufferInfo.size);

  if (result != VK_SUCCESS) {
    BS_LOG_ERROR(
        "[Mesh::createVertexBuffer] vmaCopyMemoryToAllocation failed: {}",
        (int)result);
    throw std::runtime_error("vmaCopyMemoryToAllocation (vertex) failed");
  }

  BS_LOG_DEBUG("[Mesh::createVertexBuffer] uploaded {} vertices ({} bytes)",
               m_Vertices.size(), (uint64_t)bufferInfo.size);
}

void bowstring::Mesh::createIndexBuffer() {
  VmaAllocator allocator = this->m_Renderer.getAllocator();

  vk::BufferCreateInfo bufferInfo{};
  bufferInfo.size = sizeof(this->m_Indices[0]) * this->m_Indices.size();
  bufferInfo.usage = vk::BufferUsageFlagBits::eIndexBuffer;
  bufferInfo.sharingMode = vk::SharingMode::eExclusive;

  VmaAllocationCreateInfo vmaAllocationInfo{};
  vmaAllocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
  vmaAllocationInfo.flags =
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  BS_LOG_DEBUG(
      "[Mesh::createIndexBuffer] count={} size={} usage=IndexBuffer host-write",
      m_Indices.size(), (uint64_t)bufferInfo.size);

  VkResult result = vmaCreateBuffer(
      allocator, reinterpret_cast<const VkBufferCreateInfo *>(&bufferInfo),
      &vmaAllocationInfo,
      reinterpret_cast<VkBuffer *>(&this->m_IndexBuffer.buffer),
      &this->m_IndexBuffer.allocation, &this->m_IndexBuffer.info);

  if (result != VK_SUCCESS) {
    BS_LOG_ERROR("[Mesh::createIndexBuffer] vmaCreateBuffer failed: {}",
                 (int)result);
    throw std::runtime_error("vmaCreateBuffer (index) failed");
  }

  BS_LOG_DEBUG(
      "[Mesh::createIndexBuffer] buffer={} alloc={} memoryType={} size={}",
      (void *)static_cast<VkBuffer>(this->m_IndexBuffer.buffer),
      (void *)this->m_IndexBuffer.allocation,
      this->m_IndexBuffer.info.memoryType,
      (uint64_t)this->m_IndexBuffer.info.size);

  result = vmaCopyMemoryToAllocation(allocator, this->m_Indices.data(),
                                     this->m_IndexBuffer.allocation, 0,
                                     bufferInfo.size);

  if (result != VK_SUCCESS) {
    BS_LOG_ERROR(
        "[Mesh::createIndexBuffer] vmaCopyMemoryToAllocation failed: {}",
        (int)result);
    throw std::runtime_error("vmaCopyMemoryToAllocation (index) failed");
  }

  BS_LOG_DEBUG("[Mesh::createIndexBuffer] uploaded {} indices ({} bytes)",
               m_Indices.size(), (uint64_t)bufferInfo.size);
}

void bowstring::Mesh::bindBuffers(vk::CommandBuffer commandBuffer) {
  vk::Buffer vertexBuffers[] = {this->m_VertexBuffer.buffer};
  vk::DeviceSize offsets[] = {0};
  commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

  if (this->m_IndexBuffer.buffer != VK_NULL_HANDLE) {
    commandBuffer.bindIndexBuffer(this->m_IndexBuffer.buffer, 0,
                                  vk::IndexType::eUint32);
  }
}

void bowstring::Mesh::render(vk::CommandBuffer commandBuffer) {
  if (this->m_IndexBuffer.buffer != VK_NULL_HANDLE) {
    commandBuffer.drawIndexed(static_cast<uint32_t>(this->m_Indices.size()), 1,
                              0, 0, 0);
    return;
  }

  commandBuffer.draw(static_cast<uint32_t>(m_Vertices.size()), 1, 0, 0);
}

bowstring::Mesh::~Mesh() {
  if (this->m_IndexBuffer.buffer != VK_NULL_HANDLE) {
    vmaDestroyBuffer(this->m_Renderer.getAllocator(),
                     this->m_IndexBuffer.buffer,
                     this->m_IndexBuffer.allocation);
  }

  vmaDestroyBuffer(this->m_Renderer.getAllocator(), this->m_VertexBuffer.buffer,
                   this->m_VertexBuffer.allocation);
}
