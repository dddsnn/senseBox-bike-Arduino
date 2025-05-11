#include "unity.h"

#include <sensors/AccelerationSensor/AccelerationData.h>

void setUp(void) {}

void tearDown(void) {}

void test_bufferAppendAndPopSingleValue(void)
{
    AccelerationBuffer buffer;
    buffer.append(1.5);
    std::uint8_t *buf = buffer.pop();
    float actual = *(reinterpret_cast<float *>(buf));
    TEST_ASSERT_EQUAL_FLOAT(1.5, actual);
}

void test_bufferAppendAndPopMultipleValues(void)
{
    AccelerationBuffer buffer;
    buffer.append(1.5);
    buffer.append(2.0);
    buffer.append(3.25);
    std::uint8_t *buf = buffer.pop();
    float *floatBuf = reinterpret_cast<float *>(buf);
    float actual = *(reinterpret_cast<float *>(buf));
    TEST_ASSERT_EQUAL_FLOAT(1.5, *(floatBuf));
    TEST_ASSERT_EQUAL_FLOAT(2.0, *(floatBuf + 1));
    TEST_ASSERT_EQUAL_FLOAT(3.25, *(floatBuf + 2));
}

void test_bufferTracksSize(void)
{
    AccelerationBuffer buffer;
    TEST_ASSERT_EQUAL_size_t(0, buffer.size());
    buffer.append(1.5);
    TEST_ASSERT_EQUAL_size_t(1, buffer.size());
    buffer.append(2.0);
    TEST_ASSERT_EQUAL_size_t(2, buffer.size());
    buffer.append(3.25);
    TEST_ASSERT_EQUAL_size_t(3, buffer.size());
}

void test_bufferResetsSizeWithPop(void)
{
    AccelerationBuffer buffer;
    buffer.append(1.5);
    buffer.append(2.0);
    buffer.pop();
    TEST_ASSERT_EQUAL_size_t(0, buffer.size());
}

void test_bufferResetsContentWithPop(void)
{
    AccelerationBuffer buffer;
    buffer.append(1.5);
    buffer.append(2.0);
    buffer.pop();
    buffer.append(3.25);
    TEST_ASSERT_EQUAL_FLOAT(3.25, *(reinterpret_cast<float *>(buffer.pop())));
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_bufferAppendAndPopSingleValue);
    RUN_TEST(test_bufferAppendAndPopMultipleValues);
    RUN_TEST(test_bufferTracksSize);
    RUN_TEST(test_bufferResetsSizeWithPop);
    RUN_TEST(test_bufferResetsContentWithPop);
    return UNITY_END();
}

int main(void)
{
    return runUnityTests();
}
