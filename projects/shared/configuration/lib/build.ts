import * as process from "process";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";

type Result = ConfigType & DeclarationsType & {
    timestamp: Date;
    parameters: string;
};

try {
    const result: Result = {
        timestamp: new Date(),
        parameters: "",
        ...ConfigParser.parse("./src/config.yaml"),
        ...DeclarationsParser.parse("./src/declarations.yaml")
    };
    result.parameters = Object.keys(result.declarations.parameter_type).reduce((out: Array<string>, key: string) => {
        if(key !== "None") {
            out.push(`enum class ${key}s {`, "  _LENGTH_", "};");
        }
        return out;
    }, []).join("\n");
    generateOutputFile("./src/config.h.in", "./generated/config.h", result);
    generateOutputFile("./src/declarations.h.in", "./generated/declarations.h", result);
    generateOutputFile("./src/main.cpp", "./generated/main.cpp", result);
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
