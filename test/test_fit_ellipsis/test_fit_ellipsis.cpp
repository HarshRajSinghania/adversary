/**
 * @file test_fit_ellipsis.cpp
 * @brief Unit tests for the fixed-width ellipsis helper (issue #56).
 */

#include <unity.h>
#include <cstring>
#include "utils/fit_ellipsis.h"

using namespace adversary::utils;

void setUp() {}
void tearDown() {}

/** A short name is unchanged. */
void test_short_unchanged(void) {
    char out[32];
    fitEllipsis("MyNet", 20, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("MyNet", out);
}

/** A name exactly at maxChars is unchanged. */
void test_exact_fit_unchanged(void) {
    char out[32];
    fitEllipsis("1234567890", 10, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("1234567890", out);
}

/** A name one character over is truncated to maxChars total including the "...". */
void test_one_over_truncated(void) {
    char out[32];
    fitEllipsis("12345678901", 10, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("1234567...", out);
    TEST_ASSERT_EQUAL(10, strlen(out));
}

/** A long hash-like SSID is truncated correctly. */
void test_long_ssid(void) {
    char out[64];
    const char* longSsid = "7vbc68v6b8b6vbc876cvb6abcdef";
    fitEllipsis(longSsid, 20, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("7vbc68v6b8b6vbc87...", out);
    TEST_ASSERT_EQUAL(20, strlen(out));
}

/** maxChars smaller than the ellipsis itself degrades sanely (no overflow, NUL-terminated). */
void test_tiny_maxchars(void) {
    char out[8];
    fitEllipsis("abcdef", 2, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("ab", out);
    // Buffer remains NUL-terminated and not overflowing.
    TEST_ASSERT_EQUAL(0, out[2]);
}

/** Empty / null-ish inputs. */
void test_empty(void) {
    char out[8];
    fitEllipsis("", 5, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("", out);

    fitEllipsis(nullptr, 5, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("", out);

    fitEllipsis("hello", 0, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("", out);
}

/** Respects outLen when the truncated result would not fit. */
void test_outlen_limit(void) {
    char out[6];  // only 5 chars + NUL
    fitEllipsis("abcdefghij", 20, out, sizeof(out));
    // Should not overflow; result fits in 5 chars.
    TEST_ASSERT_TRUE(strlen(out) <= 5);
    TEST_ASSERT_EQUAL(0, out[5]);
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_short_unchanged);
    RUN_TEST(test_exact_fit_unchanged);
    RUN_TEST(test_one_over_truncated);
    RUN_TEST(test_long_ssid);
    RUN_TEST(test_tiny_maxchars);
    RUN_TEST(test_empty);
    RUN_TEST(test_outlen_limit);
    return UNITY_END();
}
