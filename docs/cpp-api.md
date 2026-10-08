# C++ API

For hosts that embed Quantum Script, for extensions that take a script
`Random` as an argument, and for maintainers of the extension. Read the
`quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first: native functions, `Variable` and
`TPointer` work the same way here.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/Random.hpp>                  // Library.hpp
#include <XYO/QuantumScript.Extension/Random/VariableRandom.hpp>   // only to use VariableRandom

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::Random` (it imports
`XYO::Cryptography`). Export macro: `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT`:

| Define | Effect |
|--------|--------|
| `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_INTERNAL` (or `QUANTUM_SCRIPT__RANDOM_INTERNAL`, set by fabricare while building the DLL) | export macro = `XYO_PLATFORM_LIBRARY_EXPORT` |
| none | export macro = `XYO_PLATFORM_LIBRARY_IMPORT` (consumers of the DLL) |
| `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_LIBRARY` | export macro empty, no `quantumScriptExtension` entry point (static library) |
| `XYO_PLATFORM_COMPILE_STATIC` (static platforms) | `XYO_PLATFORM_LIBRARY_EXPORT` / `IMPORT` are empty |

The `quantumScriptExtension` entry point is compiled only when
`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is defined and
`XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_LIBRARY` is not.

## Registering the extension

```cpp
void Extension::Random::registerInternalExtension(Executive *executive);
void Extension::Random::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"Random"` as an internal extension;
  call it from the host's init callback (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `Random` in a thread. It:

  1. sets the extension name, info (`"Random"` and the short license text),
     version and public flag;
  2. creates the context (`newContext`): registers the global function
     `Random` (a `VariableFunction` whose native body returns
     `VariableRandom::newVariable()`), keeps its `prototype` in
     `RandomContext::prototypeRandom` and installs `deleteContext` with
     `setExtensionDeleteContext`;
  3. registers the five methods with `executive->setFunction2`:
     `Random.prototype.next()`, `toInteger()`, `toNumber()`, `toString()`,
     `seed(x)`.

  Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--random.dll`.

## `RandomContext`

```cpp
class RandomContext : public Object {
	public:
		Symbol symbolFunctionRandom;              // the "Random" symbol
		TPointerX<Prototype> prototypeRandom;     // Random.prototype
};

RandomContext *getContext();                      // TSingleton<RandomContext>::getValue()
```

One per thread (a `TSingleton`), like every extension context.
`VariableRandom::instancePrototype()` returns
`getContext()->prototypeRandom->prototype`, which is how a `Random` value
finds its methods.

## `VariableRandom`

The script value: a `Variable` holding an `XYO::Cryptography::RandomMT`.

```cpp
class VariableRandom : public Variable {
	public:
		RandomMT value;

		static Variable *newVariable();
		String getVariableType();                 // "Random"
		Variable *instancePrototype();            // Random.prototype
		Variable *clone(SymbolList &inSymbolList);  // a new VariableRandom with value.copy(value)
		bool toBoolean();                         // true
		String toString();                        // "Random"
};
```

- Allocated from an active memory pool
  (`TMemory<VariableRandom> : TMemoryPoolActive<VariableRandom>`):
  `activeConstructor()` runs on every allocation, including reused pool
  entries, and calls `value.seed(0)` — seeded from the clock.
- Runtime type information: `XYO_DYNAMIC_TYPE_DEFINE` /
  `XYO_DYNAMIC_TYPE_IMPLEMENT(VariableRandom, "{A2D9B22E-4185-45CE-BA5D-40989BD2947A}")`;
  test with `TIsType<VariableRandom>(variable)`.
- `clone` copies the full generator state: the copy continues with the same
  numbers.
- `toString()` (the `Variable` conversion, used by `"" + rnd`) returns the
  type name; the script method `Random.prototype.toString()` is a different
  function that returns the value.

### Using a script `Random` from another extension

Depend on `quantum-script--random`, include `VariableRandom.hpp`, check the
type, then use the `RandomMT` directly. This is what
`Pixel32.Image.prototype.noise(random)` does:

```cpp
#include <XYO/QuantumScript.Extension/Random/VariableRandom.hpp>

typedef Extension::Random::VariableRandom VariableRandom;

static TPointer<Variable> imageNoise(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointerX<Variable> &random = arguments->index(0);
	if (TIsType<VariableRandom>(random)) {
		RandomMT &rnd = ((VariableRandom *)(random.value()))->value;
		uint32_t value = rnd.nextRandom();      // advances the script's generator
		// ...
	};
	return Context::getValueUndefined();
};
```

The script's generator is shared: values taken in C++ are not seen again by
the script, and the script's `toInteger()` / `toNumber()` return the last
value C++ produced. The extension's script part should call
`Script.requireExtension("Random")` so the constructor exists.

## The native functions

All are `static` in `Random/Library.cpp`, with the signature

```cpp
static TPointer<Variable> name(VariableFunction *function, Variable *this_, VariableArray *arguments);
```

| Script | C++ | Body |
|--------|-----|------|
| `Random()` / `new Random()` | `functionRandom` | `return VariableRandom::newVariable();` |
| `rnd.next()` | `nextRandom` | `value.nextRandom()`, return `toNumber_` |
| `rnd.toInteger()` | `toInteger` | `(Number)value.getValue()` |
| `rnd.toNumber()` | `toNumber` | `(Number)value.getValue() / 4294967296.0` |
| `rnd.toString()` | `toString` | `VariableNumber::toStringX(toNumber_)` |
| `rnd.seed(x)` | `seed` | `false` if `isnan`, `isinf` or `signbit` of `x->toNumber()`; else `value.seed((Integer)x)` (truncated to `uint32_t`), `true` |

Each method first checks `TIsType<VariableRandom>(this_)` and throws
`Error("invalid parameter")` otherwise. With
`XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` defined each prints a trace line
(`- random-next-random`, `- random-to-integer`, ...).

`RandomMT::seed(0)` seeds from `time(nullptr)`, which is why `seed(0)` from a
script and every new `Random` are clock seeded (see the `xyo-cryptography`
documentation, `docs/random.md`).

## Notes for maintainers

- New method: write `static TPointer<Variable> name(...)` in `Library.cpp`
  (check `TIsType<VariableRandom>(this_)` first), register it in
  `initExecutive` with
  `executive->setFunction2("Random.prototype.name(x)", name)`. Then update
  `README.md`, `docs/script-api.md`, `docs/reference.md`, the skill in
  `.claude/skills/quantum-script--random/` and `test/test.01.js`.
- The sequence for a given seed is part of the contract: scripts and the
  `Pixel32` noise functions rely on `seed(x)` reproducing the same values,
  and on the values matching `std::mt19937`. Do not change the algorithm,
  the `/ 4294967296.0` scaling or the seed truncation.
- `VariableRandom`'s layout (`RandomMT value`) is used by other extensions
  (`quantum-script--pixel32`); changing it requires rebuilding them.
- `test/test.01.js` only loads the extension; run it with `fabricare test`.
