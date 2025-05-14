#ifndef ACCELERATION_DATA_H
#define ACCELERATION_DATA_H

#include <cstddef>
#include <cstdint>
#include <vector>

class AccelerationBuffer
{
public:
  AccelerationBuffer();
  void append(unsigned long millis, float z);
  std::size_t size() const;
  std::uint8_t *pop();

private:
  std::vector<std::uint8_t> buffers[2];
  uint_fast8_t activeBufferIndex;
  bool fullTimeWritten;
  unsigned long lastTime;
  std::vector<std::uint8_t> &activeBuffer();
  std::vector<std::uint8_t> const &activeBufferConst() const;
  static void pushUint32(std::vector<std::uint8_t> &buf, std::uint32_t x);
  static void pushFloat(std::vector<std::uint8_t> &buf, float x);
};

#endif // ACCELERATION_DATA_H
