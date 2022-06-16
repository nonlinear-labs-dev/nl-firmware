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
    result.enums.parameter_type = result.declarations.parameter_type.getTokens().join(",\n");
    result.enums.smoother_section = result.declarations.smoother_section.getTokens().join(",\n");
    result.enums.smoother_clock = result.declarations.smoother_clock.getTokens().join(",\n");
    result.enums.parameter_signal = result.declarations.parameter_signal.getTokens().join(",\n");
    result.enums.smoother_scale = result.declarations.smoother_scale.getTokens().join(",\n");
    generateOutputFor("./src/declarations.in.h", result, "./generated/declarations.h");
}
