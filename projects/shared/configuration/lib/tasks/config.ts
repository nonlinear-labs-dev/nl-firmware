import { Type, Parser } from "../yaml";

export type ConfigType = {
    config: {
        milestone: string,
        version: number,
        max_voices: number,
        params: number,
        clock_rates: {
            sd: [ number, number, number ];
            hd: [ number, number, number ];
        }
        fade_time_in_ms: string,
        physical_key_range: { from: number, to: number };
        virtual_key_range: { from: number, to: number };
        generic_keys: { pivot: number, center: number, reference: number };
    };
};

export const ConfigParser = new Parser<ConfigType>(
    new Type<ConfigType>("config", {
        kind: "mapping",
        construct(config: any): ConfigType { return { config }; }
    })
);
