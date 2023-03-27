CFLAGS=sudo g++ -o lisp lisp.cpp

.PHONY: clean test

lisp: lisp.cpp

clean:
	rm -f minilisp *~

test: lisp
	@./lisp
