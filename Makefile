BUILD_DIR := ./build

WARNINGS := -Wno-implicit-int -Wall

simple_progs := contingency_table 
SIMPLE_BINS := $(simple_progs:%=$(BUILD_DIR)/%)

mathfn_lib := $(BUILD_DIR)/lib/mathfn.o

mathfn_dependent := find_know_number mode logarithm  binomial_series bhaskara\
					coefficient
MATHFN_BINS := $(mathfn_dependent:%=$(BUILD_DIR)/%)

expression_lib := $(BUILD_DIR)/lib/expression.o
expression_dependent := series_convergence print_series integral_aprox
EXPRESSION_BINS := $(expression_dependent:%=$(BUILD_DIR)/%)

la_lib := $(BUILD_DIR)/lib/linear_algebra.o
la_dependent := base_orthonormalization linear_solver linear_eq_tester\
				invert_matrix determinant inverse matmul inner_product kruskal_wallis
LA_BINS := $(la_dependent:%=$(BUILD_DIR)/%)

gmp_dependent := lcm gcd
GMP_BINS := $(gmp_dependent:%=$(BUILD_DIR)/%)

all: $(mathfn_lib) $(torfnum_lib) $(expression_lib) $(la_lib) $(SIMPLE_BINS)\
	 $(MATHFN_BINS) $(EXPRESSION_BINS) $(LA_BINS) $(GMP_BINS)

$(BUILD_DIR)/contingency_table: contingency_table.c string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)

$(mathfn_lib): mathfn.c
	mkdir -p $(BUILD_DIR)/lib
	$(CC) -c $< -o $@ -lm $(WARNINGS)

$(torfnum_lib): torfnum.c
	mkdir -p $(BUILD_DIR)/lib
	$(CC) -c $< -o $@ $(WARNINGS)

$(BUILD_DIR)/coefficient: coefficient.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/bhaskara: bhaskara.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/find_know_number: find_know_number.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/mode: mode.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/logarithm: logarithm.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/binomial_series: binomial_series.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)

$(expression_lib): expression.c expression.h
	$(CC) -c $< -o $@ $(WARNINGS)

$(BUILD_DIR)/series_convergence: series_convergence.c $(expression_lib) string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/print_series: print_series.c $(expression_lib) string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/integral_aprox: integral_aprox.c $(expression_lib) string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)

$(BUILD_DIR)/know_constant: know_constant.c $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $< -o $@ $(mathfn_lib) -lm $(WARNINGS)

$(la_lib): linear_algebra.c linear_algebra.h know_constant.h $(BUILD_DIR)/know_constant
	mkdir -p $(BUILD_DIR)/lib
	$(BUILD_DIR)/know_constant
	$(CC) -c $< -o $@ $(WARNINGS)

$(BUILD_DIR)/base_orthonormalization: base_orthonormalization.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm -lcblas $(WARNINGS)

$(BUILD_DIR)/linear_solver: linear_solver.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/linear_eq_tester: linear_eq_tester.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/invert_matrix: invert_matrix.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/determinant: determinant.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/inverse: inverse.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/matmul: matmul.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lcblas $(WARNINGS)
$(BUILD_DIR)/inner_product: inner_product.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lcblas -lm $(WARNINGS)
$(BUILD_DIR)/kruskal_wallis: kruskal_wallis.c $(la_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lcblas -lm $(WARNINGS)

$(BUILD_DIR)/lcm: lcm.c
	mkdir -p $(BUILD_DIR)
	$(CC) $< -o $@ $(WARNINGS) -lgmp
$(BUILD_DIR)/gcd: gcd.c
	mkdir -p $(BUILD_DIR)
	$(CC) $< -o $@ $(WARNINGS) -lgmp

clean:
	rm -rd $(BUILD_DIR)
	rm know_constants.data

install:
	ln -rsf $(SIMPLE_BINS) $(MATHFN_BINS) $(EXPRESSION_BINS) $(LA_BINS) $(HOME)/.local/bin

test:
	xargs -a tests/logarithm/t01.in ./build/logarithm | diff - tests/logarithm/t01.out 
	xargs -a tests/logarithm/t02.in ./build/logarithm | diff - tests/logarithm/t02.out 
	xargs -a tests/logarithm/t03.in ./build/logarithm | diff - tests/logarithm/t03.out 
	
	xargs -a tests/bhaskara/t01.in ./build/bhaskara | diff - tests/bhaskara/t01.out
	
	cat tests/determinant/t01.in | ./build/determinant | diff - tests/determinant/t01.out
	cat tests/determinant/t02.in | ./build/determinant | diff - tests/determinant/t02.out
	cat tests/determinant/t03.in | ./build/determinant | diff - tests/determinant/t03.out
	cat tests/determinant/t04.in | xargs -a tests/determinant/t04.args ./build/determinant | diff - tests/determinant/t04.out
	
	cat tests/linear_solver/t01.in | ./build/linear_solver | diff - tests/linear_solver/t01.out
	cat tests/linear_solver/t02.in | ./build/linear_solver | diff - tests/linear_solver/t02.out
	
	cat tests/linear_eq_tester/t01.in | ./build/linear_eq_tester | diff - tests/linear_eq_tester/t01.out
	
	xargs -a tests/print_series/t01.in ./build/print_series | diff - tests/print_series/t01.out
	xargs -a tests/print_series/t02.in ./build/print_series | diff - tests/print_series/t02.out
	xargs -a tests/print_series/t03.in ./build/print_series | diff - tests/print_series/t02.out
	xargs -a tests/print_series/t04.in ./build/print_series | diff - tests/print_series/t01.out
	xargs -a tests/print_series/t05.in ./build/print_series | diff - tests/print_series/t05.out
	xargs -a tests/print_series/t06.in ./build/print_series | diff - tests/print_series/t05.out
	xargs -a tests/print_series/t07.in ./build/print_series | diff - tests/print_series/t07.out
	
	xargs -a tests/integral_aprox/t01.in ./build/integral_aprox | diff - tests/integral_aprox/t01.out
	
	xargs -a tests/find_know_number/t01.in ./build/find_know_number | diff - tests/find_know_number/t01.out
	
	xargs -a tests/gcd/t01.in ./build/gcd | diff - tests/gcd/t01.out
	
	xargs -a tests/lcm/t01.in ./build/lcm | diff - tests/lcm/t01.out
	xargs -a tests/lcm/t02.in ./build/lcm | diff - tests/lcm/t02.out
	
	cat tests/matmul/t01.in | ./build/matmul | diff - tests/matmul/t01.out
