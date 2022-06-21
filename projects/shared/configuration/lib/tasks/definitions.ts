import { Type, Parser } from "../yaml";

type ValueType = {
    coarse: number;
    fine: number;
    unit: string;
    scale: string;
};

type ParameterType = {
    token: string;
    label_long: string;
    label_short: string;
    type: string;
    id: number;
    control_position: ValueType & {
        initial: string;
        inactive?: number;
        bipolar?: boolean;
    };
    info: string;
    availability: {
        [key: string]: {
            hidden: boolean;
            disabled: boolean;
            count: number;
        };
    };
    return_behavior?: Array<string>;
    modulation_amount?: ValueType;
    rendering_args?: {
        scaling: string;
        factor: string;
        offset: string;
        section: string;
        clock: string;
        signal: string;
    };
};

export type SignalType = {
    [key: string]: Array<string>;
};

export type DefinitionsType = {
    group: string;
    parameters: Array<ParameterType>;
    signals?: SignalType;
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
