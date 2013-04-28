#pragma once

#include <functional>
#include <cstddef>

using std::size_t;

template <typename T> class DoFinally {
private:
	std::function<T (void)> doFunc;
	std::function<void (void)> finallyFunc;
private:
	static void* operator new     (size_t); // Invalidate scalar new
	static void* operator new[]   (size_t); // Invalidate array  new
	static void  operator delete  (void*) ; // Invalidate scalar delete
	static void  operator delete[](void*) ; // Invalidate array  delete
public:
	explicit DoFinally(std::function<T (void)> do_func, std::function<void (void)> finally_func = []()->void{}) {
		doFunc = do_func;
		finallyFunc = finally_func;
	}
	const T operator() () const {return doFunc();}
	~DoFinally() {finallyFunc();}
};
