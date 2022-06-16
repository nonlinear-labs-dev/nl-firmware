import * as fs from "fs";
import * as yaml from "js-yaml";

export const namespace = "tag:nonlinear-labs.de,2022:js-yaml";

const schema = yaml.DEFAULT_SCHEMA.extend([
    new yaml.Type(`${namespace}:config`, { kind: "mapping", resolve(value) {
        if(value === null || value === undefined) return false;
        return true;
    } })
]);

export type YamlResult = {
    timestamp?: Date;
    config?: Object;
    declarations: {
        [key: string]: any;
    };
    enums: {
        [key: string]: any;
    };
    param_enums: {
        [key: string]: any;
    };
    smoother_enums: {
        [key: string]: any;
    };
    signal_enums: {
        [key: string]: any;
    };
};

export function parseYaml(filename: string, schema: yaml.Schema, key: string) : YamlResult {
    return {
        declarations: {}, enums: {}, param_enums: {}, smoother_enums: {}, signal_enums: {},
        [key]: yaml.load(fs.readFileSync(filename, "utf-8"), { filename, schema })
    }
}

function generateOutput(filename: string, result: YamlResult) : string {
    return fs.readFileSync(filename, "utf-8").replace(/([ ]*)\$\{([^\}]*)\}/g, (_, ws = "", keys) => {
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
        return ws + known.toString().split("\n").join(`\n${ws}`);
    });
}

export function generateOutputFor(filename: string, result: YamlResult, output: string) {
    fs.writeFileSync(output, generateOutput(filename, result));
}

export function generateConfig(timestamp: Date) : YamlResult {
    const result = {
        timestamp,
        ...parseYaml("./src/config.yaml", schema, "config")
    };
    generateOutputFor("./src/config.h.in", result, "./generated/config.h");
    return result;
}
