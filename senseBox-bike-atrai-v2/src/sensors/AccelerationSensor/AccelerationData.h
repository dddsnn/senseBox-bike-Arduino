#ifndef ACCELERATION_DATA_H
#define ACCELERATION_DATA_H

#include <cstddef>
#include <cstdint>
#include <vector>

class AccelerationBuffer
{
public:
  AccelerationBuffer();
  void append(float z);
  std::size_t size() const;
  std::uint8_t *pop();

private:
  std::vector<float> buffers[2];
  uint_fast8_t activeBufferIndex;
  std::vector<float> &activeBuffer();
  std::vector<float> const &activeBufferConst() const;
  void rotateBuffer();
};

#endif // ACCELERATION_DATA_H
