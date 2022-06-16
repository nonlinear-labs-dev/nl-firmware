import * as process from "process";
import { generateOutputFor, generateConfig } from "./tasks/config";
import { generateDeclarations } from "./tasks/declarations";

const timestamp = new Date();

try {
    const result = generateConfig(timestamp);
    generateDeclarations(result);
    generateOutputFor("./src/main.cpp", result, "./generated/main.cpp");
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}