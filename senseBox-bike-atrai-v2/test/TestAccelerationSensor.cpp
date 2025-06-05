#include "unity.h"

#include <sensors/AccelerationSensor/AccelerationData.h>

void setUp(void) {}

void tearDown(void) {}

std::uint32_t extractUint32(std::uint8_t const *buf)
{
    std::uint32_t result = 0;
    result |= (*buf << 24);
    result |= (*(buf + 1) << 16);
    result |= (*(buf + 2) << 8);
    result |= (*(buf + 3));
    return result;
}

float extractFloat32(std::uint8_t const *buf)
{
    std::uint32_t u = extractUint32(buf);
    return *(reinterpret_cast<float *>(&u));
}

void test_bufferAppendAndPopSingleValue(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf.data()));
    TEST_ASSERT_EQUAL_FLOAT(1.5, extractFloat32(buf.data() + 4));
}

void test_bufferAppendAndPopUsesBigEndianUint32s(void)
{
    AccelerationBuffer buffer;
    unsigned long millis = (1 << 5) + (1 << 12) + (1 << 18) + (1 << 28);
    buffer.append(millis, 1.5);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(millis, extractUint32(buf.data()));
}

void test_bufferAppendAndPopUsesBigEndianFloat32s(void)
{
    AccelerationBuffer buffer;
    float z = -7.750000476837158203125;
    buffer.append(100, z);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_FLOAT(z, extractFloat32(buf.data() + 4));
}

void test_bufferAppendAndPopMultipleValuesUsesShortFormatWithSmallDiffs(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(101, 2.0);
    buffer.append(356, 3.25);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf.data()));
    TEST_ASSERT_EQUAL_FLOAT(1.5, extractFloat32(buf.data() + 4));
    TEST_ASSERT_EQUAL_UINT8(1, *(buf.data() + 8));
    TEST_ASSERT_EQUAL_FLOAT(2.0, extractFloat32(buf.data() + 9));
    TEST_ASSERT_EQUAL_UINT8(255, *(buf.data() + 13));
    TEST_ASSERT_EQUAL_FLOAT(3.25, extractFloat32(buf.data() + 14));
}

void test_bufferAppendAndPopMultipleValuesUsesLongFormatWithLargerDiffs(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(356, 2.0);
    buffer.append(360, 3.25);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf.data()));
    TEST_ASSERT_EQUAL_FLOAT(1.5, extractFloat32(buf.data() + 4));
    TEST_ASSERT_EQUAL_UINT8(0, *(buf.data() + 8));
    TEST_ASSERT_EQUAL_UINT32(356, extractUint32(buf.data() + 9));
    TEST_ASSERT_EQUAL_FLOAT(2.0, extractFloat32(buf.data() + 13));
    TEST_ASSERT_EQUAL_UINT8(4, *(buf.data() + 17));
    TEST_ASSERT_EQUAL_FLOAT(3.25, extractFloat32(buf.data() + 18));
}

void test_bufferAppendAndPopMultipleValuesUsesLongFormatWithZeroDiffs(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(100, 2.0);
    buffer.append(112, 3.25);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf.data()));
    TEST_ASSERT_EQUAL_FLOAT(1.5, extractFloat32(buf.data() + 4));
    TEST_ASSERT_EQUAL_UINT8(0, *(buf.data() + 8));
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf.data() + 9));
    TEST_ASSERT_EQUAL_FLOAT(2.0, extractFloat32(buf.data() + 13));
    TEST_ASSERT_EQUAL_UINT8(12, *(buf.data() + 17));
    TEST_ASSERT_EQUAL_FLOAT(3.25, extractFloat32(buf.data() + 18));
}

void test_bufferAppendAndPopMultipleValuesUsesLongFormatWithNegativeDiffs(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(99, 2.0);
    buffer.append(112, 3.25);
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(buf.data()));
    TEST_ASSERT_EQUAL_FLOAT(1.5, extractFloat32(buf.data() + 4));
    TEST_ASSERT_EQUAL_UINT8(0, *(buf.data() + 8));
    TEST_ASSERT_EQUAL_UINT32(99, extractUint32(buf.data() + 9));
    TEST_ASSERT_EQUAL_FLOAT(2.0, extractFloat32(buf.data() + 13));
    TEST_ASSERT_EQUAL_UINT8(13, *(buf.data() + 17));
    TEST_ASSERT_EQUAL_FLOAT(3.25, extractFloat32(buf.data() + 18));
}

void test_bufferTracksSize(void)
{
    AccelerationBuffer buffer;
    TEST_ASSERT_EQUAL_size_t(0, buffer.size());
    buffer.append(100, 1.5);
    TEST_ASSERT_EQUAL_size_t(8, buffer.size());
    buffer.append(400, 2.0);
    TEST_ASSERT_EQUAL_size_t(17, buffer.size());
    buffer.append(410, 3.25);
    TEST_ASSERT_EQUAL_size_t(22, buffer.size());
}

void test_bufferPredictsNextSize(void)
{
    AccelerationBuffer buffer;
    TEST_ASSERT_EQUAL_size_t(8, buffer.maxNextSize());
    buffer.append(100, 1.5);
    TEST_ASSERT_EQUAL_size_t(17, buffer.maxNextSize());
    buffer.append(400, 2.0);
    TEST_ASSERT_EQUAL_size_t(26, buffer.maxNextSize());
    buffer.append(410, 3.25);
    TEST_ASSERT_EQUAL_size_t(31, buffer.maxNextSize());
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
    auto &buf = buffer.pop();
    TEST_ASSERT_EQUAL_UINT32(110, extractUint32(buf.data()));
    TEST_ASSERT_EQUAL_FLOAT(3.25, extractFloat32(buf.data() + 4));
}

void test_bufferLeavesOldBufferValidWhileAppendingToNewOne(void)
{
    AccelerationBuffer buffer;
    buffer.append(100, 1.5);
    buffer.append(101, 2.0);
    auto &oldBuf = buffer.pop();
    buffer.append(110, 3.25);
    TEST_ASSERT_EQUAL_UINT32(100, extractUint32(oldBuf.data()));
    TEST_ASSERT_EQUAL_FLOAT(1.5, extractFloat32(oldBuf.data() + 4));
    TEST_ASSERT_EQUAL_UINT8(1, *(oldBuf.data() + 8));
    TEST_ASSERT_EQUAL_FLOAT(2.0, extractFloat32(oldBuf.data() + 9));
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_bufferAppendAndPopSingleValue);
    RUN_TEST(test_bufferAppendAndPopUsesBigEndianUint32s);
    RUN_TEST(test_bufferAppendAndPopUsesBigEndianFloat32s);
    RUN_TEST(
        test_bufferAppendAndPopMultipleValuesUsesShortFormatWithSmallDiffs);
    RUN_TEST(
        test_bufferAppendAndPopMultipleValuesUsesLongFormatWithLargerDiffs);
    RUN_TEST(test_bufferAppendAndPopMultipleValuesUsesLongFormatWithZeroDiffs);
    RUN_TEST(
        test_bufferAppendAndPopMultipleValuesUsesLongFormatWithNegativeDiffs);
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
