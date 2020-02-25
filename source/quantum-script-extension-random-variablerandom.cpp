//
// Quantum Script Extension Random
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-random.hpp"
#include "quantum-script-extension-random-variablerandom.hpp"

#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Random {

				using namespace XYO;
				using namespace Quantum::Script;

				const char *VariableRandom::typeRandomKey = "{A2D9B22E-4185-45CE-BA5D-40989BD2947A}";
				const void *VariableRandom::typeRandom;
				const char *VariableRandom::strTypeRandom = "Random";

				String VariableRandom::getType() {
					return strTypeRandom;
				};

				Variable *VariableRandom::newVariable() {
					return (Variable *) TMemory<VariableRandom>::newMemory();
				};

				Variable &VariableRandom::operatorReference(Symbol symbolId) {
					return operatorReferenceX(symbolId, (Extension::Random::getContext())->prototypeRandom->prototype);
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
		};
	};
};


