#include "HookException.h"
#include <sstream>

HookException::HookException(int line, const char* file) noexcept : line(line), file(file)
{
}

const char* HookException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* HookException::GetType() const noexcept
{
	return "Hook Exception";
}

int HookException::GetLine() const noexcept
{
	return line;
}

const std::string& HookException::GetFile() const noexcept
{
	return file;
}

std::string HookException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl << "[Line] " << line;
	return oss.str();
}