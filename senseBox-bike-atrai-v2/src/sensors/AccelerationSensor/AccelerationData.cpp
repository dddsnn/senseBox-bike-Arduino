#include "AccelerationData.h"

AccelerationBuffer::AccelerationBuffer() {}

void AccelerationBuffer::append(float z) { buf.push_back(z); }

std::size_t AccelerationBuffer::size()
{
    return 0;
}
/**
 * @brief Writes two float values to a characteristic.
 * #TODO++++++++++++++
 *
 * @param characteristic The ID of the characteristic to write to.
 * @param f1 The first float value to write.
 * @param f2 The second float value to write.
 * @return true if the write was successful, false otherwise.
 */
std::uint8_t *AccelerationBuffer::pop()
{
    return reinterpret_cast<std::uint8_t *>(buf.data());
}
