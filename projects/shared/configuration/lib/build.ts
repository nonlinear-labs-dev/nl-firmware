import * as process from "process";
import * as fs from "fs";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";
import { DefinitionsType, SignalType, DefinitionsParser } from "./tasks/definitions";

// yaml parsing result type
type Result = ConfigType & DeclarationsType & {
    definitions: Array<DefinitionsType & {
        filename: string;
    }>;
    timestamp: Date;
    parameters: string;
    smoothers: string;
    signals: string;
    pid: string;
    parameter_list: string;
};

type ParamType = {
    [key: string]: Array<string>;
};

const indent = " ".repeat(4);

function validateToken(token: string): boolean {
    return /^\w+$/.test(token);
}

// validate (essential sanity checks) and transform parsed yaml output
function processDefinitions(result: Result) {
    const
        errmsg = "processDefinitions error",
        // parameter validation: unique id/token
        params: Array<number | string> = [],
        // parameterID collection (resulting in enum)
        pid: Array<string | undefined> = [],
        // parameterType collection (resulting in enum)
        parameterType = Object.keys(result.declarations.parameter_type).reduce((out: ParamType, type: string) => {
            if(!validateToken(type)) {
                throw new Error(`${errmsg}: invalid token for parameterType "${type}"`);
            }
            if(type !== "None") out[`${type}s`] = []; // every type !== None can collect tokens
            return out;
        }, {}),
        // smootherType collection (resulting in enum)
        smootherType = Object.keys(result.declarations.smoother_section).reduce((out: SignalType, section: string) => {
            if(!validateToken(section)) {
                throw new Error(`${errmsg}: invalid token for smootherSection "${section}"`);
            }
            if(section !== "None") {
                Object.keys(result.declarations.smoother_clock).forEach((clock: string) => {
                    if(!validateToken(clock)) {
                        throw new Error(`${errmsg}: invalid token for smootherClock "${clock}"`);
                    }
                    out[`${section}_${clock}`] = [];
                });
            }
            return out;
        }, {}),
        // signalType collection (resulting in enum)
        signalType = Object.keys(result.declarations.parameter_signal).reduce((out: SignalType, type: string) => {
            if(!validateToken(type)) {
                throw new Error(`${errmsg}: invalid token for parameterSignal "${type}"`);
            }
            if(type !== "None") out[`${type}s`] = [];
            return out;
        }, {});
    // automatic detection of number of params (highest parameter id)
    result.config.params = 1 + result.definitions.reduce((max, definition) => {
        return Math.max(max, Math.max(...definition.parameters.map((parameter) => {
            if(!Number.isInteger(parameter.id)) {
                throw new Error(
                    `${errmsg} in ${definition.filename}: parameter id "${parameter.id}" is invalid`
                );
            }
            if(parameter.id < 0 || parameter.id > 16382) {
                throw new Error(
                    `${errmsg} in ${definition.filename}: parameter id ${parameter.id} is out of tcd range [0 ... 16382]`
                );
            }
            return parameter.id;
        })));
    }, 0);
    // explicit signals (appended to implicit parameter signals)
    let explicitSignalIndex = result.config.params;
    // parameterList collection
    const
        processedGroups: Array<string> = [],
        parameterList: Array<string> = new Array<string>(result.config.params).fill("{None}");
    // for every yaml resource of ./src/definitions, providing a parameter group
    result.definitions.forEach(({filename, ...definition}) => {
        const
            err = `${errmsg} in ${filename}`,
            group = {
                name: definition.group,
                data: result.declarations.parameter_group[definition.group]
            };
        // group sanity checks
        if(group.data === undefined) {
            throw new Error(`${err}: unknown group token "${group.name}"`);
        }
        if(processedGroups.includes(group.name)) {
            throw new Error(`${err}: group "${group.name}" is already defined`);
        }
        processedGroups.push(group.name);
        // for every parameter of the group
        definition.parameters.forEach((parameter, index) => {
            const
                type = { name: parameter.type, data: result.declarations.parameter_type[parameter.type] },
                {
                    // required
                    token, id, label_long, label_short, control_position, info, availability,
                    // optional
                    return_behavior, modulation_aspects, rendering_args
                } = parameter;
            // type sanity checks
            if(type.data === undefined) {
                throw new Error(`${err}: unknown parameter type "${type.name}"`);
            }
            type.data.forEach((property) => {
                if(!result.declarations.parameter_properties.includes(property)) {
                    throw new Error(`${err}: unknown parameter type property "${property}"`);
                }
            });
            // property sanity checks
            [token, label_long, label_short, control_position, info, availability].forEach((property) => {
                if(property === undefined) {
                    throw new Error(`${err}: insufficient parameter definition in group "${group.name}" element ${index + 1}`);
                }
            });
            if(!validateToken(token)) {
                throw new Error(`${err}: invalid parameter token "${token}" in parameter id ${id}`);
            }
            // token and descriptor generation
            const
                tokenStr = type.data.includes("group_label") ? `${group.name}_${token}` : token,
                typeStr = `${type.name}s`,
                smootherDescriptor: Array<string> = [],
                playgroundDescriptor: Array<string> = [];
            // parameterId sanity checks
            if(params.includes(id)) {
                throw new Error(`${err}: parameter id ${id} is already defined`);
            }
            if(params.includes(tokenStr)) {
                throw new Error(`${err}: parameter token ${tokenStr} is already defined`);
            }
            // feed relevant enum buffers
            params.push(id, tokenStr);
            pid[id] = `${tokenStr} = ${id}`;
            parameterType[typeStr].push(tokenStr);
            // controlPosition properties
            const
                { coarse, fine, scale, initial, inactive } = control_position,
                displayScalingType = result.declarations.display_scaling_type[scale];
            // controlPosition sanity checks
            [coarse, fine, scale, initial].forEach((property) => {
                if(property === undefined) {
                    throw new Error(`${err}: insufficient control_position definition in paramter id ${id}`);
                }
            });
            // displayScalingType sanity checks
            if(displayScalingType === undefined) {
                throw new Error(`${err}: unknown DisplayScalingType "${scale}" in parameter id ${id}`);
            }
            const bipolar = displayScalingType.bipolar;
            // feed playgroundDescriptor
            playgroundDescriptor.push(coarse.toString(), fine.toString());
            // optional returnBehavior - currently unused
            if(type.data.includes("return_behavior")) {
                // returnBehavior sanity checks
                if(return_behavior === undefined) {
                    throw new Error(`${err}: parameter id ${id} of type "${type.name}" requires return_behavior`);
                }
            }
            // optional modulationAspects, feeding playgrounDescriptor
            if(type.data.includes("modulation_aspects")) {
                // modulationAspects sanity checks
                if(modulation_aspects === undefined) {
                    throw new Error(`${err}: parameter id ${id} of type "${type.name}" requires modulation_aspects`);
                }
                // modulationAspects properties
                const
                    { coarse, fine, scale } = modulation_aspects,
                    displayScalingType = result.declarations.display_scaling_type[scale];
                // modulationAspect property sanity checks
                [coarse, fine, scale].forEach((property) => {
                    if(property === undefined) {
                        throw new Error(`${err}: insufficient modulation_aspects definition in paramter id ${id}`);
                    }
                });
                if(displayScalingType === undefined) {
                    throw new Error(`${err}: unknown DisplayScalingType "${scale}" in modulation_aspects of parameter id ${id}`);
                }
                if(displayScalingType.bipolar !== true) {
                    throw new Error(`${err}: DisplayScalingType "${scale}" in modulation_aspects of parameter id ${id} can only be bipolar`);
                }
                playgroundDescriptor.push(coarse.toString(), fine.toString());
            } else {
                playgroundDescriptor.push("None", "None");
            }
            // feed playgroundDescriptor with parameter labels and info strings
            playgroundDescriptor.push(...[
                inactive || "", group.data.label_long, group.data.label_short, label_long, label_short, info.trim().replace(/\n/g, "\\n")
            ].map((entry) => `"${entry}"`));
            // optional renderingArgs (relevant for C15Synth only)
            if(type.data.includes("rendering_args")) {
                // renderingArgs sanity checks
                if(rendering_args === undefined) {
                    throw new Error(`${err}: parameter id ${id} of type "${type.name}" requires rendering_args`);
                }
                // renderingArgs properties
                const
                    { scaling, factor, offset, section, clock, signal } = rendering_args,
                    sectionType = `${section}_${clock}`;
                // optional smootherSection handling
                if(section !== "None") {
                    // clock sanity checks
                    if(clock === "None") {
                        throw new Error(`${err}: invalid smoother clock "${clock}" in parameter id ${id}`);
                    }
                    // renderingArgs property sanity checks
                    if(!smootherType[sectionType]) {
                        throw new Error(`${err}: invalid smoother "${sectionType}" in parameter id ${id}`);
                    }
                    // feed SmootherDescriptor
                    smootherDescriptor.push(`Smoothers::${sectionType}::${tokenStr}`);
                    // smootherType
                    smootherType[sectionType][id] = tokenStr;
                } else {
                    smootherDescriptor.push("None");
                }
                // optional parameterSignal, feeding SmootherDescriptor
                if(signal) {
                    if(signal !== "None") {
                        const signalStr = `${signal}s`;
                        // signalType sanity checks
                        if(signalType[signalStr] === undefined) {
                            throw new Error(`${err}: unknown signal "${signal}" in parameter id ${id}`);
                        }
                        if(signalType[signalStr].includes(tokenStr)) {
                            throw new Error(`${err}: signal "${signal}" in parameter id ${id} is already defined`);
                        }
                        signalType[signalStr][id] = tokenStr;
                        smootherDescriptor.push(`Signals::${signalStr}::${tokenStr}`);
                    } else {
                        smootherDescriptor.push("None");
                    }
                }
                // parameter scaling
                smootherDescriptor.push(
                    `Properties::SmootherScale::${scaling}`,
                    factor,
                    offset,
                    (bipolar || false).toString()
                );
            }
            // final parameterDescriptor
            const descriptor: Array<string> = [
                `Parameters::${typeStr}::${tokenStr}`,
                `PID::${tokenStr}`,
                initial,
                (
                    rendering_args === undefined ? "{}" :
                    `{\n${indent.repeat(2)}${smootherDescriptor.join(`,\n${indent.repeat(2)}`)}\n${indent}}`
                ),
                `{\n${indent.repeat(2)}${playgroundDescriptor.join(`,\n${indent.repeat(2)}`)}\n${indent}}`
            ];
            // feed parameterList
            parameterList[id] = [
                "{",
                descriptor.map((entry: string) => `${indent}${entry}`).join(",\n"),
                "}"
            ].join("\n");
        });
        // collect explicitly defined signals of parameter group
        if(definition.signals !== undefined) {
            Object.entries(definition.signals).forEach(([key, value]) => {
                const signalGroup = `${key}s`;
                if(signalType[signalGroup] === undefined) {
                    throw new Error(`${err}: unknown signal type "${key}" in group "${group.name}"`);
                }
                value.forEach((signal) => {
                    if(!validateToken(signal)) {
                        throw new Error(`${err}: invalid explicit signal token "${signal}"`);
                    }
                    const signalStr = `${group.name}_${signal}`;
                    if(signalType[signalGroup].includes(signalStr)) {
                        throw new Error(`${err}: signal "${signalStr}" in in group "${group.name}" is already defined`);
                    }
                    // append explicit signal (behind any potential implicit parameter signal)
                    signalType[signalGroup][explicitSignalIndex++] = signalStr;
                });
            });
        }
    });
    // provide string replacements
    result.parameter_list = parameterList.join(",\n");
    result.parameters = Object.entries(parameterType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries, "_LENGTH_"].join(`,\n${indent}`)}\n};`);
        return out;
    }, []).join("\n");
    result.smoothers = Object.entries(smootherType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries.filter((entry => entry !== undefined)), "_LENGTH_"].join(`,\n${indent}`)}\n};`);
        return out;
    }, []).join("\n");
    result.signals = Object.entries(signalType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries.filter((entry => entry !== undefined)), "_LENGTH_"].join(`,\n${indent}`)}\n};`);
        return out;
    }, []).join("\n");
    result.enums.pid = ["None = -1", ...pid.filter((id) => id !== undefined)].join(",\n");
}

// main process
try {
    const
        // scan definitions folder to collect contained yaml resources for automatical parsing
        definitionsPath = "./src/definitions",
        definitions = fs.readdirSync(definitionsPath).map((filename: string) => {
            return `${definitionsPath}/${filename}`;
        }).filter((filename: string) => {
            return fs.statSync(filename).isFile();
        }),
        // yaml parsing
        result: Result = {
            timestamp: new Date(), parameters: "", smoothers: "", signals: "", pid: "", parameter_list: "",
            ...ConfigParser.parse("./src/config.yaml"),
            ...DeclarationsParser.parse("./src/declarations.yaml"),
            definitions: DefinitionsParser.parseAll(...definitions).map((definition, index) => {
                return { ...definition, filename: definitions[index] }
            })
        };
    // processing of parsed yaml (sanity checks, enum sorting/filtering, providing strings for replacements)
    processDefinitions(result);
    // validate declaration tokens
    Object.keys(result.declarations).forEach((enumeration) => {
        if(!validateToken(enumeration)) {
            throw new Error(`declarations error: invalid enumeration token "${enumeration}"`);
        }
        Object.keys(result.declarations[enumeration]).forEach((token) => {
            if(!validateToken(token)) {
                throw new Error(`declarations error: invalid token "${token}" in enumeration ${enumeration}`);
            }
        });
    });
    // transformations of ./src/*.in.* files into usable resources in ./generated via string replacements
    generateOutputFile("./src/config.h.in", "./generated/config.h", result);
    generateOutputFile("./src/declarations.h.in", "./generated/declarations.h", result);
    generateOutputFile("./src/definitions.h.in", "./generated/definitions.h", result);
    generateOutputFile("./src/descriptor.h.in", "./generated/descriptor.h", result);
    generateOutputFile("./src/main.cpp.in", "./generated/main.cpp", result);
    // transformations not covered by g++ and therefore unsafe
    generateOutputFile("./src/placeholder.h.in", "./generated/placeholder.h", result);
    generateOutputFile("./src/ParameterFactory.java.in", "./generated/ParameterFactory.java", result);
    generateOutputFile("./src/MacroIds.js.in", "./generated/MacroIds.js", result);
    // if no error was raised, node is done
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
