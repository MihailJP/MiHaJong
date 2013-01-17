#pragma once

#include <stdexcept>

// Calculate GCD (for reduction)
template<typename T> T gcd(T x, T y) {
	// Euclidean algorithm
	T val1 = (x < y) ? y : x, val2 = (x < y) ? x : y;
	T* m = &val1; T* n = &val2; T* tmpptr = nullptr;
	while (true) {
		*m %= *n;
		if (*m == 0) break;
		tmpptr = m; m = n; n = tmpptr;
	}
	return *n;
}

// Fraction class template
template<typename T> class Rational {
private:
	// Value
	T num, denom;
	// Reduction
	Rational<T>& reduce() {
		if (denom == 0) throw std::range_error("Denominator is zero");
		if (denom < 0) {
			num *= -1; denom *= -1;
		}
		if (num == 0) {
			denom = 1;
		} else {
			T gcd_ans = gcd<T>(num, denom);
			num /= gcd_ans; denom /= gcd_ans;
		}
		return *this;
	}
public:
	// Constructors
	Rational(T numerator = 0, T denominator = 1) {
		if (denominator == 0) throw std::invalid_argument("Cannot set denominator to 0");
		num = numerator; denom = denominator;
		reduce();
	}
	Rational(const Rational<T>& arg) {
		num = arg.num; denom = arg.denom;
	}
	Rational<T>& operator=(const Rational<T>& arg) {
		num = arg.num; denom = arg.denom;
		return *this;
	}
	Rational<T>& operator=(T arg) {
		num = arg; denom = 1;
		return *this;
	}
	// Unary plus and minus
	const Rational& operator+() const {return *this;}
	const Rational& operator-() const {
		return Rational<T>(-num, denom).reduce();
	}
	// addition
	const Rational& operator+(const Rational<T>& addend) const {
		return Rational<T>(num * addend.denom + addend.num * denom, denom * addend.denom).reduce();
	}
	Rational& operator+=(const Rational<T>& addend) {
		return (*this = Rational<T>(num * addend.denom + addend.num * denom, denom * addend.denom).reduce());
	}
	// Subtraction
	const Rational& operator-(const Rational<T>& subtrahend) const {
		return Rational<T>(num * subtrahend.denom - subtrahend.num * denom, denom * subtrahend.denom).reduce();
	}
	Rational& operator-=(const Rational<T>& subtrahend) {
		return (*this = Rational<T>(num * subtrahend.denom - subtrahend.num * denom, denom * subtrahend.denom).reduce());
	}
	// Multiplication
	const Rational& operator*(const Rational<T>& multiplier) const {
		return Rational<T>(num * multiplier.num, denom * multiplier.denom).reduce();
	}
	Rational& operator*=(const Rational<T>& multiplier) {
		return (*this = Rational<T>(num * multiplier.num, denom * multiplier.denom).reduce());
	}
	// Division
	const Rational& operator/(const Rational<T>& divisor) const {
		if (multiplier.num == 0) throw std::invalid_argument("Division by zero");
		return Rational<T>(num * divisor.denom, denom * divisor.num).reduce();
	}
	Rational& operator/=(const Rational<T>& divisor) {
		if (multiplier.num == 0) throw std::invalid_argument("Division by zero");
		return (*this = Rational<T>(num * divisor.denom, denom * divisor.num).reduce());
	}
	// Equality
	bool operator==(const Rational<T>& arg) const {
		return (num == arg.num) && (denom == arg.denom);
	}
	bool operator!=(const Rational<T>& arg) const {return !(*this == arg);}
	// Comparison
	bool operator<(const Rational<T>& arg) const {
		return ((num * arg.denom) < (arg.num * denom));
	}
	bool operator>=(const Rational<T>& arg) const {return !(*this < arg);}
	bool operator>(const Rational<T>& arg) const {return (arg < *this);}
	bool operator<=(const Rational<T>& arg) const {return !(arg < *this);}
	// Accessor
	T getNumerator() const {return num;}
	T getDenominator() const {return denom;}
	// Cast into float
	operator float() const {return (float)num / (float)denom;}
	operator double() const {return (double)num / (double)denom;}
	operator long double() const {return (long double)num / (long double)denom;}
};
