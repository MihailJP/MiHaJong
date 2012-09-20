#ifndef EXCEPT_H
#define EXCEPT_H

#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

class socket_error : public std::runtime_error {
private:
	int myErrorCode;
public:
	socket_error(const int err) : std::runtime_error(
		[](int i) -> std::string {std::ostringstream o; o << "Error " << i; return o.str();} (err))
	{
		myErrorCode = err;
	}
	int error_code() {return myErrorCode;}
};

#endif
