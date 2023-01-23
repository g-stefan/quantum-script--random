// Quantum Script Extension Random
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Random/Library.hpp>
#include <XYO/QuantumScript.Extension/Random/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Random/License.hpp>
#include <XYO/QuantumScript.Extension/Random/Version.hpp>
#include <XYO/QuantumScript.Extension/Random/Context.hpp>
#include <XYO/QuantumScript.Extension/Random/VariableRandom.hpp>

namespace XYO::QuantumScript::Extension::Random {
	using namespace XYO::Cryptography;

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
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- random-next-random\n");
#endif

		if (!TIsType<VariableRandom>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableRandom *)(this_))->value.nextRandom();
		return VariableNumber::newVariable(toNumber_(this_));
	};

	static TPointer<Variable> toInteger(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- random-to-integer\n");
#endif

		if (!TIsType<VariableRandom>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(toInteger_(this_));
	};

	static TPointer<Variable> toNumber(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- random-to-number\n");
#endif

		if (!TIsType<VariableRandom>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(toNumber_(this_));
	};

	static TPointer<Variable> toString(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- random-to-string\n");
#endif

		if (!TIsType<VariableRandom>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableString::newVariable(VariableNumber::toStringX(toNumber_(this_)));
	};

	static TPointer<Variable> seed(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- random-seed\n");
#endif

		if (!TIsType<VariableRandom>(this_)) {
			throw(Error("invalid parameter"));
		};

		Number value = (arguments->index(0))->toNumber();
		if (isnan(value) || isinf(value) || signbit(value)) {
			return VariableBoolean::newVariable(false);
		};

		((VariableRandom *)(this_))->value.seed((Integer)value);

		return VariableBoolean::newVariable(true);
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Random", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "Random\r\n";
		info << License::shortLicense();

		executive->setExtensionName(extensionId, "Random");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Random::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		newContext(executive, extensionId);

		executive->setFunction2("Random.prototype.next()", nextRandom);
		executive->setFunction2("Random.prototype.toInteger()", toInteger);
		executive->setFunction2("Random.prototype.toNumber()", toNumber);
		executive->setFunction2("Random.prototype.toString()", toString);
		executive->setFunction2("Random.prototype.seed(x)", seed);
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Random::initExecutive(executive, extensionId);
};
#endif
