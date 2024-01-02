// Quantum Script Extension Random
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_VARIABLERANDOM_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_VARIABLERANDOM_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Random/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Random {
	class VariableRandom;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::Random::VariableRandom> : public TMemoryPoolActive<XYO::QuantumScript::Extension::Random::VariableRandom> {};
};

namespace XYO::QuantumScript::Extension::Random {

	class VariableRandom : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableRandom);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT, VariableRandom);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT static const char *strTypeRandom;

		public:
			RandomMT value;

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT VariableRandom();

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT void activeConstructor();

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT String toString();
	};

};

#endif
