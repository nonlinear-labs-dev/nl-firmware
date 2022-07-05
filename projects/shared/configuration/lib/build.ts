import * as process from "process";
import * as fs from "fs";
import { generateOutputFile } from "./yaml";
import { ConfigType, ConfigParser } from "./tasks/config";
import { DeclarationsType, DeclarationsParser } from "./tasks/declarations";
import { DefinitionsType, SignalType, ParameterType, DefinitionsParser } from "./tasks/definitions";

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
    parameter_units: string;
    display_scaling_types: string;
    parameter_groups: string;
};

type ParamType = {
    [key: string]: Array<string>;
};

const indent = " ".repeat(4);

function validateToken(token: string): boolean {
    return /^\w+$/.test(token);
}

function replaceResultInFiles(result: Result, ...files: Array<string>) {
    files.forEach((infile: string) => {
        const outfile = infile.replace("./src/", "./generated/").replace(/.in$/, "");
        generateOutputFile(infile, outfile, result);
    });
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
        parameterType = Object.entries(result.declarations.parameter_type).reduce((out: ParamType, [type, props]) => {
            if(type !== "None") {
                // every type !== None can collect tokens
                if(!props.includes("parameter_id_none")) {
                    out[`${type}s`] = [];
                } else {
                    out[`${type}s`] = ["None"];
                }
            }
            return out;
        }, {}),
        // smootherType collection (resulting in enum)
        smootherType = Object.keys(result.declarations.smoother_section).reduce((out: SignalType, section: string) => {
            if(section !== "None") {
                Object.keys(result.declarations.smoother_clock).forEach((clock: string) => {
                    out[`${section}_${clock}`] = [];
                });
            }
            return out;
        }, {}),
        // signalType collection (resulting in enum)
        signalType = Object.keys(result.declarations.parameter_signal).reduce((out: SignalType, type: string) => {
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
    // parameterList collection
    const
        processedGroups: Array<string> = [],
        parameterList: Array<string> = new Array<string>(result.config.params).fill("{None}");
    // for every yaml resource of ./src/definitions, providing a parameter group
    result.definitions.sort((...defs) => {
        return defs.reduce((out, {filename, group}, index) => {
            const
                err = `${errmsg} in ${filename}`,
                found = result.declarations.parameter_group[group];
            if(found === undefined) {
                throw new Error(`${err}: unknown group token "${group}"`);
            }
            if(found === null) {
                return Infinity;
            }
            return out + ([1, -1][index] * found.index);
        }, 0);
    }).forEach(({filename, ...definition}) => {
        const
            err = `${errmsg} in ${filename}`,
            group = {
                name: definition.group,
                data: result.declarations.parameter_group[definition.group]
            };
        // group sanity checks
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
            // global/local sanity checks
            const isGlobalParam = type.data.includes("global_parameter");
            if(group.data.global_group) {
                if(!isGlobalParam) {
                    throw new Error(`${err}: global group "${group.name}" cannot contain non-global parameter id ${id}`);
                }
            } else {
                if(isGlobalParam) {
                    throw new Error(`${err}: non-global group "${group.name}" cannot contain global parameter id ${id}`);
                }
            }
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
            const displayScaling = {
                cp: `Properties::DisplayScalingType::${scale}`,
                ma: "Properties::DisplayScalingType::None"
            };
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
                displayScaling.ma = `Properties::DisplayScalingType::${scale}`;
                playgroundDescriptor.push(coarse.toString(), fine.toString());
            } else {
                playgroundDescriptor.push("None", "None");
            }
            // feed playgroundDescriptor with parameter labels and info strings
            playgroundDescriptor.push(...[
                inactive || "", group.data.label_long, group.data.label_short, label_long, label_short,
                info.trim().replace(/\n/g, "\\n")
            ].map((entry) => `"${entry}"`), displayScaling.cp, displayScaling.ma);
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
                        signalType[signalStr].push(tokenStr);
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
                    bipolar.toString()
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
                    // append explicit signal
                    signalType[signalGroup].push(signalStr);
                });
            });
        }
    });
    // provide string replacements
    result.parameter_list = parameterList.join(",\n");
    result.parameter_units = Object.values(result.declarations.parameter_unit).map((entry => `"${entry}"`)).join("\n");
    result.display_scaling_types = Object.entries(result.declarations.display_scaling_type).reduce((out, [key, props]) => {
        out.push(`{\n${indent}${[
            `Properties::DisplayScalingType::${key}`,
            `Properties::ParameterRounding::${props.round}`,
            `Properties::ParameterUnit::${props.unit}`,
            `Properties::ParameterInfinity::${props.inf}`,
            props.bipolar.toString()
        ].join(`,\n${indent}`)}\n}`);
        return out;
    }, new Array<string>()).join(",\n");
    result.parameter_groups = Object.entries(result.declarations.parameter_group).reduce((out, [key, props]) => {
        if(props === null) {
            out.push(`{\n${indent}Descriptors::ParameterGroup::${key}\n}`)
        } else {
            out.push(`{\n${indent}${[
                `Descriptors::ParameterGroup::${key}`,
                `{${props.color.join(", ")}}`,
                `"${props.label_long}"`,
                `"${props.label_short}"`,
                (props.global_group || false).toString()
            ].join(`,\n${indent}`)}\n}`);
        }
        
        return out;
    }, new Array<string>()).join(",\n");
    result.parameters = Object.entries(parameterType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries, "_LENGTH_"].join(`,\n${indent}`)}\n};`);
        return out;
    }, []).join("\n");
    result.smoothers = Object.entries(smootherType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries.filter((entry => entry !== undefined)), "_LENGTH_"].join(`,\n${indent}`)}\n};`);
        return out;
    }, []).join("\n");
    result.signals = Object.entries(signalType).reduce((out: Array<string>, [key, entries]) => {
        out.push(`enum class ${key} {\n${indent}${[...entries, "_LENGTH_"].join(`,\n${indent}`)}\n};`);
        return out;
    }, []).join("\n");
    result.enums.pid = ["None = -1", ...pid.filter((id) => id !== undefined)].join(",\n");
}

function generateOverview(result: Result) {
    const
        { timestamp, config } = result,
        parameter_list = result.definitions.reduce((out, {group, parameters}) => {
            out.push(...parameters.map((p) => { return { ...p, group }; }));
            return out;
        }, new Array<ParameterType & { group: string }>()).sort((a, b) => a.id - b.id).reduce((out, {group, id, type, label_long}) => {
            const
                group_label = result.declarations.parameter_group[group].label_long,
                rgb = result.declarations.parameter_group[group].color.join(",");
            out[id] = [
                `<div class="parameter" title="${group_label} / ${label_long}" style="background-color: rgb(${rgb});">`,
                `${indent}<code class="parameter-type">${type}</code>`,
                `${indent}<div class="group-label">${group_label}</div>`,
                `${indent}<code class="parameter-id">${id}</code>`,
                `${indent}<div class="parameter-label">${label_long}</div>`,
                "</div>"
            ].join("\n");
            return out;
        }, new Array<string>(result.config.params).fill('<div title="unused"></div>')).join("\n"),
        parameterCountColumns = Object.keys(result.declarations.parameter_type).filter((pt) => pt !== "None").length,
        parameter_count = Object.keys(result.declarations.parameter_type).reduce((out, paramType) => {
            if(paramType !== "None") {
                out[1].push(`<div class="hdr">${paramType.replace("_", "<br>")}</div>`);
                const params = result.definitions.reduce((out, {parameters}) => {
                    out.push(...parameters.filter(({type}) => type === paramType));
                    return out;
                }, new Array<ParameterType>());
                Object.keys(result.declarations.sound_type).filter((st) => st !== "None").forEach((soundType, index) => {
                    const
                        countSimple = params.reduce((count, {availability}) => {
                            return count + (availability[soundType].count === 0 ? 0 : 1);
                        }, 0),
                        countDual = params.reduce((count, {availability}) => {
                            return count + availability[soundType].count;
                        }, 0);
                    if(countSimple === countDual) {
                        out[2 + index].push(`<div>${countSimple}</div>`);
                    } else {
                        out[2 + index].push(`<div>${countSimple} (${countDual})</div>`);
                    }
                });
            }
            return out;
        }, [
            [`<div class="parameter-count" style="grid-template-columns: repeat(${1 + parameterCountColumns}, max-content);">`],
            ['<div class="hdr">Sound<br>Type</div>'],
            ...Object.keys(result.declarations.sound_type).filter((st) => st !== "None").map((soundType) => {
                return [`<div class="hdr">${soundType}</div>`];
            }),
            ["</div>"]
        ]).map((row) => row.join("\n")).join("\n"),
        parameter_groups = Object.entries(result.declarations.parameter_group).reduce((out, [_, groupProps]) => {
            if(groupProps !== null) {
                out.push(`<li>${groupProps.label_long}</li>`);
            }
            return out;
        }, new Array<string>()).join("\n");
    generateOutputFile(
        "./src/overview.html.in", 
        "./generated/overview.html",
        { timestamp, config, parameter_list, parameter_count, parameter_groups }
    );
}

// main function
function main() {
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
            timestamp: new Date(), parameters: "", smoothers: "", signals: "", pid: "",
            parameter_list: "", parameter_units: "", display_scaling_types: "", parameter_groups: "",
            ...ConfigParser.parse("./src/c15_config.yaml"),
            ...DeclarationsParser.parse("./src/parameter_declarations.yaml"),
            definitions: DefinitionsParser.parseAll(...definitions).map((definition, index) => {
                return { ...definition, filename: definitions[index] }
            })
        };
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
    // order ParameterGroups
    Object.entries(result.declarations.parameter_group).forEach(([groupName, groupProps], index) => {
        if(groupProps !== null) {
            Object.assign(result.declarations.parameter_group[groupName], {index});
        }
    });
    // processing of parsed yaml (sanity checks, enum sorting/filtering, providing strings for replacements)
    processDefinitions(result);
    // transformations of ./src/*.in.* files into usable resources in ./generated via string replacements
    replaceResultInFiles(
        result,
        // transformations covered by g++ and therefore safe
        "./src/c15_config.h.in",
        "./src/parameter_declarations.h.in",
        "./src/parameter_list.h.in",
        "./src/parameter_descriptor.h.in",
        "./src/display_scaling_type.h.in",
        "./src/parameter_group.h.in",
        "./src/main.cpp.in",
        // transformations not covered by g++ and therefore unsafe
        "./src/placeholder.h.in",
        "./src/ParameterFactory.java.in",
        "./src/MacroIds.js.in",
        // validation
        "./src/validate.h.in",
        "./src/validate.cpp.in"
    );
    // overview
    generateOverview(result);
}

function createDirectorys(dir) {
    fs.mkdir(dir, (err) => {
        if(err) {
            console.error(err);
        } else {
            console.log(dir + " was created!");
        }
    });
}

// process
try {
    createDirectorys("./generated");
    main();
    // if no error was raised, node is done
    process.exit(0);
} catch(err) {
    console.error(err);
    process.exit(1);
}
