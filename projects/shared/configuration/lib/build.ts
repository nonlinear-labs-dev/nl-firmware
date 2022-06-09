import * as process from "process";
import { generateConfig } from "./tasks/config";
import { generateClassification } from "./tasks/classification";

try {
    generateConfig();
    generateClassification();
} catch(err) {
    console.error(err);
    process.exit(1);
}