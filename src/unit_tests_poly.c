/*
 * Copyright 2008 Google Inc.
 * Copyright 2015 Tomasz Kociumaka
 * Copyright 2016, 2017 IPP team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
//edited by: Michał Tepper, 2017

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "cmocka.h"

#include "poly.h"

static jmp_buf jmp_at_exit;
static int exit_status;

extern int calc_poly_main();

/**
 * Atrapa funkcji main
 */
int mock_main() {
    if (!setjmp(jmp_at_exit))
        return calc_poly_main();
    return exit_status;
}

/**
 * Atrapa funkcji exit
 */
void mock_exit(int status) {
    exit_status = status;
    longjmp(jmp_at_exit, 1);
}


int mock_fprintf(FILE* const file, const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(2, 3);
int mock_printf(const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

/**
 * Pomocnicze bufory, do których piszą atrapy funkcji printf i fprintf oraz
 * pozycje zapisu w tych buforach. Pozycja zapisu wskazuje bajt o wartości 0.
 */
static char fprintf_buffer[256];
static char printf_buffer[256];
static int fprintf_position = 0;
static int printf_position = 0;

/**
 * Atrapa funkcji fprintf sprawdzająca poprawność wypisywania na stderr.
 */
int mock_fprintf(FILE* const file, const char *format, ...) {
    int return_value;
    va_list args;

    assert_true(file == stderr);
    /* Poniższa asercja sprawdza też, czy fprintf_position jest nieujemne.
    W buforze musi zmieścić się kończący bajt o wartości 0. */
    assert_true((size_t)fprintf_position < sizeof(fprintf_buffer));

    va_start(args, format);
    return_value = vsnprintf(fprintf_buffer + fprintf_position,
                             sizeof(fprintf_buffer) - fprintf_position,
                             format,
                             args);
    va_end(args);

    fprintf_position += return_value;
    assert_true((size_t)fprintf_position < sizeof(fprintf_buffer));
    return return_value;
}

/**
 * Atrapa funkcji printf sprawdzająca poprawność wypisywania na stderr.
 */
int mock_printf(const char *format, ...) {
    int return_value;
    va_list args;

    /* Poniższa asercja sprawdza też, czy printf_position jest nieujemne.
    W buforze musi zmieścić się kończący bajt o wartości 0. */
    assert_true((size_t)printf_position < sizeof(printf_buffer));

    va_start(args, format);
    return_value = vsnprintf(printf_buffer + printf_position,
                             sizeof(printf_buffer) - printf_position,
                             format,
                             args);
    va_end(args);

    printf_position += return_value;
    assert_true((size_t)printf_position < sizeof(printf_buffer));
    return return_value;
}

/**
 *  Pomocniczy bufor, z którego korzystają atrapy funkcji operujących na stdin.
 */
static char input_stream_buffer[256];
static int input_stream_position = 0;
static int input_stream_end = 0;
int read_char_count;

/**
 * Atrapa funkcji scanf używana do przechwycenia czytania z stdin.
 */
int mock_scanf(const char *format, ...) {
    va_list fmt_args;
    int ret;

    va_start(fmt_args, format);
    ret = vsscanf(input_stream_buffer + input_stream_position, format, fmt_args);
    va_end(fmt_args);

    if (ret < 0) { /* ret == EOF */
        input_stream_position = input_stream_end;
    }
    else {
        assert_true(read_char_count >= 0);
        input_stream_position += read_char_count;
        if (input_stream_position > input_stream_end) {
            input_stream_position = input_stream_end;
        }
    }
    return ret;
}

/**
 * Atrapa funkcji getchar używana do przechwycenia czytania z stdin.
 */
int mock_getchar() {
    if (input_stream_position < input_stream_end)
        return input_stream_buffer[input_stream_position++];
    else
        return EOF;
}
/**
 * Atrapa funkcji getc używana do przechwycenia czytania z stdin.
 * Obsługiwane hest tylko standardowe wejście.
 */
int mock_getc(FILE *stream){
	assert_true(stream == stdin);
	return mock_getchar();
}	

/**
 * Atrapa funkcji ungetc.
 * Obsługiwane jest tylko standardowe wejście.
 */
int mock_ungetc(int c, FILE *stream) {
    assert_true(stream == stdin);
    if (input_stream_position > 0)
        return input_stream_buffer[--input_stream_position] = c;
    else
        return EOF;
}

/**
 * Funkcja wołana przed każdym testem korzystającym z stdout lub stderr.
 */
static int test_setup(void **state) {
    (void)state;

    memset(fprintf_buffer, 0, sizeof(fprintf_buffer));
    memset(printf_buffer, 0, sizeof(printf_buffer));
    printf_position = 0;
    fprintf_position = 0;

    /* Zwrócenie zera oznacza sukces. */
    return 0;
}

/**
 * Funkcja inicjująca dane wejściowe dla programu korzystającego ze stdin.
 */
static void init_input_stream(const char *str) {
    memset(input_stream_buffer, 0, sizeof(input_stream_buffer));
    input_stream_position = 0;
    input_stream_end = strlen(str);
    assert_true((size_t)input_stream_end < sizeof(input_stream_buffer));
    strcpy(input_stream_buffer, str);
}

static void run_main_and_check_outputs(char *printf_correct_output, 
	char *fprintf_correct_output){
	mock_main();
	assert_string_equal(printf_buffer, printf_correct_output);
	assert_string_equal(fprintf_buffer, fprintf_correct_output);
}

static void test_calc_poly_no_parameter(void **state) {
    (void)state;

    init_input_stream("COMPOSE");
	run_main_and_check_outputs("", "ERROR 1 WRONG COUNT\n");
}
static void test_calc_poly_min_arg(void **state) {
    (void)state;

    init_input_stream("COMPOSE 0");
	run_main_and_check_outputs("", "ERROR 1 STACK UNDERFLOW\n");
}
static void test_calc_poly_max_arg(void **state) {
    (void)state;
	long max_arg = UINT_MAX;
	char command[51];
	sprintf(command, "COMPOSE %ld", max_arg);
    init_input_stream(command);
	run_main_and_check_outputs("", "ERROR 1 STACK UNDERFLOW\n");
}
static void test_calc_poly_min_arg_minus_one(void **state) {
    (void)state;

    init_input_stream("COMPOSE -1");
	run_main_and_check_outputs("", "ERROR 1 WRONG COUNT\n");
}
static void test_calc_poly_max_arg_plus_one(void **state) {
    (void)state;
	long max_arg_plus_one = (long)UINT_MAX + 1l;
	char command[51];
	sprintf(command, "COMPOSE %ld", max_arg_plus_one);
    init_input_stream(command);
	run_main_and_check_outputs("", "ERROR 1 WRONG COUNT\n");
}
static void test_calc_poly_above_max_arg(void **state) {
    (void)state;

    init_input_stream("COMPOSE 55555555555555555555555555555555555555555555");
	run_main_and_check_outputs("", "ERROR 1 WRONG COUNT\n");
}
static void test_calc_poly_arg_word(void **state) {
    (void)state;

    init_input_stream("COMPOSE ZBIGNIEW");
	run_main_and_check_outputs("", "ERROR 1 WRONG COUNT\n");
}
static void test_calc_poly_arg_letters_digits_combination(void **state) {
    (void)state;

    init_input_stream("COMPOSE 7ELEVEN");
	run_main_and_check_outputs("", "ERROR 1 WRONG COUNT\n");
}

static void run_poly_compose_and_compare_results(Poly *p, unsigned count, Poly x[], Poly *expectedRes) {
	Poly composeRes = PolyCompose(p, count, x);
	assert_true(PolyIsEq(expectedRes, &composeRes));
	PolyDestroy(&composeRes);
}

static void test_poly_zero_count_zero(void **state) {
	(void)state;

    Poly p = PolyZero();
    unsigned count = 0;
    Poly expectedRes = PolyZero();
    
    Poly x[1];
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
}

static void test_poly_zero_count_one_coeff(void **state) {
    (void)state;

    Poly p = PolyZero();
    unsigned count = 1;
    Poly expectedRes = PolyZero();
    
    Poly x[1];
    x[0] = PolyFromCoeff(1337);
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
}

static void test_poly_coeff_count_zero(void **state) {
    (void)state;

    Poly p = PolyFromCoeff(1337);
    unsigned count = 0;
    Poly expectedRes = PolyZero();
    
    Poly x[1];
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
}

static void test_poly_coeff_count_one_coeff(void **state) {
    (void)state;

    Poly p = PolyFromCoeff(1337);
    unsigned count = 1;
    Poly expectedRes = PolyFromCoeff(1337);
    
    Poly x[1];
    x[0] = PolyFromCoeff(1);
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
}

static void test_poly_x0_count_zero(void **state) {
    (void)state;
	
	Poly p0 = PolyFromCoeff(1);
	Mono m[1];
	m[0] = MonoFromPoly(&p0, 1);
    Poly p = PolyAddMonos(1, m);
    unsigned count = 0;
    Poly expectedRes = PolyZero();
    
    Poly x[1];
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
    PolyDestroy(&p);
}
static void test_poly_x0_count_one_coeff(void **state) {
    (void)state;
	
	Poly p0 = PolyFromCoeff(1);
	Mono m[1];
	m[0] = MonoFromPoly(&p0, 1);
    Poly p = PolyAddMonos(1, m);
    
    unsigned count = 1;
    Poly expectedRes = PolyFromCoeff(1337);
    
    Poly x[1];
    x[0] = PolyFromCoeff(1337);
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
    PolyDestroy(&p);
}
static void test_poly_x0_count_one_x0(void **state) {
    (void)state;
	
	Poly p0 = PolyFromCoeff(1);
	Mono m0[1];
	m0[0] = MonoFromPoly(&p0, 1);
    Poly p = PolyAddMonos(1, m0);
    
    unsigned count = 1;
    Poly p1 = PolyFromCoeff(1);
    Mono m1[1];
    m1[0] = MonoFromPoly(&p1, 1);
    Poly expectedRes = PolyAddMonos(1, m1);
  
    Poly x[1];
    
    Poly p2 = PolyFromCoeff(1);
    Mono m2[1];
    m2[0] = MonoFromPoly(&p2, 1);
    x[0] = PolyAddMonos(1, m2);
    
    run_poly_compose_and_compare_results(&p, count, x, &expectedRes);
    PolyDestroy(&(x[0]));
    PolyDestroy(&expectedRes);
    PolyDestroy(&p);
}

int main() {
    const struct CMUnitTest tests_group_1[] = {
        cmocka_unit_test(test_poly_zero_count_zero),
        cmocka_unit_test(test_poly_zero_count_one_coeff),
        cmocka_unit_test(test_poly_coeff_count_zero),
        cmocka_unit_test(test_poly_coeff_count_one_coeff),
        cmocka_unit_test(test_poly_x0_count_zero),
        cmocka_unit_test(test_poly_x0_count_one_coeff),
        cmocka_unit_test(test_poly_x0_count_one_x0)
    };
    const struct CMUnitTest tests_group_2[] = {
        cmocka_unit_test_setup(test_calc_poly_no_parameter, test_setup),
        cmocka_unit_test_setup(test_calc_poly_min_arg, test_setup),
        cmocka_unit_test_setup(test_calc_poly_max_arg, test_setup),
        cmocka_unit_test_setup(test_calc_poly_min_arg_minus_one, test_setup),
        cmocka_unit_test_setup(test_calc_poly_max_arg_plus_one, test_setup),
        cmocka_unit_test_setup(test_calc_poly_above_max_arg, test_setup),
        cmocka_unit_test_setup(test_calc_poly_arg_word, test_setup),
        cmocka_unit_test_setup(test_calc_poly_arg_letters_digits_combination, test_setup)
    };
    return cmocka_run_group_tests(tests_group_1, NULL, NULL) + 
    	cmocka_run_group_tests(tests_group_2, NULL, NULL);
}
