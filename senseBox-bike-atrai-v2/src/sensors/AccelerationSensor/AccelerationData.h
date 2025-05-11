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
  std::size_t size();
  std::uint8_t *pop();

private:
  std::vector<float> buf;
};

#endif // ACCELERATION_DATA_H
