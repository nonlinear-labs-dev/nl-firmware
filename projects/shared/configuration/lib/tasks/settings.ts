import { Type, Parser } from "../yaml";

export type SettingType = {
    default?: any;
    scale?: string;
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
