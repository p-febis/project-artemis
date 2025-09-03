#pragma once

#include "VMA.h"
#include "pch.hpp"

namespace bowstring {
struct AllocatedBuffer {
  vk::Buffer buffer;
  VmaAllocation allocation;
  VmaAllocationInfo info;
};

struct AllocatedImage {
  vk::Image image;
  VmaAllocation allocation;
  VmaAllocationInfo info;
};

} // namespace bowstring
