// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

#ifndef XYO_CRYPTOGRAPHY_HPP
#	include <XYO/Cryptography.hpp>
#endif

// -- Export

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_INTERNAL
#	ifdef QUANTUM_SCRIPT__RANDOM_INTERNAL
#		define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_LIBRARY
#	undef XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT
#	define XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT
#endif

namespace XYO::QuantumScript::Extension::Random {
	using namespace XYO::Cryptography;
};

#endif
