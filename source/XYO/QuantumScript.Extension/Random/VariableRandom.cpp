// Quantum Script Extension Random
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript.Extension/Random/Context.hpp>
#include <XYO/QuantumScript.Extension/Random/VariableRandom.hpp>

namespace XYO::QuantumScript::Extension::Random {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableRandom, "{A2D9B22E-4185-45CE-BA5D-40989BD2947A}");
	const char *VariableRandom::strTypeRandom = "Random";

	VariableRandom::VariableRandom() {
		XYO_DYNAMIC_TYPE_PUSH(VariableRandom);
	};

	String VariableRandom::getVariableType() {
		return strTypeRandom;
	};

	Variable *VariableRandom::newVariable() {
		return (Variable *)TMemory<VariableRandom>::newMemory();
	};

	Variable *VariableRandom::instancePrototype() {
		return (Extension::Random::getContext())->prototypeRandom->prototype;
	};

	void VariableRandom::activeConstructor() {
		value.seed(0);
	};

	Variable *VariableRandom::clone(SymbolList &inSymbolList) {
		VariableRandom *out = (VariableRandom *)newVariable();
		out->value.copy(value);
		return out;
	};

	bool VariableRandom::toBoolean() {
		return true;
	};

	String VariableRandom::toString() {
		return strTypeRandom;
	};

};
