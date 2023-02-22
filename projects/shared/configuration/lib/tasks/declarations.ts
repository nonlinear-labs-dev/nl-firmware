import { Type, Parser } from "../yaml";

export type DeclarationsType = {
    declarations: {
        sound_type: {
            [key: string]: null;
        };
        layer_id: {
            [key: string]: null;
        };
        return_behavior: {
            [key: string]: null;
        };
        parameter_properties: Array<string>;
        parameter_type: {
            [key: string]: Array<string>;
        }
        parameter_signal: {
            [key: string]: null;
        };
        parameter_unit: {
            [key: string]: string;
        };
        parameter_rounding: {
            [key: string]: string;
        };
        parameter_reference: {
            [key: string]: string;
        };
        parameter_infinity: {
            [key: string]: null;
        }
        parameter_group: {
            [key: string]: {
                label_long: string;
                label_short: string;
                color: Array<number>;
                identifier: string;
                global_group: boolean;
                index: number;
            }
        };
        smoother_section: {
            [key: string]: null;
        };
        smoother_clock: {
            [key: string]: null;
        };
        smoother_scale: {
            [key: string]: null;
        };
        display_scaling_type: {
            [key: string]: {
                round: string;
                unit: string;
                reference: string;
                values: Array<string>;
                inf: boolean | string;
                min: string;
                max: string;
                bipolar: boolean;
            };
        };
    };
    enums: {
        [key: string]: string;
    };
};

export const DeclarationsParser = new Parser<DeclarationsType>(
    new Type("declarations", {
        kind: "mapping",
        keys: [
            "sound_type", "layer_id", "return_behavior",
            "parameter_properties", "parameter_type", "parameter_signal", "parameter_unit",
            "parameter_rounding", "parameter_reference", "parameter_infinity", "parameter_group",
            "smoother_section", "smoother_clock", "smoother_scale",
            "display_scaling_type"
        ],
        resolve(declarations: any): boolean {
            return this.keys.reduce((out, key) => out && (key in declarations), true);
        },
        construct(declarations: any): DeclarationsType {
            const enums = {
                sound_type: Object.keys(declarations.sound_type).join(",\n"),
                layer_id: [...Object.keys(declarations.layer_id), "_LENGTH_"].join(",\n"),
                return_behavior: Object.keys(declarations.return_behavior).join(",\n"),
                parameter_type: Object.keys(declarations.parameter_type).join(",\n"),
                parameter_signal: Object.keys(declarations.parameter_signal).join(",\n"),
                parameter_unit: Object.keys(declarations.parameter_unit).join(",\n"),
                parameter_rounding: Object.keys(declarations.parameter_rounding).join(",\n"),
                parameter_infinity: Object.keys(declarations.parameter_infinity).join(",\n"),
                parameter_group: Object.keys(declarations.parameter_group).join(",\n"),
                smoother_section: Object.keys(declarations.smoother_section).join(",\n"),
                smoother_clock: Object.keys(declarations.smoother_clock).join(",\n"),
                smoother_scale: Object.keys(declarations.smoother_scale).join(",\n"),
                display_scaling_type: Object.keys(declarations.display_scaling_type).join(",\n")
            };
            return { declarations, enums };
        }
    })
);
