#include <esl/module/Library.h>
#include <boostst4esl/Module.h>

esl::module::Library::GetModule esl__module__library__getModule = &boostst4esl::getModule;
