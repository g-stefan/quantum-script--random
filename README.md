# Quantum Script Extension Random

Quantum Script extension
- A seedable pseudo-random number generator for Quantum Script, which has
none in its core: `new Random()` creates an independent Mersenne Twister
(MT19937, 32-bit) generator, the same algorithm as C++ `std::mt19937`.
- `seed(x)` gives the same sequence on every run and platform: tests,
replays, procedural content, `Pixel32` noise textures. Seeded from the clock
by default.
- Not cryptographically secure: never use it for keys, passwords or tokens.

```javascript
Script.requireExtension("Random");

Random();
Random.prototype.next();
Random.prototype.toInteger();
Random.prototype.toNumber();
Random.prototype.toString();
Random.prototype.seed(x);
```

Built on `quantum-script` and `xyo-cryptography`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose, comparison with `Math.random()`
- [Getting started](docs/getting-started.md) - build, load from a script, hosts, register in a C++ host
- [Script API](docs/script-api.md) - methods, value model, seeding rules, recipes
- [C++ API](docs/cpp-api.md) - registration, DLL entry point, `VariableRandom`, use from other extensions
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--random](.claude/skills/quantum-script--random/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
