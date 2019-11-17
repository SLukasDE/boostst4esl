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

#include <boostst4esl/Module.h>
#include <esl/stacktrace/Interface.h>
#include <esl/bootstrap/Interface.h>
#include <new>         // placement new
#include <type_traits> // aligned_storage
#include <boostst4esl/Stacktrace.h>

namespace boostst4esl {

namespace {
class Module : public esl::bootstrap::Module {
public:
	Module() = default;
	~Module() = default;

	static void initialize();

private:
	esl::stacktrace::Interface interfaceEslStacktrace;
};

typename std::aligned_storage<sizeof(Module), alignof(Module)>::type moduleBuffer; // memory for the object;
Module& module = reinterpret_cast<Module&> (moduleBuffer);

esl::stacktrace::Interface::Stacktrace* createStacktrace() {
	return new Stacktrace();
}

void Module::initialize() {
	static bool isInitialized = false;

	if(isInitialized == false) {
		isInitialized = true;

		/* ***************** *
		 * initialize module *
		 * ***************** */
		new (&module) Module(); // placement new
		esl::bootstrap::Module::initialize(module);
		esl::stacktrace::Interface::initialize(module.interfaceEslStacktrace,
				&createStacktrace);
		module.interfacesProvided.next = &module.interfaceEslStacktrace;
	}
}
}

const esl::bootstrap::Module& getModule() {
	Module::initialize();
	return module;
}

} /* namespace boostst4esl */
