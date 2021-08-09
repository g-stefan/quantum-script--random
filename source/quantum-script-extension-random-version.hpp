//
// Quantum Script Extension Random
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_RANDOM_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_RANDOM_VERSION_HPP

#define QUANTUM_SCRIPT_EXTENSION_RANDOM_VERSION_ABCD                 2,1,0,19
#define QUANTUM_SCRIPT_EXTENSION_RANDOM_VERSION_STR                 "2.1.0"
#define QUANTUM_SCRIPT_EXTENSION_RANDOM_VERSION_STR_BUILD           "19"
#define QUANTUM_SCRIPT_EXTENSION_RANDOM_VERSION_STR_DATETIME        "2021-08-09 17:38:06"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_RANDOM__EXPORT_HPP
#include "quantum-script-extension-random--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Random {
				namespace Version {
					QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT const char *version();
					QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT const char *build();
					QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT const char *versionWithBuild();
					QUANTUM_SCRIPT_EXTENSION_RANDOM_EXPORT const char *datetime();
				};
			};
		};
	};
};

#endif
#endif

