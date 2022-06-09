import * as process from "process";
import * as fs from "fs";
import * as yaml from "js-yaml";

const result = {
    timestamp: new Date(),
    config: yaml.load(fs.readFileSync("./src/config.yaml", "utf-8"))
};

const config = fs.readFileSync("./src/config.in.h", "utf-8").replace(/\$\{([^\}]*)\}/g, (_, keys) => {
    let known = result;
    keys.split(".").forEach((key) => {
        if(key in known) {
            known = known[key];
        } else {
            throw new Error(`build.ts: unknown key "${key}"`);
        }
    });
    return known.toString();
});

fs.writeFileSync("./generated/config.h", config);

process.exit(0);