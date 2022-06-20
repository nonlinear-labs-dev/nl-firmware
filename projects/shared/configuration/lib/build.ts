import * as process from "process";
import * as fs from "fs";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";
import { DefinitionsType, DefinitionsParser } from "./tasks/definitions";
import { group } from "console";

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
        paramId: Array<number> = [],
        paramToken: Array<string> = [],
        pid: Array<string> = [],
        parameterType = Object.keys(result.declarations.parameter_type).reduce((out: ParamType, type: string) => {
            if(type !== "None") out[`${type}s`] = [];
            return out;
        }, {});
    result.definitions.forEach((definition) => {
        const group = {name: definition.group, data: result.declarations.parameter_group[definition.group]};
        if(!group.data) throw new Error(`${err}: unknown group token "${group.name}"`);
        definition.parameters.forEach((parameter) => {
            const
                type = { name: parameter.type, data: result.declarations.parameter_type[parameter.type] },
                { token, id, control_position, return_behavior, modulation_amount, bipolar, info, rendering } = parameter;
            if(!type.data) throw new Error(`${err}: unknown parameter type "${type.name}"`);
            if(id < 0 || id > 16382) throw new Error(`${err}: parameter id ${id} is out of tcd range [0 ... 16382]`);
            if(type.data.rendering_args && !rendering) throw new Error(`${err}: parameter id ${id} requires rendering args`);
            if(paramId.includes(id)) throw new Error(`${err}: parameter id ${id} is already defined`);
            const tokenStr = type.data.combined_label ? `${group.name}_${token}` : token;
            if(paramToken.includes(tokenStr)) throw new Error(`${err}: parameter token ${tokenStr} is already defined`);
            paramId.push(id);
            paramToken.push(tokenStr);
            pid[id] = `${tokenStr} = ${id}`;
            parameterType[`${type.name}s`].push(tokenStr);
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
    generateOutputFile("./src/main.cpp", "./generated/main.cpp", result);
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
