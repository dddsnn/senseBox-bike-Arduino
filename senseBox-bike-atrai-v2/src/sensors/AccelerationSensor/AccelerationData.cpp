#include "AccelerationData.h"

AccelerationBuffer::AccelerationBuffer() : activeBufferIndex(0) {}

void AccelerationBuffer::append(float z)
{
    activeBuffer().push_back(z);
}

std::size_t AccelerationBuffer::size() const
{
    return activeBufferConst().size();
}

std::uint8_t *AccelerationBuffer::pop()
{
    auto rawBuffer = reinterpret_cast<std::uint8_t *>(activeBuffer().data());
    rotateBuffer();
    activeBuffer().clear();
    return rawBuffer;
}

std::vector<float> &AccelerationBuffer::activeBuffer()
{
    return buffers[activeBufferIndex];
}

std::vector<float> const &AccelerationBuffer::activeBufferConst() const
{
    return (const_cast<AccelerationBuffer *>(this))->activeBuffer();
}

void AccelerationBuffer::rotateBuffer()
{
    activeBufferIndex = (activeBufferIndex + 1) % 2;
}
