import { Type, Parser } from "../yaml";

type ValueType = {
    coarse: number;
    fine: number;
    unit: string;
    initial: number;
    scale: string;
};

type ParameterType = {
    token: string;
    label_long: string;
    label_short: string;
    type: string;
    id: number;
    control_position: ValueType & {
        inactive?: number
    };
    return_behavior?: Array<string>;
    modulation_amount?: ValueType;
    bipolar?: boolean;
    info: string;
    rendering?: {
        scale: string;
        factor: number;
        offset: number;
        section: string;
        clock: string;
        signal: string;
    };
};

export type DefinitionsType = {
    group: string;
    parameters: Array<ParameterType>;
};

export const DefinitionsParser = new Parser<DefinitionsType>(
    new Type<DefinitionsType>("definitions", {
        kind: "mapping",
        keys: ["group", "parameters"],
        resolve(definitions: any): boolean {
            return this.keys.reduce((out, key) => out && (key in definitions), true);
        }
    })
);
