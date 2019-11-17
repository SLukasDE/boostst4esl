/*
MIT License
Copyright (c) 2019 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <boostst4esl/Stacktrace.h>
#include <boost/stacktrace.hpp>

namespace boostst4esl {

namespace { /* anonymous Namespace */

std::string getFile(const std::string& fileName) {
	const std::size_t fileLength = 50;
	std::string rv;

	if(fileName.size() > fileLength) {
		rv = "..." + fileName.substr(fileName.size()+3-fileLength);
	}
	else {
		rv = fileName;
	}
	while(rv.size() < fileLength) {
		rv += " ";
	}

	return rv;
}

std::string getLineNo(unsigned int lineNo) {
	const std::size_t lineNoLength = 6;
	std::string rv = std::to_string(lineNo);

	while(rv.size() < lineNoLength) {
		rv = " " + rv;
	}

	return rv;
}

template<typename T>
void genericDump(T& t, const Stacktrace::Backtrace& backtrace) {
	t << "\nStacktrace [file] at [lineno]: [function]:\n";
	for (const auto& entry : backtrace) {
		t << getFile(entry.fileName) << " at " << getLineNo(entry.lineNo) << ": " << entry.functionName << "\n";
	}
}

} /* anonymous Namespace */


Stacktrace::BacktraceEntry::BacktraceEntry(std::string functionName, std::string fileName, std::size_t lineNo)
: functionName(std::move(functionName)),
  fileName(std::move(fileName)),
  lineNo(lineNo)
{ }

Stacktrace::Stacktrace()
: esl::stacktrace::Interface::Stacktrace()
{
	boost::stacktrace::stacktrace st;
	unsigned int skipLines = 0;

	for(const auto& frame : st) {
		if(skipLines > 0) {
			--skipLines;
			continue;
		}

		backtrace.push_back(BacktraceEntry(frame.name(), frame.source_file(), frame.source_line()));
	}
}

void Stacktrace::dump(std::ostream& oStream) const {
        genericDump(oStream, backtrace);
}


void Stacktrace::dump(esl::logging::Logger& logger, esl::logging::Level level) const {
	switch(level) {
	case esl::logging::Level::TRACE:
		genericDump(logger.trace, backtrace);
		break;
	case esl::logging::Level::DEBUG:
		genericDump(logger.debug, backtrace);
		break;
	case esl::logging::Level::INFO:
		genericDump(logger.info, backtrace);
		break;
	case esl::logging::Level::WARN:
		genericDump(logger.warn, backtrace);
		break;
	case esl::logging::Level::ERROR:
		genericDump(logger.error, backtrace);
		break;
	default: /* logging::Level::SILENT */
		break;
	}
}

std::unique_ptr<esl::stacktrace::Interface::Stacktrace> Stacktrace::clone() const {
	return std::unique_ptr<esl::stacktrace::Interface::Stacktrace>(new Stacktrace(*this));
}

} /* namespace boostst4esl */
