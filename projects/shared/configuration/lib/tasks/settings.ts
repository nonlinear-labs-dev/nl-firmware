import { Type, Parser } from "../yaml";
import { RenderScaling } from "./definitions";

export type SettingType = {
    default?: number | string;
    display?: {
        scale: string;
        coarse: number;
        fine: number;
    };
    renderScaling?: RenderScaling;
};

export type Settings = Record<string, SettingType>;

export const SettingParser = new Parser<Settings>(
    new Type<Settings>("settings", {
        kind: "mapping",
        resolve(_: Settings): boolean {
            return true;
        },
        construct(settings: Settings): Settings {
            return settings;
        }
    })
);
