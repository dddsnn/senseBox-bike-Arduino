#include "unity.h"

#include <sensors/AccelerationSensor/AccelerationData.h>

void setUp(void) {}

void tearDown(void) {}

std::uint32_t extractUint32(std::uint8_t *buf)
{
    std::uint32_t result = 0;
    result |= (*buf << 24);
    result |= (*(buf + 1) << 16);
    result |= (*(buf + 2) << 8);
    result |= (*(buf + 3));
    return result;
}

void test_bufferAppendAndPopSingleValue(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    std::uint8_t *buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf));
    TEST_ASSERT_EQUAL_FLOAT(1.5, *(reinterpret_cast<float *>(buf + 4)));
}

void test_bufferAppendAndPopUsesBigEndian(void)
{
    AccelerationBuffer buffer;
    unsigned long millis = (1 << 5) + (1 << 12) + (1 << 18) + (1 << 28);
    buffer.append(millis, 1.5);
    std::uint8_t *buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(millis, extractUint32(buf));
}

void test_bufferAppendAndPopMultipleValues(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(101, 2.0);
    buffer.append(110, 3.25);
    std::uint8_t *buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf));
    TEST_ASSERT_EQUAL_FLOAT(1.5, *(reinterpret_cast<float *>(buf + 4)));
    TEST_ASSERT_EQUAL_UINT8(1, *(buf + 8));
    TEST_ASSERT_EQUAL_FLOAT(2.0, *(reinterpret_cast<float *>(buf + 9)));
    TEST_ASSERT_EQUAL_UINT8(9, *(buf + 13));
    TEST_ASSERT_EQUAL_FLOAT(3.25, *(reinterpret_cast<float *>(buf + 14)));
}

void test_bufferTracksSize(void)
{
    AccelerationBuffer buffer;
    TEST_ASSERT_EQUAL_size_t(0, buffer.size());
    buffer.append(100, 1.5);
    TEST_ASSERT_EQUAL_size_t(8, buffer.size());
    buffer.append(101, 2.0);
    TEST_ASSERT_EQUAL_size_t(13, buffer.size());
    buffer.append(110, 3.25);
    TEST_ASSERT_EQUAL_size_t(18, buffer.size());
}

void test_bufferPredictsNextSize(void)
{
    AccelerationBuffer buffer;
    TEST_ASSERT_EQUAL_size_t(8, buffer.nextSize());
    buffer.append(100, 1.5);
    TEST_ASSERT_EQUAL_size_t(13, buffer.nextSize());
    buffer.append(101, 2.0);
    TEST_ASSERT_EQUAL_size_t(18, buffer.nextSize());
    buffer.append(110, 3.25);
    TEST_ASSERT_EQUAL_size_t(23, buffer.nextSize());
}

void test_bufferResetsSizeWithPop(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(101, 2.0);
    buffer.pop();
    TEST_ASSERT_EQUAL_size_t(0, buffer.size());
}
void test_bufferResetsContentWithPop(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(101, 2.0);
    buffer.pop();
    buffer.append(110, 3.25);
    std::uint8_t *buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(110, extractUint32(buf));
    TEST_ASSERT_EQUAL_FLOAT(3.25, *(reinterpret_cast<float *>(buf + 4)));
}

void test_bufferLeavesOldBufferValidWhileAppendingToNewOne(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(101, 2.0);
    std::uint8_t *oldBuf = buffer.pop();
    buffer.append(110, 3.25);
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(oldBuf));
    TEST_ASSERT_EQUAL_FLOAT(1.5, *(reinterpret_cast<float *>(oldBuf + 4)));
    TEST_ASSERT_EQUAL_UINT8(1, *(oldBuf + 8));
    TEST_ASSERT_EQUAL_FLOAT(2.0, *(reinterpret_cast<float *>(oldBuf + 9)));
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_bufferAppendAndPopSingleValue);
    RUN_TEST(test_bufferAppendAndPopUsesBigEndian);
    RUN_TEST(test_bufferAppendAndPopMultipleValues);
    RUN_TEST(test_bufferTracksSize);
    RUN_TEST(test_bufferPredictsNextSize);
    RUN_TEST(test_bufferResetsSizeWithPop);
    RUN_TEST(test_bufferResetsContentWithPop);
    RUN_TEST(test_bufferLeavesOldBufferValidWhileAppendingToNewOne);
    return UNITY_END();
}

int main(void)
{
    return runUnityTests();
}
