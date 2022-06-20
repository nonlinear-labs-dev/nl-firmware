import * as fs from "fs";
import * as yaml from "js-yaml";

const namespace = "tag:nonlinear-labs.de,2022:js-yaml";

function fsTestFile(filename: string) {
    if(!fs.existsSync(filename)) throw new Error(`fs error: file ${filename} doesn't exist`);
    if(!fs.statSync(filename).isFile()) throw new Error(`fs error: ${filename} is not a file`);
}

type TypeOptions<Result> = {
    kind: "scalar" | "sequence" | "mapping";
    resolve?: (data: any) => boolean;
    construct?: (data: any) => Result;
    [key: string]: any;
};

export class Type<Result> extends yaml.Type {
    constructor(name: string, options: TypeOptions<Result>) {
        super(`${namespace}:${name}`, {
            kind: options.kind,
            resolve(data: any) {
                if([null, undefined].includes(data)) return false;
                if(options.resolve) return options.resolve(data);
                return true;
            },
            construct(data: any) {
                if(options.construct) return options.construct(data);
                return data;
            }
        });
    }
};

export class Parser<Result> {
    schema: yaml.Schema;
    constructor(...schema: Array<yaml.Type>) {
        this.schema = yaml.DEFAULT_SCHEMA.extend(schema);
    }
    parse(filename: string): Result {
        fsTestFile(filename);
        const parsed = yaml.load(fs.readFileSync(filename, "utf-8"), { filename, schema: this.schema }) as Result;
        return parsed;
    }
    parseAll(...filenames: Array<string>): Array<Result> {
        return filenames.map((filename) => this.parse(filename));
    }
};

export function generateOutputFile(infile: string, outfile: string, result: any) {
    fsTestFile(infile);
    fs.writeFileSync(outfile, fs.readFileSync(infile, "utf-8").replace(/([ \t]*)\$\{([^\}]*)\}/g, (_, ws = "", keys) => {
        let known = result;
        keys.split(".").forEach((key) => {
            if(key in known) {
                known = known[key];
            } else {
                throw new Error(`generateOutputFile error: ${infile}: unknown key "${key}" in ${keys}`);
            }
        });
        return ws + known.toString().split("\n").join(`\n${ws}`);
    }));
};
