#include "AccelerationData.h"

AccelerationBuffer::AccelerationBuffer() : activeBufferIndex(0), lastTime(0) {}

void AccelerationBuffer::append(unsigned long millis, float z)
{
    auto &buf = activeBuffer();
    auto timeDiff = millis - lastTime;
    if (buf.empty())
    {
        // First value, write full timestamp.
        pushUint32(buf, millis);
    }
    else if (timeDiff > 0 && timeDiff < 256)
    {
        // Difference to previous timestamp is 0 < diff < 256, write only the
        // difference in one byte.
        buf.push_back(millis - lastTime);
    }
    else
    {
        // Exceptional case: write a single zero byte to mark that the following
        // 4 bytes are a full timestamp again.
        buf.push_back(0);
        pushUint32(buf, millis);
    }
    lastTime = millis;
    pushFloat(buf, z);
}

std::size_t AccelerationBuffer::size() const
{
    return activeBufferConst().size();
}

std::size_t AccelerationBuffer::maxNextSize() const
{
    return activeBufferConst().empty() ? 8 : activeBufferConst().size() + 9;
}

std::vector<std::uint8_t> const &AccelerationBuffer::pop()
{
    auto &buf = activeBuffer();
    activeBufferIndex = (activeBufferIndex + 1) % 2;
    activeBuffer().clear();
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
