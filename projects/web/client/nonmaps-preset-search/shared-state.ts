import { ReactiveVar } from "meteor/reactive-var";

export var cachedSearch = new ReactiveVar<Array<string>>([]);
export var scrollY = new ReactiveVar<number>(0);
export var multipleSelection = new ReactiveVar<Array<string> | null>(null);
export var multipleSelectionStartedByShiftClick = new ReactiveVar<boolean>(false);

export function getPresetHeight() {
    var e = document.getElementsByClassName("preset");
    if (e.length)
        return e[0].getBoundingClientRect().height || 30;
    return 30;
}