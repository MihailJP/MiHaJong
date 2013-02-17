#pragma once

#include <functional>

template <typename T> class DoFinally {
private:
	std::function<T (void)> doFunc;
	std::function<void (void)> finallyFunc;
public:
	explicit DoFinally(std::function<T (void)> do_func, std::function<void (void)> finally_func = []()->void{}) {
		doFunc = do_func;
		finallyFunc = finally_func;
	}
	const T operator() () const {return doFunc();}
	~DoFinally() {finallyFunc();}
};
