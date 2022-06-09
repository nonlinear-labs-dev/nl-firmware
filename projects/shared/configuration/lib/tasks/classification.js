"use strict";
exports.__esModule = true;
exports.generateClassification = void 0;
var fs = require("fs");
var yaml = require("js-yaml");
function generateClassification() {
    var result = {
        timestamp: new Date(),
        classification: yaml.load(fs.readFileSync("./src/classification.yaml", "utf-8")),
        definitions: yaml.load(fs.readFileSync("./src/definitions.yaml", "utf-8"))
    };
    console.log(result);
}
exports.generateClassification = generateClassification;
