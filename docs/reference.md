# API reference

## Script

Available after `Script.requireExtension("Random")`.

### Constructor

| Symbol | Returns | Notes |
|--------|---------|-------|
| `new Random()` | Random | independent MT19937 generator, seeded with the time in seconds; arguments ignored |
| `Random()` | Random | same as `new Random()` |

`typeof(rnd)` is `"Random"`, `rnd instanceof Random` is `true`,
`Script.isObject(rnd)` is `false`, a `Random` is always truthy, assignment
copies the reference.

### Methods (`Random.prototype`)

| Symbol | Returns | Notes |
|--------|---------|-------|
| `rnd.next()` | Number | advances; the new value / 2³², in `[0, 1)` |
| `rnd.toInteger()` | Number | the last value, integer in `[0, 4294967295]`; does not advance |
| `rnd.toNumber()` | Number | the last value / 2³², in `[0, 1)`; does not advance |
| `rnd.toString()` | String | `toNumber()` as text; does not advance |
| `rnd.seed(x)` | Boolean | restarts the sequence; see below |

The "last value" right after `new Random()` or `seed(x)` is the seed itself:
call `next()` before `toInteger()` / `toNumber()`.

### `seed(x)`

| `x` (after `toNumber`) | Returns | Seed used |
|------------------------|---------|-----------|
| integer `1` .. `4294967295` | `true` | `x` |
| fractional | `true` | truncated toward zero |
| `>= 4294967296` | `true` | low 32 bits |
| `0` (or low 32 bits `0`) | `true` | the time in seconds |
| negative (also a computed `-0`), `NaN`, `±Infinity`, missing | `false` | generator unchanged |

### Values

| Expression | Result |
|------------|--------|
| `rnd.seed(12345); rnd.next()` | `0.9296160866506398` |
| then `rnd.toInteger()` | `3992670690` |
| then `rnd.next()` | `0.8901547130662948` (`3823185381`) |
| then `rnd.next()` | `0.31637556036002934` (`1358822685`) |
| `rnd.seed(1); rnd.next(); rnd.toInteger()` | `1791095845` (as `std::mt19937(1)`) |
| `rnd.seed(5489); rnd.next(); rnd.toInteger()` | `3499211612` (as `std::mt19937()`) |
| `rnd.seed(12345); rnd.toInteger()` | `12345` (no `next()` yet) |
| `"" + rnd`, `Console.writeLn(rnd)` | `"Random"` |
| `Convert.toNumber(rnd)` | `NaN` |
| `new Random().next() == new Random().next()` | `true` within the same second |

### Errors

| Message | Cause |
|---------|-------|
| `Unable to open "Random"` | the extension library was not found and no internal one is registered |
| `invalid parameter` | a method called with a `this` that is not a `Random` |
| `setPropertyBySymbol` | assigning a property to a `Random` value |

`seed` reports a bad argument by returning `false`, not by throwing.

### Not defined

Static `Random.next()`, ranges, 53-bit floats, distributions, copying a
generator from script, a cryptographically secure generator.

## C++

Namespace `XYO::QuantumScript::Extension::Random`, umbrella header
`<XYO/QuantumScript.Extension/Random.hpp>`.

### Library (`Random/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"Random"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (not in static builds) |

### Context (`Random/Context.hpp`)

| Symbol | Notes |
|--------|-------|
| `class RandomContext` | `Symbol symbolFunctionRandom`, `TPointerX<Prototype> prototypeRandom` |
| `RandomContext *getContext()` | per-thread singleton |

### Value (`Random/VariableRandom.hpp`)

| Symbol | Notes |
|--------|-------|
| `class VariableRandom : public Variable` | the script `Random` value |
| `RandomMT value` | the generator (`XYO::Cryptography::RandomMT`) |
| `static Variable *newVariable()` | new value from the active pool, seeded from the clock |
| `String getVariableType()` | `"Random"` |
| `Variable *instancePrototype()` | `Random.prototype` |
| `Variable *clone(SymbolList &)` | copy with the same generator state |
| `bool toBoolean()` | `true` |
| `String toString()` | `"Random"` |
| `TIsType<VariableRandom>(v)` | type test (GUID `{A2D9B22E-4185-45CE-BA5D-40989BD2947A}`) |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::Random::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--random` | fabricare project, `dll-or-lib`; depends on `quantum-script`, `quantum-script--console`, `xyo-cryptography` |
| `test.01` | fabricare test project, runs `test/test.01.js` |
| `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__RANDOM_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_LIBRARY` | static library: empty export macro, no DLL entry point |
