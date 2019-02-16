#include <config.h>
#include <stdlib.h> //EXIT_SUCCESS, EXIT_FAILURE
#include <check.h>
#include "../src/utils.h"
#include "../src/query.h"
#include "../src/uri.h"

START_TEST (test_valid_response) {
    unsigned char *result;
    int length = redirector_query_txt("example.com", &result);
    ck_assert_int_ne(REDIRECTOR_ERROR, length);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq("http://example.org", result);
    free(result);
}
END_TEST

START_TEST (test_valid_response_uri) {
    unsigned char *result;
    int length = redirector_query_uri("example.com", &result);
    ck_assert_int_eq(strlen("http://example.org"), length);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq("http://example.org", result);
    free(result);
}
END_TEST

START_TEST (test_missing_response) {
    unsigned char *result;
    int length = redirector_query_txt("missing.example.com", &result);
    free(result);
    ck_assert_int_eq(REDIRECTOR_ERROR, length);
}
END_TEST

START_TEST (test_remove_www) {
    unsigned char *result;
    int length = redirector_query_txt("www.example.com", &result);
    ck_assert_int_ne(REDIRECTOR_ERROR, length);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq("http://example.org", result);
    free(result);
}
END_TEST

START_TEST (test_invalid) {
    unsigned char *result;
    int length = redirector_query_txt("!@#$^&*()_+-=", &result);
    free(result);
    ck_assert_int_eq(REDIRECTOR_ERROR, length);
}
END_TEST

START_TEST (test_null) {
    unsigned char *result;
    int length = redirector_query_txt(NULL, &result);
    ck_assert_int_eq(REDIRECTOR_ERROR, length);
}
END_TEST

START_TEST (test_null_dest) {
    int length = redirector_query_txt("example.com", NULL);
    ck_assert_int_eq(REDIRECTOR_ERROR, length);
}
END_TEST

START_TEST (test_punycode) {
    unsigned char *result;
    int length = redirector_query_txt("✔️.example.com", &result);
    ck_assert_int_ne(REDIRECTOR_ERROR, length);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq("http://✔️.example.org", result);
    free(result);
}
END_TEST

START_TEST (test_underflow) {
    unsigned char *result;
    int length = redirector_query_txt("www.", &result);
    ck_assert_int_eq(REDIRECTOR_ERROR, length);
}
END_TEST

START_TEST (test_empty) {
    unsigned char *result;
    int length = redirector_query_txt("", &result);
    ck_assert_int_eq(REDIRECTOR_ERROR, length);
}
END_TEST

START_TEST (test_uri_valid) {
    unsigned char *uri = "http://example.org";
    unsigned char *result;

    result = redirect_uri_normalize(uri);
    ck_assert_str_eq(uri, result);

    free(result);
}
END_TEST

START_TEST (test_uri_valid_path) {
    unsigned char *uri = "https://example.org/testing/";
    unsigned char *result;

    result = redirect_uri_normalize(uri);
    ck_assert_str_eq(uri, result);

    free(result);
}
END_TEST

START_TEST (test_uri_ftp) {
    unsigned char *uri = "ftp://example.org";
    unsigned char *result;

    result = redirect_uri_normalize(uri);
    ck_assert_ptr_null(result);
}
END_TEST

START_TEST (test_uri_relative) {
    unsigned char *result;

    result = redirect_uri_normalize("./test");
    ck_assert_ptr_null(result);
}
END_TEST

START_TEST (test_uri_garbage) {
    unsigned char *result;

    result = redirect_uri_normalize("~!#$^&*()_");
    ck_assert_ptr_null(result);
}
END_TEST

Suite * redirector_suite(void)
{
    Suite *s;
    TCase *tc_query;
    TCase *tc_uri;

    s = suite_create("Redirector");

    tc_query = tcase_create("query");
    tcase_add_test(tc_query, test_valid_response);
    tcase_add_test(tc_query, test_valid_response_uri);
    tcase_add_test(tc_query, test_missing_response);
    tcase_add_test(tc_query, test_remove_www);
    tcase_add_test(tc_query, test_invalid);
    tcase_add_test(tc_query, test_null);
    tcase_add_test(tc_query, test_null_dest);
    tcase_add_test(tc_query, test_punycode);
    tcase_add_test(tc_query, test_underflow);
    tcase_add_test(tc_query, test_empty);
    suite_add_tcase(s, tc_query);

    tc_uri = tcase_create("uri");
    tcase_add_test(tc_uri, test_uri_valid);
    tcase_add_test(tc_uri, test_uri_valid_path);
    tcase_add_test(tc_uri, test_uri_ftp);
    tcase_add_test(tc_uri, test_uri_relative);
    tcase_add_test(tc_uri, test_uri_garbage);
    suite_add_tcase(s, tc_uri);

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