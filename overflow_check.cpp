#include <bits/stdc++.h>

struct without_sign
{};

struct with_sign
{};

struct without_point
{};

struct with_point
{};

template <bool sign>
struct check_overflow_sign
{
	typedef with_sign type_sign;
};

template <>
struct check_overflow_sign <true>
{
	typedef with_sign type_sign;
};

template <>
struct check_overflow_sign <false>
{
	typedef without_sign type_sign;
};

template <bool point>
struct check_overflow_point
{
	typedef without_point type_point;
};

template <>
struct check_overflow_point <true>
{
	typedef without_point type_point;
};

template <>
struct check_overflow_point <false>
{
	typedef with_point type_point;
};

struct exception
{};

template <typename T>
struct check_overflow
{
	T value;
	//static const size_t sz = std::numeric_limits<T>::digits;
	static const T max_value = std::numeric_limits<T>::max();
	static const T min_value = std::numeric_limits<T>::min();
	static const bool no_point = std::numeric_limits<T>::is_integer;
	static const bool sign = std::numeric_limits<T>::is_signed;
	
	check_overflow (T a);

	check_overflow <T>& operator+=(check_overflow<T> const&);
	check_overflow <T>& operator-=(check_overflow<T> const&);
	check_overflow <T>& operator*=(check_overflow<T> const&);
	check_overflow <T>& operator/=(check_overflow<T> const&);

	void add(T const&, with_sign);
	void add(T const&, without_sign);
	void subtract(T const&, with_sign);
	void subtract(T const&, without_sign);
	void multiply(T const&, with_sign, without_point);
	void multiply(T const&, without_sign, without_point);
	void multiply(T const&, with_sign, with_point);
	void multiply(T const&, without_sign, with_point);
	void divide(T const&, with_sign, without_point);
	void divide(T const&, without_sign, without_point);
	void divide(T const&, with_sign, with_point);
	void divide(T const&, without_sign, with_point);
	
	template <T>
	friend check_overflow<T> operator+(check_overflow<T> const&, check_overflow<T> const&);
	
	template <T>
	friend check_overflow<T> operator-(check_overflow<T> const&, check_overflow<T> const&);

	template <T>
	friend check_overflow<T> operator*(check_overflow<T> const&, check_overflow<T> const&);

	template <T>
	friend check_overflow<T> operator/(check_overflow<T> const&, check_overflow<T> const&);
};

template <typename T>
check_overflow <T>::check_overflow(T a):
	value(a)
{}

template <typename T>
void check_overflow <T>::add(T const& that, with_sign)
{
	if ((that >= 0 && value > max_value - that) || (that < 0 && value < min_value - that))
		throw (new exception());
	value += that;
}

template <typename T>
void check_overflow <T>::add(T const& that, without_sign)
{
	if (value > max_value - that)
		throw (new exception());
	value += that;
}

template <typename T>
void check_overflow <T>::subtract(T const& that, with_sign)
{
	if ((that >= 0 && value < max_value + that) || (that < 0 && value > min_value + that))
		throw exception();
	value -= that;
}

template <typename T>
void check_overflow <T>::subtract(T const& that, without_sign)
{
	if (value < max_value + that)
		throw exception();
	value -= that;
}

template <typename T>
void check_overflow <T>::multiply(T const& t, with_sign, without_point)
{
	T that = t;
	if (value < that)
		std::swap(that, value);
	if (value == 0 || that == 0)
	{
		value = (T)0;
		return;
	}
	if (value > 0 && that < 0) {
		if (that < min_value / value)
			throw exception();
	}
	else
	{
		if (value < 0 && that < 0)
		{
			if (that < max_value / value)
				throw exception();
		}
		else
		{
			if (that > max_value / value)
				throw exception();
		}
	}
	value *= that;
}

template <typename T>
void check_overflow <T>::multiply(T const& that, without_sign, without_point)
{
	if (that == 0 || value == 0)
	{
		value = (T)0;
		return;
	}
	if (that > max_value / value)
		throw exception();
	value *= that;
}

template <typename T>
void check_overflow <T>::multiply(T const& that, with_sign, with_point)
{
	T x = (value >= 0) ? value : -value;
	T y = (that >= 0) ? that : -that;
	if (x < y)
		std::swap(x, y);
	if (x <= 1)
	{
		value *= that;
		return;
	}
	if (y > max_value / x)
		throw exception();
	value *= that;
}

template <typename T>
void check_overflow <T>::multiply(T const& that, without_sign, with_point)
{
	if (value <= 1 || that <= 1)
	{
		value *= that;
		return;
	}
	if (that > max_value / value)
		throw exception();
	value *= that;
}

template <typename T>
void check_overflow <T>::divide(T const& that, with_sign, without_point)
{
	if (that == 0)
		throw exception();
	if (that == -1 && value == min_value)
		throw new exception();
	value /= that;
}

template <typename T>
void check_overflow <T>::divide(T const& that, without_sign, without_point)
{
	if (that == 0)
		throw exception();
	value /= that;
}

template <typename T>
void check_overflow <T>::divide(T const& that, with_sign, with_point)
{
	if (that >= 1 || that <= -1)
	{
		value /= that;
		return;
	}
	if (that >= 0)
	{
		if (value > max_value * that || value < min_value * that)
			throw exception();		
	}
	else
	{
		if (value < max_value * that || value > min_value * that)
			throw exception();
	}
	value /= that;
}

template <typename T>
void check_overflow <T>::divide(T const& that, without_sign, with_point)
{
	if (that >= 1)
	{
		value /= that;
		return;
	}
	if (value > max_value * that)
		throw exception();
	value /= that;
}

template <typename T>
check_overflow <T>& check_overflow <T>::operator+=(check_overflow <T> const& that)
{
	 add(that.value, typename check_overflow_sign <sign>::type_sign());
	 return *this;
}

template <typename T>
check_overflow <T>& check_overflow <T>::operator-=(check_overflow <T> const& that)
{
	subtract(that.value, typename check_overflow_sign <sign>::type_sign());
	return *this;
}

template <typename T>
check_overflow <T>& check_overflow <T>::operator*=(check_overflow <T> const& that)
{
	multiply(that.value, typename check_overflow_sign <sign>::type_sign(), typename check_overflow_point <no_point>::type_point());
	return *this;
}

template <typename T>
check_overflow <T>& check_overflow <T>::operator/=(check_overflow <T> const& that)
{
	divide(that.value, typename check_overflow_sign <sign>::type_sign(), typename check_overflow_point <no_point>::type_point());
	return *this;
}

template <typename T>
check_overflow <T> operator+(check_overflow <T> const& a, check_overflow <T> const& b)
{
	return (check_overflow <T> (a.value) += b);
}

template <typename T>
check_overflow <T> operator-(check_overflow <T> const& a, check_overflow <T> const& b)
{
	return (check_overflow <T> (a.value) -= b);
}

template <typename T>
check_overflow <T> operator*(check_overflow <T> const& a, check_overflow <T> const& b)
{
	return (check_overflow <T> (a.value) *= b);
}

template <typename T>
check_overflow <T> operator/(check_overflow <T> const& a, check_overflow <T> const& b)
{
	return (check_overflow <T> (a.value) /= b);
}

//using namespace std;

//typedef unsigned long long my_type; 

//int main()
//{
	//check_overflow <my_type> that = 1;
	//for (check_overflow <my_type> i = 1; ; i = i + check_overflow <my_type> (1))
	//{
		//try
		//{
			//that *= 2;
			//cout << that.value << endl;
		//}
		//catch (...)
		//{
			//break;
		//}
	//}
	
	//return 0;
//}
