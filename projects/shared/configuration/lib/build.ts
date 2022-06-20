import * as process from "process";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";
import { DefinitionsType, DefinitionsParser } from "./tasks/definitions";

type Result = ConfigType & DeclarationsType & {
    timestamp: Date;
    parameters: string;
    smoothers: string;
    definitions: Array<DefinitionsType>;
    signals: string;
    pid: string;
};

const indent = " ".repeat(4);

try {
    const result: Result = {
        timestamp: new Date(), parameters: "", smoothers: "", signals: "", pid: "",
        ...ConfigParser.parse("./src/config.yaml"),
        ...DeclarationsParser.parse("./src/declarations.yaml"),
        definitions: DefinitionsParser.parseAll()
    };
    result.parameters = Object.keys(result.declarations.parameter_type).reduce((out: Array<string>, key: string) => {
        if(key !== "None") {
            out.push(`enum class ${key}s {`, `${indent}_LENGTH_`, "};");
        }
        return out;
    }, []).join("\n");
    result.smoothers = Object.keys(result.declarations.smoother_section).reduce((out: Array<string>, smoother: string) => {
        if(smoother !== "None") {
            Object.keys(result.declarations.smoother_clock).forEach((clock: string) => {
                if(clock !== "None") {
                    out.push(`enum class ${smoother}_${clock} {`, `${indent}_LENGTH_`, "};")
                }
            });
        }
        return out;
    }, []).join("\n");
    result.signals = Object.keys(result.declarations.parameter_signal).reduce((out: Array<string>, key: string) => {
        if(key !== "None") {
            out.push(`enum class ${key}s {`, `${indent}_LENGTH_`, "};");
        }
        return out;
    }, []).join("\n");
    result.enums.pid = ["None = -1"].join(",\n");
    generateOutputFile("./src/config.h.in", "./generated/config.h", result);
    generateOutputFile("./src/declarations.h.in", "./generated/declarations.h", result);
    generateOutputFile("./src/definitions.h.in", "./generated/definitions.h", result);
    generateOutputFile("./src/descriptor.h.in", "./generated/descriptor.h", result);
    generateOutputFile("./src/placeholder.h.in", "./generated/placeholder.h", result);
    generateOutputFile("./src/main.cpp", "./generated/main.cpp", result);
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
