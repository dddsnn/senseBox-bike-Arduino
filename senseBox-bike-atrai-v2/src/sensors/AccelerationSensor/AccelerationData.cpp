#include "AccelerationData.h"

AccelerationBuffer::AccelerationBuffer()
    : activeBufferIndex(0), fullTimeWritten(false), lastTime(0) {}

void AccelerationBuffer::append(unsigned long millis, float z)
{
    auto &buf = activeBuffer();
    if (!fullTimeWritten)
    {
        pushUint32(buf, millis);
        fullTimeWritten = true;
    }
    else
    {
        buf.push_back(millis - lastTime);
    }
    lastTime = millis;
    pushFloat(buf, z);
}

std::size_t AccelerationBuffer::size() const
{
    return activeBufferConst().size();
}

std::size_t AccelerationBuffer::nextSize() const
{
    return activeBufferConst().empty() ? 8 : activeBufferConst().size() + 5;
}

std::vector<std::uint8_t> const &AccelerationBuffer::pop()
{
    auto &buf = activeBuffer();
    activeBufferIndex = (activeBufferIndex + 1) % 2;
    activeBuffer().clear();
    fullTimeWritten = false;
    return buf;
}

std::vector<std::uint8_t> &AccelerationBuffer::activeBuffer()
{
    return buffers[activeBufferIndex];
}

std::vector<std::uint8_t> const &AccelerationBuffer::activeBufferConst() const
{
    return (const_cast<AccelerationBuffer *>(this))->activeBuffer();
}

void AccelerationBuffer::pushUint32(
    std::vector<std::uint8_t> &buf, std::uint32_t x)
{
    buf.push_back(x >> 24);
    buf.push_back(x >> 16);
    buf.push_back(x >> 8);
    buf.push_back(x);
}

void AccelerationBuffer::pushFloat(std::vector<std::uint8_t> &buf, float x)
{
    std::uint8_t *bytes = reinterpret_cast<std::uint8_t *>(&x);
    buf.push_back(*bytes);
    buf.push_back(*(bytes + 1));
    buf.push_back(*(bytes + 2));
    buf.push_back(*(bytes + 3));
}
