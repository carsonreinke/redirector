#include <config.h>
#include <stdlib.h> //EXIT_SUCCESS, EXIT_FAILURE
#include <check.h>
#include "../src/query.h"

START_TEST (test_valid_response) {
    unsigned char result[1024];
    size_t length = redirector_query("example.com", result);
    ck_assert_int_ne(-1, length);
    ck_assert_str_eq("url=http://example.org", result);
}
END_TEST

START_TEST (test_missing_response) {
    unsigned char result[1024];
    size_t length = redirector_query("missing.example.com", result);
    ck_assert_int_eq(-1, length);
}
END_TEST

START_TEST (test_invalid) {
    unsigned char result[1024];
    size_t length = redirector_query("!@#$^&*()_+-=", result);
    ck_assert_int_eq(-1, length);
}
END_TEST

START_TEST (test_punycode) {
    unsigned char result[1024];
    size_t length = redirector_query("✔️.example.com", result);
    ck_assert_int_ne(-1, length);
    ck_assert_str_eq("url=http://✔️.example.org", result);
}
END_TEST

Suite * redirector_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Redirector");

    tc_core = tcase_create("Core");

    //tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_valid_response);
    tcase_add_test(tc_core, test_missing_response);
    tcase_add_test(tc_core, test_invalid);
    tcase_add_test(tc_core, test_punycode);
    suite_add_tcase(s, tc_core);

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