#! /usr/bin/bash

.PHONY: f4v

obj = main.o f4v_log.o f4v_box.o f4v_parse.o f4v_utility.o
f4v:	$(obj)
	g++ -o f4v $(obj)

main.o: f4v_log.hpp f4v_type.hpp f4v_parse.hpp
	g++ -c -g main.cpp
f4v_log.o:
	g++ -c -g f4v_log.cpp
f4v_box.o:
	g++ -c -g f4v_box.cpp
f4v_parse.o: f4v_type.hpp f4v_log.hpp f4v_box.hpp
	g++ -c -g f4v_parse.cpp
f4v_utility.o:
	g++ -c -g f4v_utility.cpp

clean:
	rm f4v $(obj)
