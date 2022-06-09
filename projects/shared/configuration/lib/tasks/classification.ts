import * as fs from "fs";
import * as yaml from "js-yaml";

export function generateClassification() {

    const result = {
        timestamp: new Date(),
        classification: yaml.load(fs.readFileSync("./src/classification.yaml", "utf-8")),
        definitions: yaml.load(fs.readFileSync("./src/definitions.yaml", "utf-8"))
    };

    console.log(result);

}