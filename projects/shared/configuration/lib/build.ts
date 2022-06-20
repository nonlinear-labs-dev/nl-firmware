import * as process from "process";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";

type Result = { timestamp: Date; } & ConfigType & DeclarationsType;

try {
    const result : Result = {
        timestamp: new Date(),
        ...ConfigParser.parse("./src/config.yaml"),
        ...DeclarationsParser.parse("./src/declarations.yaml")
    };
    generateOutputFile("./src/config.h.in", "./generated/config.h", result);
    generateOutputFile("./src/declarations.h.in", "./generated/declarations.h", result);
    generateOutputFile("./src/main.cpp", "./generated/main.cpp", result);
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
