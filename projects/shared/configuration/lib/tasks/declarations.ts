import { Type, Parser } from "../yaml";

export type DeclarationsType = {
    declarations: {
        parameter_type: {
            [key: string]: { combined_label: boolean, rendering_args: boolean }
        };
    };
    enums: {
        [key: string]: string;
    };
};

const keys = [
    "parameter_type", "parameter_signal", "smoother_section", "smoother_clock", "smoother_scale"
];

export const DeclarationsParser = new Parser<DeclarationsType>(
    new Type("declarations", {
        kind: "mapping",
        resolve(declarations: any): boolean {
            return keys.reduce((out, key) => out && (key in declarations), true);
        },
        construct(declarations: any): DeclarationsType {
            const enums = {
                parameter_type: Object.keys(declarations.parameter_type).join(",\n"),
                parameter_signal: Object.keys(declarations.parameter_signal).join(",\n"),
                smoother_section: Object.keys(declarations.smoother_section).join(",\n"),
                smoother_clock: Object.keys(declarations.smoother_clock).join(",\n"),
                smoother_scale: Object.keys(declarations.smoother_scale).join(",\n")
            }
            return { declarations, enums };
        }
    })
);
