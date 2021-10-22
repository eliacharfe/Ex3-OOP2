#pragma once

class MyException : public std::exception
{ 
public:
	MyException(const char* str) : m_str(str) {}
	const char* what() const noexcept  { return m_str; }

private:
	const char* m_str;
};
//--------------------------------------------
class MyException2 : public std::exception
{ // error at the file
public:
	MyException2(const char* str) : m_str(str) {}
	const char* what() const noexcept { return m_str; }

private:
	const char* m_str;
};
//-------------------------------------------
class MyException3 : public std::exception
{ // error at the input for continue reading or not
public:
	MyException3() {}

private:
};