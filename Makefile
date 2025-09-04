BUILD_DIR := ./build

WARNINGS := -Wno-implicit-int

simple_progs := contingency_table 
SIMPLE_BINS := $(simple_progs:%=$(BUILD_DIR)/%)

torfnum_lib := $(BUILD_DIR)/lib/torfnum.o

mathfn_lib := $(BUILD_DIR)/lib/mathfn.o
mathfn_dependent := factorizer lcm gcd find_know_number mode logarithm binomial_series bhaskara coefficient
MATHFN_BINS := $(mathfn_dependent:%=$(BUILD_DIR)/%)

expression_lib := $(BUILD_DIR)/lib/expression.o
expression_dependent := series_convergence print_series integral_aprox
EXPRESSION_BINS := $(expression_dependent:%=$(BUILD_DIR)/%)

la_lib := $(BUILD_DIR)/lib/linear_algebra.o
la_dependent := base_orthonormalization linear_solver linear_eq_tester invert_matrix determinant inverse matmul inner_product kruskal_wallis
LA_BINS := $(la_dependent:%=$(BUILD_DIR)/%)

all: $(mathfn_lib) $(torfnum_lib) $(expression_lib) $(la_lib) $(SIMPLE_BINS)\
	 $(MATHFN_BINS) $(EXPRESSION_BINS) $(LA_BINS)

$(BUILD_DIR)/contingency_table: contingency_table.c getch.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)

$(mathfn_lib): mathfn.c
	mkdir -p $(BUILD_DIR)/lib
	$(CC) -c $< -o $@ -lm $(WARNINGS)

$(torfnum_lib): torfnum.c
	mkdir -p $(BUILD_DIR)/lib
	$(CC) -c $< -o $@ $(WARNINGS)

$(BUILD_DIR)/coefficient: coefficient.c getch.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/bhaskara: bhaskara.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/factorizer: factorizer.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/lcm: lcm.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/gcd: gcd.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/find_know_number: find_know_number.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/mode: mode.c getch.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/logarithm: logarithm.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/binomial_series: binomial_series.c $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)

$(expression_lib): expression.c expression.h
	$(CC) -c $< -o $@ $(WARNINGS)

$(BUILD_DIR)/series_convergence: series_convergence.c $(expression_lib) string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/print_series: print_series.c $(torfnum_lib) $(mathfn_lib) $(expression_lib) string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)
$(BUILD_DIR)/integral_aprox: integral_aprox.c $(torfnum_lib) $(mathfn_lib) $(expression_lib) string.c
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm $(WARNINGS)

generate_header: generate_header.c $(mathfn_lib)
	$(CC) $< -o $@ $(mathfn_lib) -lm $(WARNINGS)

$(la_lib): linear_algebra.c linear_algebra.h generate_header
	./generate_header > la_print_know_constant.h
	mkdir -p $(BUILD_DIR)/lib
	$(CC) -c $< -o $@ $(WARNINGS)

$(BUILD_DIR)/base_orthonormalization: base_orthonormalization.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lm -lcblas $(WARNINGS)

$(BUILD_DIR)/linear_solver: linear_solver.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/linear_eq_tester: linear_eq_tester.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/invert_matrix: invert_matrix.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/determinant: determinant.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/inverse: inverse.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(WARNINGS)
$(BUILD_DIR)/matmul: matmul.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lcblas $(WARNINGS)
$(BUILD_DIR)/inner_product: inner_product.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lcblas -lm $(WARNINGS)
$(BUILD_DIR)/kruskal_wallis: kruskal_wallis.c $(la_lib) $(torfnum_lib) $(mathfn_lib)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ -lcblas -lm $(WARNINGS)

clean:
	rm -rd $(BUILD_DIR)

install:
	ln -rsf $(SIMPLE_BINS) $(MATHFN_BINS) $(LA_BINS) $(HOME)/.local/bin

test:
	cat tests/mode/t01.in | ./build/mode | diff - tests/mode/t01.out
	cat tests/mode/t02.in | ./build/mode | diff - tests/mode/t02.out
	cat tests/mode/t03.in | ./build/mode | diff - tests/mode/t03.out
	
	./build/contingency_table tests/contingency_table/t01.in | diff - tests/contingency_table/t01.out
	./build/contingency_table tests/contingency_table/t02.in | diff - tests/contingency_table/t02.out
	
	cat tests/logarithm/t01.in | xargs ./build/logarithm | diff - tests/logarithm/t01.out 
	cat tests/logarithm/t02.in | xargs ./build/logarithm | diff - tests/logarithm/t02.out 
	cat tests/logarithm/t03.in | xargs ./build/logarithm | diff - tests/logarithm/t03.out 
	
	cat tests/bhaskara/t01.in | xargs ./build/bhaskara | diff - tests/bhaskara/t01.out
	
	cat tests/coefficient/t01.in | ./build/coefficient | diff - tests/coefficient/t01.out
	cat tests/coefficient/t02.in | ./build/coefficient | diff - tests/coefficient/t02.out
	cat tests/coefficient/t03.in | ./build/coefficient | diff - tests/coefficient/t03.out
	
	cat tests/determinant/t01.in | ./build/determinant | diff - tests/determinant/t01.out
	
	cat tests/linear_solver/t01.in | ./build/linear_solver | diff - tests/linear_solver/t01.out
	cat tests/linear_solver/t02.in | ./build/linear_solver | diff - tests/linear_solver/t02.out
	
	cat tests/linear_eq_tester/t01.in | ./build/linear_eq_tester | diff - tests/linear_eq_tester/t01.out
	
	cat tests/print_series/t01.in | xargs ./build/print_series | diff - tests/print_series/t01.out
	cat tests/print_series/t02.in | xargs ./build/print_series | diff - tests/print_series/t02.out
	cat tests/print_series/t03.in | xargs ./build/print_series | diff - tests/print_series/t02.out
	cat tests/print_series/t04.in | xargs ./build/print_series | diff - tests/print_series/t01.out
	cat tests/print_series/t05.in | xargs ./build/print_series | diff - tests/print_series/t05.out
	cat tests/print_series/t06.in | xargs ./build/print_series | diff - tests/print_series/t05.out
