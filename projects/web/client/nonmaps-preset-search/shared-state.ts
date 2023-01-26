import { ReactiveVar } from "meteor/reactive-var";

export var cachedSearch = new ReactiveVar<Array<string>>([]);
export var scrollY = new ReactiveVar<number>(0);
export var multipleSelection = new ReactiveVar<Array<string> | null>(null);
export var multipleSelectionStartedByShiftClick = new ReactiveVar<boolean>(false);
export const numPresetsBeforeFirstVisible = 20;
export var currentContextMenuItem = new ReactiveVar<String[]>([]);

var presetHeight = new ReactiveVar<number | null>(null);

export function getPresetHeight(): number {
    if (presetHeight.get() != null)
        return presetHeight.get()!;

    return 30;
}

export function setPresetHeight(): boolean {
    if (presetHeight.get() == null) {
        var e = document.getElementsByClassName("preset-container");
        if (e.length) {
            const h = e[0].getBoundingClientRect().height;
            if (h != 0) {
                presetHeight.set(h);
                return true;
            }
        }
    }
    return false;
}

function listenOnDevicePixelRatio() {
    function onChange() {
        console.log("devicePixelRatio changed: " + window.devicePixelRatio);
        presetHeight.set(null);
        setPresetHeight();
        listenOnDevicePixelRatio();
    }

    matchMedia(`(resolution: ${window.devicePixelRatio}dppx)`).addEventListener("change", onChange, { once: true });
}
listenOnDevicePixelRatio();