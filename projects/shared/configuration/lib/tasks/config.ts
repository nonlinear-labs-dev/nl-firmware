import * as fs from "fs";
import * as yaml from "js-yaml";

export const namespace = "tag:nonlinear-labs.de,2022:js-yaml";

const schema = yaml.DEFAULT_SCHEMA.extend([
    new yaml.Type(`${namespace}:config`, { kind: "mapping", resolve(value) {
        if(value === null || value === undefined) return false;
        return true;
    } })
]);

function parseYaml(filename: string) : Object {
    return { config: yaml.load(fs.readFileSync(filename, "utf-8"), { filename, schema }) }
}

function generateOutput(filename: string, result: Object) : string {
    return fs.readFileSync(filename, "utf-8").replace(/\$\{([^\}]*)\}/g, (_, keys) => {
        // a rather crude lookup mechanism
        let known = result;
        keys.split(".").forEach((key) => {
            if(key in known) {
                known = known[key];
            } else {
                // a rather crude error message
                throw new Error(`build.ts ${filename}: unknown key "${key}" in ${keys}`);
            }
        });
        return known.toString();
    });
}

export function generateOutputFor(filename: string, result: object, output: string) {
    fs.writeFileSync(output, generateOutput(filename, result));
}

export function generateConfig(timestamp: Date) : Object {

    const
        result = {
            timestamp,
            ...parseYaml("./src/config.yaml")
        };

    generateOutputFor("./src/config.h.in", result, "./generated/config.h");

    return result;

}
