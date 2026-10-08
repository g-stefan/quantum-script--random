---
name: quantum-script--random
description: >-
  How to use the Quantum Script Random extension (quantum-script--random),
  the seedable pseudo-random generator loaded with
  Script.requireExtension("Random"): new Random() / Random() (MT19937, the
  algorithm of std::mt19937, seeded with time() in seconds, arguments
  ignored), Random.prototype.next() (advances, returns [0, 1)), toInteger()
  (last value as a 32-bit integer), toNumber() (last value / 2^32),
  toString() (last value as text), seed(x) (returns true / false; 0 means
  "seed from the clock", fractional truncated, low 32 bits used, negative /
  NaN / Infinity / missing return false). Covers the value model (read
  methods do not advance; right after seed or new the value is the seed, so
  call next() first), clock seeds repeating within a second, "" + rnd being
  "Random", reference semantics, not secure (never for keys / tokens), the
  comparison with Math.random(), recipes (integer range, element, weighted
  choice, Fisher-Yates shuffle, random string with substring(start, length),
  temp file names, reproducible runs, Pixel32 noise / perlinNoiseWrapBox);
  the C++ side (registerInternalExtension, initExecutive,
  quantumScriptExtension, VariableRandom with RandomMT value, TIsType, using
  a script Random from another extension). Use when writing or reviewing
  Quantum Script code that uses Random, porting JavaScript code that needs
  seeded random numbers, C++ code that includes
  <XYO/QuantumScript.Extension/Random.hpp> or VariableRandom.hpp, a
  fabricare.json depending on "quantum-script--random", or when working
  inside the quantum-script--random repository.
---

# quantum-script--random

`Random` extension of Quantum Script (see the `quantum-script` skill for the
language and its differences from JavaScript; its rules apply). Purpose:
**give scripts a seedable random generator** — each `Random` object is an
independent Mersenne Twister MT19937 (32-bit) whose sequence is fixed by
`seed(x)`, identical on every run and platform and equal to C++
`std::mt19937`. The core has no random numbers; `Math.random()` (Math
extension) is C `rand()`, unseedable and coarse.

Full documentation: `docs/` in the quantum-script--random repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--random\docs` on this machine):
README (purpose, comparison with Math.random), getting-started (build, load,
hosts, C++ registration, static builds, threads), **script-api** (methods,
value model, seeding, recipes), cpp-api (VariableRandom, use from other
extensions), reference. When in doubt read
`source/XYO/QuantumScript.Extension/Random/Library.cpp` — it is short. The
generator is `XYO::Cryptography::RandomMT` (xyo-cryptography `docs/random.md`).

## Script API

```javascript
Script.requireExtension("Random");

var rnd = new Random();     // or Random(); seeded with time() in SECONDS; arguments ignored
rnd.seed(12345);            // true; fixed sequence
rnd.next();                 // 0.9296160866506398  advances, returns value / 2^32 in [0, 1)
rnd.toInteger();            // 3992670690          last value, [0, 4294967295], no advance
rnd.toNumber();             // 0.9296160866506398  last value / 2^32, no advance
rnd.toString();             // "0.9296160866506398" no advance
```

## Hard rules

1. **Load it first.** `Script.requireExtension("Random");` in every script
   and every thread (one engine per thread). Not preloaded in fabricare;
   magnet registers it internally; `Pixel32` and `SSHRemote` load it.
   Missing → `Unable to open "Random"`.
2. **`next()` is the only method that advances.** `toInteger()`,
   `toNumber()`, `toString()` re-read the last value. For several integers:
   `rnd.next(); a = rnd.toInteger(); rnd.next(); b = rnd.toInteger();`.
3. **Call `next()` before reading.** After `seed(x)` or `new Random()` the
   current value is the seed: `seed(12345); toInteger()` → `12345`.
4. **`seed(x)` rules** (x through `toNumber`): integer `1..4294967295` →
   that sequence; fractional → truncated (`seed(7.9)` = `seed(7)`); `>= 2^32`
   → low 32 bits (`seed(4294967297)` = `seed(1)`); **`0` → clock seed, not
   "seed 0"**; negative, computed `-0`, `NaN`, `Infinity`, missing argument,
   non-number string → returns **`false`**, generator unchanged (no throw).
   Check the result for external input.
5. **Clock seeds repeat within a second.** Two `new Random()` in the same
   second give the same numbers (`new Random().next() == new Random().next()`
   is `true`). Seed each stream / thread explicitly
   (`rnd.seed(DateTime.timestampInMilliseconds() + k)` needs `DateTime`).
6. **Not secure.** MT19937 is predictable after 624 outputs; never use it for
   keys, passwords, salts, tokens, session ids, nonces. Scripts have no
   secure generator: produce secrets in C++ with
   `XYO::Cryptography::SystemRandom`.
7. **Text conversion differs from the method.** `"" + rnd`,
   `Console.writeLn(rnd)`, `Convert.toString(rnd)` → `"Random"` (type name);
   `Convert.toNumber(rnd)` → `NaN`. Print `rnd.toString()` or `rnd.next()`.
8. `typeof(rnd)` is `"Random"`, `instanceof Random` true,
   `Script.isObject(rnd)` false; assigning a property throws
   `setPropertyBySymbol`. Assignment shares the generator (`b = a;
   b.next()` advances `a`). There is no script-level copy of the state.
9. Methods on a non-`Random` `this` throw `invalid parameter`. No static
   `Random.next()`, no ranges, no distributions: build them from `next()`.
10. Integer ranges need `Math.floor` (load `Math`) or `rnd.toInteger() % n`.
    Quantum Script strings cannot be indexed (`s[i]` is `undefined`) and
    `substring(start, length)` takes a **length**.

## Recipes

```javascript
Script.requireExtension("Math");
Script.requireExtension("Random");

function randomInt(rnd, min, max) {                 // inclusive
	return min + Math.floor(rnd.next() * (max - min + 1));
};
var item = list[Math.floor(rnd.next() * list.length)];
if (rnd.next() < 0.25) { /* 25% chance */ };
function shuffle(rnd, list) {                       // Fisher-Yates on a copy
	var out = [];
	for (var k = 0; k < list.length; ++k) { out[k] = list[k]; };
	for (var i = out.length - 1; i > 0; --i) {
		var j = Math.floor(rnd.next() * (i + 1));
		var t = out[i]; out[i] = out[j]; out[j] = t;
	};
	return out;
};
function randomString(rnd, length, alphabet) {      // test data, NOT passwords
	var out = "";
	for (var k = 0; k < length; ++k) {
		out += alphabet.substring(Math.floor(rnd.next() * alphabet.length), 1);
	};
	return out;
};
```

Reproducible run: compute `seed = DateTime.timestampInMilliseconds() %
4294967296` (use `1` if `0`), log it, `rnd.seed(seed)`; rerun with the
logged seed. Weighted choice: see `docs/script-api.md`.

Pixel32: `image.noise(rnd)`, `image.noise2Bit(rnd)`,
`Pixel32.perlinNoiseWrapBox(lx, ly, octaves, rnd)` draw from the script
generator (one `next()` per pixel); a seeded `rnd` gives the same texture
every run; a non-`Random` argument is silently ignored.

Known values (for tests): `seed(12345)` → `next()` `0.9296160866506398` /
`3992670690`, then `3823185381`, `1358822685`; `seed(1)` → `1791095845`;
`seed(5489)` → `3499211612`.

## C++

```cpp
#include <XYO/QuantumScript.Extension/Random.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                   // host init callback
	Extension::Random::registerInternalExtension(executive);    // scripts still requireExtension("Random")
};
```

- fabricare.json dependency: `"quantum-script--random"` (`dll-or-lib`: DLL
  on dynamic platforms, static lib on `*.static` platforms; pulls in
  `quantum-script`, `quantum-script--console`, `xyo-cryptography`). There is
  no `.static` project. Static hosts must register the extension as
  internal.
- DLL entry point `extern "C" quantumScriptExtension(Executive *, void *)`
  only with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` and without
  `XYO_QUANTUMSCRIPT_EXTENSION_RANDOM_LIBRARY`.
- `initExecutive` registers the global function `Random` (returns
  `VariableRandom::newVariable()`, prototype kept in the per-thread
  `RandomContext`) and the methods with
  `setFunction2("Random.prototype.next()", nextRandom)` etc.
- `VariableRandom : Variable` holds `RandomMT value`; active pool, each
  allocation runs `value.seed(0)` (clock). `clone` copies the state.
  `getVariableType()` / `toString()` → `"Random"`.
- Using a script generator from another extension (as Pixel32 does):
  include `<XYO/QuantumScript.Extension/Random/VariableRandom.hpp>`, test
  `TIsType<Extension::Random::VariableRandom>(arg)`, then use
  `((VariableRandom *)arg)->value.nextRandom()` — this advances the
  script's generator.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console and Random as internal extensions and runs
  `test/test.01.js`; run `make` first), `fabricare install` (see the
  `fabricare` skill). `quantum-script`, `quantum-script--console` and
  `xyo-cryptography` must be installed first. With the SDK installed a script
  runs directly: `quantum-script file.js` (uses the installed
  `quantum-script--random.dll`).
- Methods live in `Random/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`,
  check `TIsType<VariableRandom>(this_)` (throw `Error("invalid parameter")`),
  registered in `initExecutive` with
  `executive->setFunction2("Random.prototype.name(x)", name)`.
- The sequence for a seed, the `/ 4294967296.0` scaling and the seed
  truncation are a contract (scripts, Pixel32 textures, std::mt19937
  compatibility). `VariableRandom`'s layout is used by
  `quantum-script--pixel32`.
- New or changed methods: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `test/test.01.js` and this skill.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX: MIT for `source/` and `docs/`, Unlicense
  for `test/` and `.claude/` (see `.reuse/dep5`).
