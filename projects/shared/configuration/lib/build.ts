import * as process from "process";
import * as fs from "fs";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";
import { DefinitionsType, SignalType, DefinitionsParser } from "./tasks/definitions";

type Result = ConfigType & DeclarationsType & {
    timestamp: Date;
    parameters: string;
    smoothers: string;
    definitions: Array<DefinitionsType>;
    signals: string;
    pid: string;
};

type ParamType = {
    [key: string]: Array<string>;
};

const indent = " ".repeat(4);

function processDefinitions(result: Result) {
    const
        err = "processDefinitions error",
        params: Array<number | string> = [],
        pid: Array<string> = [],
        parameterType = Object.keys(result.declarations.parameter_type).reduce((out: ParamType, type: string) => {
            if(type !== "None") out[`${type}s`] = [];
            return out;
        }, {}),
        smootherType =  Object.keys(result.declarations.smoother_section).reduce((out: SignalType, section: string) => {
            if(section !== "None") {
                Object.keys(result.declarations.smoother_clock).forEach((clock: string) => {
                    out[`${section}_${clock}`] = [];
                });
            }
            return out;
        }, {}),
        signalType =  Object.keys(result.declarations.parameter_signal).reduce((out: SignalType, type: string) => {
            if(type !== "None") out[`${type}s`] = [];
            return out;
        }, {}),
        parameterList: Array<string> = [];
    result.definitions.forEach((definition) => {
        const
            group = {
                name: definition.group,
                data: result.declarations.parameter_group[definition.group]
            };
        if(!group.data) {
            throw new Error(`${err}: unknown group token "${group.name}"`);
        }
        definition.parameters.forEach((parameter, index) => {
            const
                type = { name: parameter.type, data: result.declarations.parameter_type[parameter.type] },
                {
                    // required
                    token, id, label_long, label_short, control_position, info, availability,
                    // optional
                    return_behavior, modulation_amount, bipolar, rendering_args
                } = parameter;
            if(type.data === undefined) {
                throw new Error(`${err}: unknown parameter type "${type.name}"`);
            }
            type.data.forEach((property) => {
                if(!result.declarations.parameter_properties.includes(property)) {
                    throw new Error(`${err}: unknown parameter type property "${property}"`);
                }
            });
            [token, id, label_long, label_short, control_position, info, availability].forEach((property) => {
                if(property === undefined) {
                    throw new Error(`${err}: insufficient parameter definition in group "${group.name}" element ${index + 1}`);
                }
            });
            const tokenStr = type.data.includes("group_label") ? `${group.name}_${token}` : token;
            if(id < 0 || id > 16382) {
                throw new Error(`${err}: parameter id ${id} is out of tcd range [0 ... 16382]`);
            }
            if(params.includes(id)) {
                throw new Error(`${err}: parameter id ${id} is already defined`);
            }
            if(params.includes(tokenStr)) {
                throw new Error(`${err}: parameter token ${tokenStr} is already defined`);
            }
            params.push(id, tokenStr);
            pid[id] = `${tokenStr} = ${id}`;
            parameterType[`${type.name}s`].push(tokenStr);
            if(type.data.includes("return_behavior")) {
                if(return_behavior === undefined) {
                    throw new Error(`${err}: parameter id ${id} of type "${type.name}" requires return_behavior`);
                }
            }
            if(type.data.includes("modulation_amount")) {
                if(modulation_amount === undefined) {
                    throw new Error(`${err}: parameter id ${id} of type "${type.name}" requires modulation_amount`);
                }
            }
            if(bipolar) {}
            if(type.data.includes("rendering_args")) {
                if(rendering_args === undefined) {
                    throw new Error(`${err}: parameter id ${id} of type "${type.name}" requires rendering_args`);
                }
            }
        });
    });
    result.parameters = Object.entries(parameterType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries, "_LENGTH_"].join(`,\n${indent}`)}\n};`);
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
    result.enums.pid = ["None = -1", ...pid.filter((id) => id !== undefined)].join(",\n");
}

try {
    const
        definitionsPath = "./src/definitions",
        definitions = fs.readdirSync(definitionsPath).map((filename: string) => {
            return `${definitionsPath}/${filename}`;
        }).filter((filename: string) => {
            return fs.statSync(filename).isFile();
        }),
        result: Result = {
            timestamp: new Date(), parameters: "", smoothers: "", signals: "", pid: "",
            ...ConfigParser.parse("./src/config.yaml"),
            ...DeclarationsParser.parse("./src/declarations.yaml"),
            definitions: DefinitionsParser.parseAll(...definitions)
        };
    processDefinitions(result);
    generateOutputFile("./src/config.h.in", "./generated/config.h", result);
    generateOutputFile("./src/declarations.h.in", "./generated/declarations.h", result);
    generateOutputFile("./src/definitions.h.in", "./generated/definitions.h", result);
    generateOutputFile("./src/descriptor.h.in", "./generated/descriptor.h", result);
    generateOutputFile("./src/placeholder.h.in", "./generated/placeholder.h", result);
    generateOutputFile("./src/main.cpp.in", "./generated/main.cpp", result);
    // not covered by g++ and therefore unsafe
    generateOutputFile("./src/ParameterFactory.java.in", "./generated/ParameterFactory.java", result);
    generateOutputFile("./src/MacroIds.js.in", "./generated/MacroIds.js", result);
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
