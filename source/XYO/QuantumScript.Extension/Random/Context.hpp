// Quantum Script Extension Random
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Random/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Random {

	class RandomContext : public Object {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(RandomContext);

		public:
			Symbol symbolFunctionRandom;
			TPointerX<Prototype> prototypeRandom;

			XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT RandomContext();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT RandomContext *getContext();

};

#endif
