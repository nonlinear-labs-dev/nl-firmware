// import * as fs from "fs";
import * as yaml from "js-yaml";
import { namespace, YamlResult, parseYaml, generateOutputFor } from "./config";

class Enum {
    entries: Array<[string, Object]>;
    constructor(value: Object) {
        this.entries = Object.entries(value);
    }
    getTokens() {
        return this.entries.filter(([, value]) => value["token"] === true).map(([name]) => name);
    }
}

const schema = yaml.DEFAULT_SCHEMA.extend([
    new yaml.Type(`${namespace}:declarations`, { kind: "mapping", resolve(value) {
        if(value === null || value === undefined) return false;
        return true;
    } }),
    new yaml.Type(`${namespace}:enum`, {
        kind: "mapping",
        resolve(value) {
            if(value === null || value === undefined) return false;
            return true;
        },
        construct(value) {
            return new Enum(value);
        }
    })
]);

export function generateDeclarations(result: YamlResult) {
    Object.assign(result, parseYaml("./src/declarations.yaml", schema, "declarations"));
    const
        parameter_types = result.declarations.parameter_type.getTokens(),
        smoother_sections = result.declarations.smoother_section.getTokens(),
        smoother_clocks = result.declarations.smoother_clock.getTokens(),
        parameter_signals = result.declarations.parameter_signal.getTokens();
    result.enums.parameter_type = parameter_types.join(",\n");
    result.enums.smoother_section = smoother_sections.join(",\n");
    result.enums.smoother_clock = smoother_clocks.join(",\n");
    result.enums.parameter_signal = parameter_signals.join(",\n");
    result.enums.smoother_scale = result.declarations.smoother_scale.getTokens().join(",\n");
    result.param_enums = parameter_types.filter((type) => type !== "None").map((type) => `enum class ${type}s\n{};`).join("\n");
    result.smoother_enums = smoother_sections.filter((type) => type !== "None").map((type) => {
        return smoother_clocks.map((clock) => `enum class ${type}_${clock}\n{};`).join("\n");
    }).join("\n");
    result.signal_enums = parameter_signals.filter((type) => type !== "None").map((type) => `enum class ${type}s\n{};`).join("\n");
    generateOutputFor("./src/declarations.in.h", result, "./generated/declarations.h");
}
