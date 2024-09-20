#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include "draw.h"
#include <stdlib.h>

#define ASSERT_EQUAL_FMT(actual, expected, format, message, ...) \
    do { \
        if ((actual) != (expected)) { \
            CU_FAIL(message); \
            printf(" Expected: " format ", ", expected, ##__VA_ARGS__); \
            printf("Actual: " format "\n", actual, ##__VA_ARGS__); \
        } \
    } while (0)

// Unit test function
void test_line(void) {

	// Generate expected output
	struct Point p1 = {
		.x = 0,
		.y = 0
	};
	struct Point p2 = {
		.x = 1,
		.y = 1
	};
	struct Point p3 = {
		.x = 2,
		.y = 2
	};
	struct Point p4 = {
		.x = 3,
		.y = 3
	};

	struct PointVector* output = draw_line_test(p1, p4);

	CU_ASSERT_PTR_NOT_NULL_FATAL(output);
	CU_ASSERT_PTR_NOT_NULL_FATAL(output->points);
    ASSERT_EQUAL_FMT(output->length, 4, "%d", "Length of points does not equal to 4");

    ASSERT_EQUAL_FMT(output->points[0].x, p1.x, "%d", "Must match the x value of p1");
    ASSERT_EQUAL_FMT(output->points[0].y, p1.y, "%d", "Must match the y value of p1");

    ASSERT_EQUAL_FMT(output->points[1].x, p2.x, "%d", "Must match the x value of p2");
    ASSERT_EQUAL_FMT(output->points[1].y, p2.y, "%d", "Must match the y value of p2");

    ASSERT_EQUAL_FMT(output->points[2].x, p3.x, "%d", "Must match the x value of p3");
    ASSERT_EQUAL_FMT(output->points[2].y, p3.y, "%d", "Must match the y value of p3");

    ASSERT_EQUAL_FMT(output->points[3].x, p4.x, "%d", "Must match the x value of p4");
    ASSERT_EQUAL_FMT(output->points[3].y, p4.y, "%d", "Must match the y value of p4");

	if (output != NULL) {
		free(output->points);
		free(output);
	}
}

int main() {
    // Initialize the CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // Create a test suite
    CU_pSuite suite = CU_add_suite("Line test suite", 0, 0);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "test of test_line()", test_line) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run the tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Cleanup
    CU_cleanup_registry();
    return CU_get_error();
}
