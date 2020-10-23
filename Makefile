test: dist/test
	$<

dist/test: dist/test.o dist/Generator.o
	gcc -o $@ $^

dist/test.o: src/test.c
	gcc -c -o $@ $^

dist/Generator.o: src/lib/Generator.c
	gcc -c -o $@ $^

clean:
	rm -vf dist/test
	rm -vf dist/*.o

