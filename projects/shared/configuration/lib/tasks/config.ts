import * as fs from "fs";
import * as yaml from "js-yaml";

function parseYaml(filename: string) : Object {
    return { config: yaml.load(fs.readFileSync(filename, "utf-8"), { filename }) }
}

export function generateOutput(filename: string, result: Object) : string {
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

export function generateConfig(timestamp: Date) : Object {

    const
        result = {
            timestamp,
            ...parseYaml("./src/config.yaml")
        },
        config = generateOutput("./src/config.h.in", result);

    fs.writeFileSync("./generated/config.h", config);

    return result;

}