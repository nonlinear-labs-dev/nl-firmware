// import * as fs from "fs";
import * as yaml from "js-yaml";
import { namespace, YamlResult, parseYaml, generateOutputFor } from "./config";

class Enum extends Array<[string, Object]> {
    constructor(value : Object) {
        super(...Object.entries(value));
    }
    getTokens() {
        return this.filter(([, value]) => (value as Object)["token"] === true);
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
    generateOutputFor("./src/declarations.in.h", result, "./generated/declarations.h");
    console.log((result.declarations || {})["parameter_type"]);

    // const result = {
    //     timestamp: new Date(),
    //     classification: yaml.load(fs.readFileSync("./src/classification.yaml", "utf-8")),
    //     definitions: yaml.load(fs.readFileSync("./src/definitions.yaml", "utf-8"))
    // };

    // console.log(result.classification);
    // console.log(result);

}