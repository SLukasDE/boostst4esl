#include <esl/module/Library.h>
#include <boostst4esl/Module.h>
#include <esl/Module.h>

extern "C" esl::module::Module* esl__module__library__getModule(const std::string& moduleName) {
	if(moduleName == "esl") {
		return &esl::getModule();
	}
	return &boostst4esl::getModule();
}