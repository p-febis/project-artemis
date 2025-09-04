#include "Bowstring/Mesh.h"
#include "Bowstring/AllocatedData.h"
#include "Bowstring/Logging.h"
#include "Bowstring/Renderer.h"

bowstring::Mesh::Mesh(Renderer *pRenderer, const std::vector<Vertex> &vertices)
    : m_pRenderer(pRenderer), m_Vertices(vertices) {

  BS_LOG_DEBUG("[Mesh] vertices={} (bytes={}) allocator={}", m_Vertices.size(),
               m_Vertices.size() * sizeof(Vertex),
               (void *)m_pRenderer->getAllocator());

  this->createVertexBuffer();
}

bowstring::Mesh::Mesh(Renderer *pRenderer, const std::vector<Vertex> &vertices,
                      const std::vector<uint32_t> &indices)
    : m_pRenderer(pRenderer), m_Vertices(vertices), m_Indices(indices) {

  BS_LOG_DEBUG(
      "[Mesh] vertices={} (bytes={}) indices={} (bytes={}) allocator={}",
      m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), m_Indices.size(),
      m_Indices.size() * sizeof(uint32_t), (void *)m_pRenderer->getAllocator());

  this->createVertexBuffer();
  this->createIndexBuffer();
}

void bowstring::Mesh::copyBuffer(vk::Buffer sourceBuffer,
                                 vk::Buffer destinationBuffer,
                                 vk::DeviceSize size) {
  vk::CommandBuffer commandBuffer = this->m_pRenderer->startOneTimeSubmit();

  vk::BufferCopy copyRegion{};
  copyRegion.size = size;

  commandBuffer.copyBuffer(sourceBuffer, destinationBuffer, 1, &copyRegion);

  this->m_pRenderer->endOneTimeSubmit(commandBuffer);
}
bowstring::AllocatedBuffer
bowstring::Mesh::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                              VmaAllocationCreateFlags allocationFlags) {

  bowstring::AllocatedBuffer createdBuffer;
  vk::BufferCreateInfo bufferCreateInfo{};
  bufferCreateInfo.size = size;
  bufferCreateInfo.usage = usage;

  VmaAllocationCreateInfo allocationCreateInfo = {};
  allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocationCreateInfo.flags = allocationFlags;

  vmaCreateBuffer(this->m_pRenderer->getAllocator(),
                  reinterpret_cast<VkBufferCreateInfo *>(&bufferCreateInfo),
                  &allocationCreateInfo,
                  reinterpret_cast<VkBuffer *>(&createdBuffer.buffer),
                  &createdBuffer.allocation, &createdBuffer.info);

  return createdBuffer;
};

void bowstring::Mesh::createVertexBuffer() {
  auto allocator = this->m_pRenderer->getAllocator();
  vk::DeviceSize bufferSize(sizeof(this->m_Vertices[0]) *
                            this->m_Vertices.size());

  auto stagingBuffer = this->createBuffer(
      bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
          VMA_ALLOCATION_CREATE_MAPPED_BIT);

  vmaCopyMemoryToAllocation(allocator, this->m_Vertices.data(),
                            stagingBuffer.allocation, 0, bufferSize);
  this->m_VertexBuffer =
      this->createBuffer(bufferSize,
                         vk::BufferUsageFlagBits::eTransferDst |
                             vk::BufferUsageFlagBits::eVertexBuffer,
                         VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

  this->copyBuffer(stagingBuffer.buffer, this->m_VertexBuffer.buffer,
                   bufferSize);

  vmaDestroyBuffer(allocator, stagingBuffer.buffer, stagingBuffer.allocation);
}

void bowstring::Mesh::createIndexBuffer() {
  VmaAllocator allocator = this->m_pRenderer->getAllocator();

  vk::DeviceSize bufferSize(sizeof(this->m_Indices[0]) *
                            this->m_Indices.size());

  auto stagingBuffer = this->createBuffer(
      bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
          VMA_ALLOCATION_CREATE_MAPPED_BIT);

  vmaCopyMemoryToAllocation(allocator, this->m_Indices.data(),
                            stagingBuffer.allocation, 0, bufferSize);

  this->m_IndexBuffer =
      this->createBuffer(bufferSize,
                         vk::BufferUsageFlagBits::eTransferDst |
                             vk::BufferUsageFlagBits::eIndexBuffer,
                         VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT);

  this->copyBuffer(stagingBuffer.buffer, this->m_IndexBuffer.buffer,
                   bufferSize);

  vmaDestroyBuffer(allocator, stagingBuffer.buffer, stagingBuffer.allocation);
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
    vmaDestroyBuffer(this->m_pRenderer->getAllocator(),
                     this->m_IndexBuffer.buffer,
                     this->m_IndexBuffer.allocation);
  }

  vmaDestroyBuffer(this->m_pRenderer->getAllocator(), this->m_VertexBuffer.buffer,
                   this->m_VertexBuffer.allocation);
}
