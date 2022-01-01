﻿//
// Quantum Script Extension Random
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_RANDOM_VARIABLERANDOM_HPP
#define QUANTUM_SCRIPT_EXTENSION_RANDOM_VARIABLERANDOM_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_RANDOM__EXPORT_HPP
#include "quantum-script-extension-random--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Random {

				class VariableRandom;

			};
		};
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::Random::VariableRandom>:
			public TMemoryPoolActive<Quantum::Script::Extension::Random::VariableRandom> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Random {

				using namespace XYO;
				using namespace Quantum::Script;

				class VariableRandom :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableRandom);
						XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT, VariableRandom);
					protected:
						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT static const char *strTypeRandom;
					public:

						RandomMT value;

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT VariableRandom();

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT void activeConstructor();

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT String getVariableType();

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT String toString();

				};

			};
		};
	};
};


#endif
