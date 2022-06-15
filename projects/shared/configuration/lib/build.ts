import * as fs from "fs";
import * as process from "process";
import { generateOutput, generateConfig } from "./tasks/config";
// import { generateClassification } from "./tasks/classification";

const timestamp = new Date();

try {
    const
        result = generateConfig(timestamp),
        main = generateOutput("./src/main.cpp", result);
    fs.writeFileSync("./generated/main.cpp", main);
    // generateClassification();
} catch(err) {
    console.error(err);
    process.exit(1);
}