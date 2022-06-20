import { Type, Parser } from "../yaml";

export type DefinitionsType = {
    definitions: {};
};

export const DefinitionsParser = new Parser<DefinitionsType>(
    new Type<DefinitionsType>("definitions", {
        kind: "mapping",
        construct(definitions: any): DefinitionsType { return { definitions }; }
    })
);
