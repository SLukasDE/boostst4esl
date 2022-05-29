/*
MIT License
Copyright (c) 2019-2022 Sven Lukas

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

#ifndef BOOSTST4ESL_STACKTRACE_H_
#define BOOSTST4ESL_STACKTRACE_H_

#include <esl/stacktrace/Interface.h>
#include <esl/logging/Location.h>
#include <esl/logging/StreamReal.h>

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace boostst4esl {

class Stacktrace : public esl::stacktrace::Interface::Stacktrace {
public:
	static std::unique_ptr<esl::stacktrace::Interface::Stacktrace> create(const std::vector<std::pair<std::string, std::string>>& settings);

	static inline const char* getImplementation() {
		return "boostst4esl";
	}

	Stacktrace();
	~Stacktrace() = default;

	void dump(std::ostream& stream) const override;
	void dump(esl::logging::StreamReal& stream, esl::logging::Location location) const override;
	std::unique_ptr<esl::stacktrace::Interface::Stacktrace> clone() const override;

private:
	struct BacktraceEntry {
		BacktraceEntry(std::string functionName, std::string fileName, std::size_t lineNo);

		std::string functionName;
		std::string fileName;
		std::size_t lineNo = 0;
	};

	using Backtrace = std::vector<BacktraceEntry>;
	Backtrace backtrace;
};

} /* namespace boostst4esl */

#endif /* BOOSTST4ESL_STACKTRACE_H_ */
