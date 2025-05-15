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
   * @param millis A millisecond timestamp. Timestamps in subsequent calls must
   *     not decrease, and must not be more than 255 apart.
   * @param z A value.
   */
  void append(unsigned long millis, float z);
  /**
   * @return The current size of the buffer, in bytes.
   */
  std::size_t size() const;
  /**
   * @return The size the buffer will have after the next append(), in bytes.
   */
  std::size_t nextSize() const;
  /**
   * @brief Pop the active buffer.
   *
   * Returns the current buffer as a byte buffer. Resets the internal state and
   * switches to a new buffer that can be used immediately.
   *
   * The returned byte buffer remains valid until the next call to pop. The size
   * size of the returned byte buffer is indicated by an immediately preceding
   * call to size() (i.e. without an append() call in between). Immediately
   * after pop(), size() returns 0.
   *
   * The buffer contains tightly-packed pairs of timestamp and value. The first
   * of these is a uint32 in network byte order containing the timestamp
   * followed by a 4 byte float containing the value. In every following one,
   * the timestamp is encoded as a single uint8 containing the difference to the
   * previous timestamp, i.e. a total of 5 bytes including the value (hence also
   * the requirement that timestamps must not decrease and must be no more than
   * 255 apart).
   *
   * @return A pointer to a byte buffer containing accumulated data.
   */
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
