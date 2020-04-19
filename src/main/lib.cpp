#include <esl/module/Library.h>
#include <boostst4esl/Module.h>

extern "C" esl::module::Module* esl__module__library__getModule(const std::string& moduleName) {
	return boostst4esl::getModulePointer(moduleName);
}
