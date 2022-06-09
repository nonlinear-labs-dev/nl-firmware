"use strict";
exports.__esModule = true;
exports.generateConfig = void 0;
var fs = require("fs");
var yaml = require("js-yaml");
function generateConfig() {
    var result = {
        timestamp: new Date(),
        config: yaml.load(fs.readFileSync("./src/config.yaml", "utf-8"))
    };
    var config = fs.readFileSync("./src/config.in.h", "utf-8").replace(/\$\{([^\}]*)\}/g, function (_, keys) {
        // a rather crude lookup mechanism
        var known = result;
        keys.split(".").forEach(function (key) {
            if (key in known) {
                known = known[key];
            }
            else {
                throw new Error("build.ts: unknown key \"".concat(key, "\""));
            }
        });
        return known.toString();
    });
    fs.writeFileSync("./generated/config.h", config);
}
exports.generateConfig = generateConfig;
