test: dist/main
	$<

dist/main: dist/main.o dist/Generator.o
	gcc -o $@ $^

dist/main.o: src/main.c
	gcc -c -o $@ $^

dist/Generator.o: src/lib/Generator.c
	gcc -c -o $@ $^

clean:
	rm -vf dist/main
	rm -vf dist/*.o

