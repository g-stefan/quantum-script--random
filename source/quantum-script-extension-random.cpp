//
// Quantum Script Extension Random
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-random-license.hpp"
#include "quantum-script-extension-random.hpp"
#include "quantum-script-extension-random-variablerandom.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_RANDOM_NO_VERSION
#include "quantum-script-extension-random-version.hpp"
#endif

#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"


#include "quantum-script-variablestring.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Random {

				using namespace XYO;
				using namespace Quantum::Script;

				RandomContext::RandomContext() {
					symbolFunctionRandom = 0;
					prototypeRandom.pointerLink(this);
				};

				RandomContext *getContext() {
					return TSingleton<RandomContext>::getValue();
				};

				static TPointer<Variable> functionRandom(VariableFunction *function, Variable *this_, VariableArray *arguments) {
					return VariableRandom::newVariable();
				};

				static void deleteContext() {
					RandomContext *randomContext = getContext();
					randomContext->prototypeRandom.deleteMemory();
					randomContext->symbolFunctionRandom = 0;
				};

				static void newContext(Executive *executive, void *extensionId) {
					VariableFunction *defaultPrototypeFunction;

					RandomContext *randomContext = getContext();
					executive->setExtensionDeleteContext(extensionId, deleteContext);

					randomContext->symbolFunctionRandom = Context::getSymbol("Random");
					randomContext->prototypeRandom.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionRandom, nullptr, nullptr);
					(Context::getGlobalObject())->setPropertyBySymbol(randomContext->symbolFunctionRandom, defaultPrototypeFunction);
					randomContext->prototypeRandom = defaultPrototypeFunction->prototype;
				};

				static Number toNumber_(Variable *value) {
					return ((Number)((static_cast<VariableRandom *>(value))->value.getValue())) / (double)4294967296.0;
				};

				static Number toInteger_(Variable *value) {
					return ((Number)((static_cast<VariableRandom *>(value))->value.getValue()));
				};

				static TPointer<Variable> nextRandom(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- random-next-random\n");
#endif

					if(!TIsType<VariableRandom>(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableRandom *)( this_ ))->value.nextRandom();
					return VariableNumber::newVariable(toNumber_(this_));
				};

				static TPointer<Variable> toInteger(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- random-to-integer\n");
#endif

					if(!TIsType<VariableRandom>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(toInteger_(this_));
				};

				static TPointer<Variable> toNumber(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- random-to-number\n");
#endif

					if(!TIsType<VariableRandom>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(toNumber_(this_));
				};

				static TPointer<Variable> toString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- random-to-string\n");
#endif

					if(!TIsType<VariableRandom>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableString::newVariable(VariableNumber::toStringX(toNumber_(this_)));
				};


				static TPointer<Variable> seed(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- random-seed\n");
#endif

					if(!TIsType<VariableRandom>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number value = (arguments->index(0))->toNumber();
					if(isnan(value) || isinf(value) || signbit(value)) {
						return VariableBoolean::newVariable(false);
					};


					((VariableRandom *)( this_ ))->value.seed((Integer)value);

					return VariableBoolean::newVariable(true);
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("Random", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "Random\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "Random");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_RANDOM_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::Random::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					newContext(executive, extensionId);

					executive->setFunction2("Random.prototype.next()",  nextRandom);
					executive->setFunction2("Random.prototype.toInteger()",  toInteger);
					executive->setFunction2("Random.prototype.toNumber()",  toNumber);
					executive->setFunction2("Random.prototype.toString()",  toString);
					executive->setFunction2("Random.prototype.seed(x)",  seed);

				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::Random::initExecutive(executive, extensionId);
};
#endif

