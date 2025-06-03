#ifndef ACCELERATION_DATA_H
#define ACCELERATION_DATA_H

#include <cstddef>
#include <cstdint>
#include <vector>

/**
 * @brief Buffer for acceleration data.
 *
 * Stores acceleration timeseries data, i.e. data points consisting of a time
 * and a value. The buffer can be popped to be written, at which point a new one
 * is started.
 */
class AccelerationBuffer
{
public:
  AccelerationBuffer();
  /**
   * @brief Append to the buffer.
   *
   * @param millis A millisecond timestamp.
   * @param z A value.
   */
  void append(unsigned long millis, float z);
  /**
   * @return The current size of the buffer, in bytes.
   */
  std::size_t size() const;
  /**
   * @brief Predict the maximum possible next size.
   *
   * The actual size of the buffer may be less, but not more than this value.
   *
   * @return The maximum size of the buffer after the next append(), in bytes.
   */
  std::size_t maxNextSize() const;
  /**
   * @brief Pop the active buffer.
   *
   * Returns the current buffer's bytes. Resets the internal state and switches
   * to a new buffer that can be used immediately.
   *
   * The returned byte buffer is a std::vector, where data() is a pointer to the
   * underlying std::uint8_t buffer, and size() returns the size of that buffer.
   * The buffer remains valid until the next call to pop.
   *
   * The buffer contains tightly-packed pairs of timestamp and value. The first
   * of these is a uint32 in network byte order containing the timestamp
   * followed by a 4 byte float containing the value.
   *
   * In every following one, the usual case is that the timestamp is at least
   * one but at most 255 greater than the preceding one. In this case, the
   * timestamp is encoded as a single uint8 containing the difference to the
   * previous timestamp, followed by the value (i.e. a total of 5 bytes).
   * Otherwise (the timestamp is less than or equal to the preceding one, or at
   * least 256 greater than it), the timestamp is encoded as a uint8 containing
   * 0, followed by a uint32 containing the full timestamp, followed by the
   * value (i.e. a total of 9 bytes).
   *
   * @return A std::vector containing bytes of accumulated data.
   */
  std::vector<std::uint8_t> const &pop();

private:
  std::vector<std::uint8_t> buffers[2];
  uint_fast8_t activeBufferIndex;
  unsigned long lastTime;
  std::vector<std::uint8_t> &activeBuffer();
  std::vector<std::uint8_t> const &activeBufferConst() const;
  static void pushUint32(std::vector<std::uint8_t> &buf, std::uint32_t x);
  static void pushFloat(std::vector<std::uint8_t> &buf, float x);
};

#endif // ACCELERATION_DATA_H
