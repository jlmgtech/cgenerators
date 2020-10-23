test: dist/test dist/teststatic dist/testshared
	###### testing basic binary  ######
	dist/test
	###### testing static binary ######
	dist/teststatic
	###### testing shared binary ######
	dist/testshared

dist/test: dist/test.o dist/Generator.o
	gcc -o $@ $^

dist/testshared: dist/test.o dist/libgenc.so
	gcc -o $@ -L./dist $< -lgenc -Wl,-rpath,'$$ORIGIN'

dist/teststatic: dist/test.o dist/libgenc.a
	gcc -o $@ $^

dist/test.o: src/test.c
	gcc -c -o $@ $^

dist/genc.o: src/lib/Generator.c
	gcc -c -fPIC -o $@ $<

dist/libgenc.so: dist/genc.o
	gcc -shared -o $@ $^

dist/libgenc.a: dist/Generator.o
	ar -rc $@ $<

dist/Generator.o: src/lib/Generator.c
	gcc -c -o $@ $^


installstatic: dist/libgenc.a
	cp $< /usr/local/lib

installshared: dist/libgenc.so
	cp $< /usr/local/lib

installheaders: src/include/Generator.h
	cp $< /usr/local/include

# TODO - probably should run an installer script instead
install: installheaders installstatic installshared
	ldconfig
	# 						  #
	# successfully installed! #
	# 						  #

uninstall:
	rm /usr/local/lib/libgenc.a
	rm /usr/local/lib/libgenc.so
	rm /usr/local/include/Generator.h
	ldconfig

clean:
	rm -vf dist/*
	rm -vf dist/*.o

