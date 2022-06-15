# [YAML Experiments] C15 Configuration

An attempt to re-implement (and extend) the "parameter-db" sub-project on a [YAML](https://yaml.org/) basis, providing essential C15 definitions as shared header files (usable wherever they are needed). The current project name is "configuration", as we define more than just parameters here.

## Validation

Yaml possesses features for custom tags and validation. However, implementing a validation would result in quite complex and verbose typescript and yaml files, which doesn't seem to provide any benefit. Therefore, the strategy is to perform an additional gcc compilation with the generated headers, which provides validation and raises informative errors when failing.

## Prerequisites

- required programs: `node` _(currently using v16.15.1)_, `npm` _(8.10.0)_, `tsc` _(4.6.4)_, `g++` _(11.2.0)_
- required node packages: `@types/node @types/js-yaml js-yaml`
- on first run: `npm install` _(the provided package.json should suffice to install the dependencies into the git-ignored `node_modules` folder)_
- `mkdir ./generated` _(the current output directory - which is git-ignored)_

## Run Generation Process

``` bash
bash process.sh
```

## Status
- &#x2611; src/config.yaml, src/config.in.h --> generated/config.h
- &#x2610; src/classification.yaml, src/definitions.yaml --> ?

## Roadmap

- &#x2610; working proof of concept
- &#x2610; "dockerize" project and integrate into make process
- &#x2610; provide process safety (by yaml types) and detailed error messages