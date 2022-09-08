import * as fs from "fs"
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
}

export class Parser<Result> {
    schema: yaml.Schema;
    constructor(...schema: Array<yaml.Type>) {
        this.schema = yaml.DEFAULT_SCHEMA.extend(schema);
    }
    parse(filename: string): Result {
        fsTestFile(filename);
        return yaml.load(fs.readFileSync(filename, "utf-8"), { filename, schema: this.schema }) as Result;
    }
    parseAll(...filenames: Array<string>): Array<Result> {
        return filenames.map((filename) => this.parse(filename));
    }
}

export function performReplacement(str: string, result: any, filename?: string) {
    return str.replace(/([ \t]*)\$\{([^\}]*)\}/g, (_, ws = "", keys) => {
        let known = result;
        keys.split(".").forEach((key) => {
            if(key in known) {
                known = known[key];
            } else {
                const msg = `unknown key "${key}" in ${keys}`;
                if(filename) {
                    throw new Error(`generateOutputFile error in ${filename}: ${msg}`);
                } else {
                    throw new Error(`generateOutputFile error: ${msg}`);
                }
                
            }
        });
        return ws + known.toString().split("\n").join(`\n${ws}`);
    })
}

function createDirectoryStructure(file: string) {
    var path = file.split('/');
    path.splice(path.length-1,1);
    let directory = path.join('/');
    fs.mkdir(directory, (err) => {
        console.error(err);
    });
}

export function generateOutputFile(infile: string, outfile: string, result: any) {
    fsTestFile(infile);
    createDirectoryStructure(outfile);
    fs.writeFileSync(outfile, performReplacement(fs.readFileSync(infile, "utf-8"), result, infile));
}
