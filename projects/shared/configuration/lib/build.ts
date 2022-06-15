import * as process from "process";
import { generateOutputFor, generateConfig } from "./tasks/config";
// import { generateClassification } from "./tasks/classification";

const timestamp = new Date();

try {
    const
        result = generateConfig(timestamp);
    generateOutputFor("./src/main.cpp", result, "./generated/main.cpp");
    // generateClassification();
} catch(err) {
    console.error(err);
    process.exit(1);
}