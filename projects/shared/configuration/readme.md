# C15 Configuration

An attempt to re-implement (and extend) the "parameter-db" sub-project on a [YAML](https://yaml.org/) basis, providing essential C15 definitions as shared header files (usable wherever they are needed). The current project name is "configuration", as we define more than just parameters here.

## Prerequisites

- required programs: `node` _(currently using v16.15.1, at least 14.14-2)_, `npm` _(8.10.0)_, `tsc` _(4.6.4)_, `g++` _(11.2.0)_, `podman` _(3.4)_
- required node packages: `@types/node @types/js-yaml js-yaml` or better `@types/node@14.14-2 @types/js-yaml js-yaml`
- on first run: `npm install` _(the provided package.json should suffice to install the dependencies into the git-ignored `node_modules` folder)_
- `mkdir ./generated` _(the current output directory - which is git-ignored)_

### how to install PodMan on ubuntu (22.04)

https://www.atlantic.net/dedicated-server-hosting/how-to-install-and-use-podman-on-ubuntu-20-04/

from 22.10 onwards it should be a default package in apt

### configuring in VS Code

VS Code may have problems identifying `*.in` files, which results in syntax highlighting and other helpers not working properly.
In order to fix that, create a (git-ignored) `.vscode/settings.json` file within this folder (projects/shared/configuration) and paste the following:

```json
{
    "files.associations": {
        "*.cpp.in": "cpp",
        "*.h.in": "cpp",
        "*.html.in": "html",
        "*.java.in": "java",
        "*.js.in": "javascript"
    }
}
```

At least, syntax highlighting should work - but the error messages do not appear to be relevant and can be ignored.

## Run Generation Process

``` bash
bash process.sh
```

## Process Validation

Yaml possesses features for custom tags and validation. However, implementing a validation based on Yaml would result in quite complex and verbose typescript and yaml files, which doesn't seem to provide any benefit. Therefore, the strategy is to perform an additional g++ compilation with the generated headers, which provides validation and raises informative errors when failing.

The process consists of four distinct phases, each of which can fail on invalid input:

1. Typescript compilation _(validating the generation process)_
2. Node execution _(parsing and validating yaml files, generating usable output files)_
3. G++ compilation _(validating generated headers, providing executable based on generated files)_
4. "Main" execution _(additional validation of generated structures)_

In addition, the project has passed a one-time verification (`bash validate.sh`), comparing the generated structures to the now deprecated parameter-db ones.

## Compatibility with ParameterDb

The content of provided files aims for seamless exchange. However, there is one file in the ParameterDb that is deprecated/removed here:
`parameter_info.h` moved into `parameter_declarations.h`. Besides that, the provided and validated code should match the ParameterDb.

## Status / Roadmap

- [x] parse yaml to desired output: .h, .cpp
- [x] handling of several `*.in.*` files
- [x] handling of several definition.yaml files
- [x] auto-counting of parameters
- [x] good-enough error messages
- [x] generate overview _(similar to spreadsheet: parameter counts, parameter id table)_
- [x] working proof of concept
- [x] complete C15 definition
  - [x] Global: ModMatrix (HW Sources and Amounts, Macro Controls and Times), Master, Scale
  - [x] Split, Part, Unison, Mono
  - [x] Envelopes A, B, C, (G)
  - [x] Oscillators and Shapers
  - [x] Comb and SV Filter
  - [x] Feedback and Output Mixer
  - [x] Effects: Flanger, Cabinet, Gap Filter, Echo, Reverb
- [x] order: parameter groups and signals should be in order [^1]
- [x] verification: every (non-empty) ParameterDescriptor of ParameterDb should be identical to Configuration clone [^2]
- [ ] currently unclear: integration of other shared resources (f.e. nltools) in generated output?
- [ ] there seems to be a `LF/CR` "problem", on github the last empty line in each file is omitted
- [x] "dockerize" project and integrate into make process
- [ ] review:
  - can code be simplified/generalized further?
    - [ ] yaml anchors (`&ANCHOR`) are currently used for keeping yaml as concise as possible --> use custom yaml.Type instead and delegate boilerplate to typescript? _(unclear where to draw the line between ./src and ./lib)_
  - can validation and generation of error messages be more informative?
    - [x] yaml parsing and g++ errors give decent indication of what went wrong
    - [x] node validation already does lots of sanity checks
    - [x] parameter definition errors at least provide a filename and reason
- possible project enhancements
  - [ ] more PlayGround definitions _(ParameterAppearance?)_
    - [x] Enums and Descriptors: DisplayScalingType, ParameterGroup
  - [ ] turn descriptors into constexpr decscriptors
  - [ ] content for java and js resources
  - [ ] integrate clang-format _(for generated output)_?
  - [ ] provide hw sources as c header for PlayController
  - [ ] generate html artifacts for parameter-reference (manual, webui)?
  - [ ] generate css artifacts _(parameter-group colors)_
  - [ ] potentially delegate Configuration <--> NlTools _(enums: MC, SoundType, VoiceGroup, Preset Messages, Settings?)_

  [^1]: group order is determined by parameter_group declaration, signal order follows group order (but explicit signals are not in element order)
  [^2]: not covered in verification: smoother/signal ids, char pointers _(playground: labels, inactive_cp - they would require string comparisons)_
