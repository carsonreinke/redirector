#include <config.h>
#include <stdlib.h>
#include <check.h>
#include "../src/query.h"

START_TEST (test_name) {
    const char *result = redirector_query("example.com");
    printf(result);
    ck_assert_int_eq(0, 1);
}
END_TEST

START_TEST (test2_name) {
    ck_assert(1);
}
END_TEST

Suite * redirector_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Redirector");

    /* Core test case */
    tc_core = tcase_create("Core");

    //tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_name);
    suite_add_tcase(s, tc_core);

    /* Limits test case */
    //tc_limits = tcase_create("Limits");

    //tcase_add_test(tc_limits, test_money_create_neg);
    //tcase_add_test(tc_limits, test_money_create_zero);
    //suite_add_tcase(s, tc_limits);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = redirector_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}