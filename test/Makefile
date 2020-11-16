testcase: test_concurrent_write

test_concurrent_write: types.h threadpool.h test_concurrent_write.cc
	g++ -std=c++17 -g3 -pthread test_concurrent_write.cc -o test_concurrent_write

clean:
	rm test_concurrent_write
	write.txt
