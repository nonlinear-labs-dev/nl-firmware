# YAML Experiments

An attempt to re-implement (and extend) the "parameter-db" sub-project on a [YAML](https://yaml.org/) basis, providing essential C15 definitions as shared header files (usable wherever they are needed).

## Prerequisites

- required programs: node (currently using v16.15.1), npm (8.10.0), tsc (4.6.4)
- on first run (?): `npm init -y` _(as the package.json is part of the repo, an npm install may suffice?)_
- packages: `npm install @types/node @types/js-yaml js-yaml`

## Run Generation Process

``` bash
tsc ./blid/build.ts && node ./lib/build.js
```

## Roadmap

- working proof of concept
- "dockerize" project and integrate into make process