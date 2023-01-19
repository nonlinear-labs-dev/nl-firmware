import { ReactiveVar } from "meteor/reactive-var";
import { Template } from "meteor/templating";
import { Tracker } from "meteor/tracker";
import { globals, glue } from "../glue";
import "./search-result-preset";
import "./search.html";
import { cachedSearch, currentContextMenuItem, getPresetHeight, multipleSelection, multipleSelectionStartedByShiftClick, numPresetsBeforeFirstVisible, scrollY } from "./shared-state";

type StringArray = Array<String>;
enum SearchOperator { And, Or }
enum SortBy { Number, Name, Time }
enum SortDirection { Asc, Desc }

class SearchOptions {
    operator: SearchOperator = SearchOperator.And;
    searchInName: boolean = true;
    searchInComment: boolean = true;
    searchInDeviceName: boolean = false;

    sorting = [
        { by: SortBy.Number, direction: SortDirection.Asc },
        { by: SortBy.Name, direction: SortDirection.Asc },
        { by: SortBy.Time, direction: SortDirection.Asc }];
}

const syncedDatabase = globals.syncedDatabase;
const playgroundProxy = globals.playgroundProxy;

var searchBanks = new ReactiveVar<StringArray>(new Array<String>());
var searchQuery = new ReactiveVar<String>("");
var searchColors = new ReactiveVar<StringArray>(new Array<String>());
var searchOptions = new ReactiveVar<SearchOptions>(new SearchOptions());
var searchCollapsed = new ReactiveVar<boolean>(true);
var showBankSelectPane = new ReactiveVar<boolean>(false);

window['togglePresetSearch'] = function () {
    searchCollapsed.set(!searchCollapsed.get());
    syncCollapsedState(searchCollapsed.get()!);
}

window['isPresetSearchVisible'] = function () {
    return !searchCollapsed.get();
}

function syncSelectedBanks(): void {
    const dbBanks = syncedDatabase.queryItem("/preset-manager")!['banks'];
    const filteredBanks = searchBanks.get().filter((v) => dbBanks.includes(v));
    if (filteredBanks.length != searchBanks.get().length)
        searchBanks.set(filteredBanks);
}

function generateBankButtonTitle(): String {
    const dbBanks = syncedDatabase.queryItem("/preset-manager")!['banks'];
    if (searchBanks.get().length == 0 || searchBanks.get().length == dbBanks.length)
        return "All Banks";

    if (searchBanks.get().length == 1) {
        const id = searchBanks.get()[0];
        const b = syncedDatabase.queryItem("/bank/" + id)!;
        const n = b['order-number'].toString();
        const nPadded = n.padStart(2, "0");
        return nPadded + " - " + b['name'];
    }

    return "Multiple Banks";
}

function doesPresetMatch(preset: any, searchOptions: SearchOptions, subquery: String) {
    if (searchOptions.searchInName && preset['name'].toLowerCase().includes(subquery))
        return true;

    if (searchOptions.searchInComment && preset['attributes']['Comment'] && preset['attributes']['Comment'].toLowerCase().includes(subquery))
        return true;

    if (searchOptions.searchInDeviceName && preset['attributes']['DeviceName'] && preset['attributes']['DeviceName'].toLowerCase().includes(subquery))
        return true;

    return false;
}

function getPresetSortKey(lhs: any, rhs: any, by: SortBy): number {
    switch (by) {
        case SortBy.Number:
            const lBank = syncedDatabase.queryItem("/bank/" + lhs['bank-uuid'])!;
            const rBank = syncedDatabase.queryItem("/bank/" + rhs['bank-uuid'])!;
            const lBankNr = lBank['order-number'];
            const rBankNr = rBank['order-number'];
            if (lBankNr != rBankNr)
                return lBankNr - rBankNr;

            const lPresetNr = lBank['presets'].indexOf(lhs['uuid']);
            const rPresetNr = rBank['presets'].indexOf(rhs['uuid']);

            return lPresetNr - rPresetNr;

        case SortBy.Name:
            return lhs['name'].localeCompare(rhs['name']);

        case SortBy.Time:
            if (!lhs['attributes']['StoreTime'] || !rhs['attributes']['StoreTime'])
                return 0;

            return lhs['attributes']['StoreTime'].localeCompare(rhs['attributes']['StoreTime']);
    }
}

function getPresetSortResultFor(lhs: any, rhs: any, by: SortBy, dir: SortDirection): number {
    return (dir == SortDirection.Asc ? 1 : -1) * getPresetSortKey(lhs, rhs, by);
}

function getPresetSortResult(lhs: any, rhs: any): number {
    for (const s of searchOptions.get().sorting) {
        const sortKey = getPresetSortResultFor(lhs, rhs, s.by, s.direction);
        if (sortKey != 0)
            return sortKey;
    }
    return 0;
}

function performSearch() {
    const query = searchQuery.get().trim().toLowerCase().split(" ");
    const banks: Array<String> = searchBanks.get().length != 0 ? searchBanks.get() : syncedDatabase.queryItem("/preset-manager")?.['banks'];
    const colors = searchColors.get();
    const opt = searchOptions.get();

    var ret = banks?.map(bankId => syncedDatabase.queryItem("/bank/" + bankId))?.
        map(bank => bank?.['presets']).flat()?.
        map(presetId => syncedDatabase.queryItem("/preset/" + presetId))?.
        filter(preset => (preset && preset['name']) ? true : false)?.
        filter(preset => colors.length == 0 || (preset!['attributes']['color'] && colors.includes(preset!['attributes']['color'])))?.
        filter(preset => opt.operator == SearchOperator.Or || query.every(v => doesPresetMatch(preset, opt, v)))?.
        filter(preset => opt.operator == SearchOperator.And || query.some(v => doesPresetMatch(preset, opt, v)))?.
        sort((lhs, rhs) => getPresetSortResult(lhs, rhs))?.
        map(preset => preset!['uuid']);

    if (!ret)
        ret = [];

    return ret;
}

function loadCurrentPreset() {
    var pm = syncedDatabase.queryItem("/preset-manager")!;
    var selBank = pm['selected-bank'];
    var bank = syncedDatabase.queryItem("/bank/" + selBank)!;
    var selPreset = bank['selected-preset'];
    playgroundProxy.loadPreset(selPreset);
}

function selectPreset(what: string) {
    var pm = syncedDatabase.queryItem("/preset-manager")!;
    var selBank = pm['selected-bank'];
    var bank = syncedDatabase.queryItem("/bank/" + selBank)!;
    var selPreset = bank['selected-preset'];
    var searchResults = cachedSearch.get();
    var idx = searchResults.indexOf(selPreset);

    idx = Math.max(0, Math.min(searchResults.length - 1, idx));

    if (searchResults.length > 0) {

        var presetHeight = getPresetHeight();
        var scroller = $("#preset-search-results-scroller")!;
        var paneHeight = scroller.height()!;
        var scrollRangeMin = scroller.scrollTop()!;
        var scrollRangeMax = scrollRangeMin + paneHeight - presetHeight;

        switch (what) {
            case "ArrowUp":
                idx--;
                break;

            case "ArrowDown":
                idx++;
                break;

            case "PageUp":
                idx -= Math.floor(paneHeight / presetHeight);
                break;

            case "PageDown":
                idx += Math.floor(paneHeight / presetHeight);
                break;
        }

        idx = Math.max(0, Math.min(searchResults.length - 1, idx));

        playgroundProxy.selectPreset(searchResults[idx]);
        var presetScrollPosition = idx * presetHeight;

        if (presetScrollPosition < scrollRangeMin)
            scroller.scrollTop(presetScrollPosition);

        if (presetScrollPosition > scrollRangeMax)
            scroller.scrollTop(presetScrollPosition - paneHeight + presetHeight);
    }
}

function changeSortOrder(by: SortBy) {
    var o = searchOptions.get();
    if (o.sorting[0].by == by) {
        // invert currently selected criteria
        o.sorting[0].direction = o.sorting[0].direction == SortDirection.Asc ? SortDirection.Desc : SortDirection.Asc;
    }
    else {
        // or focus the selection
        const idx = o.sorting.findIndex(i => i.by == by);
        const dir = o.sorting[idx].direction;
        o.sorting.splice(idx, 1);
        o.sorting.splice(0, 0, { by: by, direction: dir });
    }
    searchOptions.set(o);
}

function syncCollapsedState(collapsed: boolean) {
    if (collapsed) {
        $(".collapser").fadeOut();
    }
    else {
        var beltPos = Number.parseFloat($("#preset-search").attr("belt-position") || "1000");
        $(".collapser").fadeIn();
        $("#preset-search").css("max-height", beltPos);
        $('#html-search').css("z-index", window['nextZIndex']());
    }

    if (window['redrawNonWorld'])
        window['redrawNonWorld']();
}

function getSearchResultScrollOffset() {
    const h = getPresetHeight();
    return scrollY.get() - scrollY.get() % h - h * numPresetsBeforeFirstVisible;
}

function isEventOutsideVisibleBankSelectPane(event): boolean {
    var panes = $(".select-pane");
    if (panes.length > 0) {
        if ($(panes[0]).is(":visible")) {
            var banksRect = panes[0].getBoundingClientRect();
            var matchX = banksRect.left < event.pageX && banksRect.right > event.pageX;
            var matchY = banksRect.top < event.pageY && banksRect.bottom > event.pageY;
            return !matchX || !matchY;
        }
    }
    return false;
}

document.addEventListener('click', (event) => {
    const classes = (event.target as Element).classList;
    if (classes && (classes.contains("no-close-bank-menu")))
        return;

    if (isEventOutsideVisibleBankSelectPane(event))
        showBankSelectPane.set(false);
}, true);


document.addEventListener('wheel', (event) => {
    if (isEventOutsideVisibleBankSelectPane(event))
        event.stopImmediatePropagation();
}, true);

function findPresetTarget(event: JQuery.Event): string {
    var presetHeight = getPresetHeight();
    const scroller = document.getElementById("preset-search-results-scroller")!;
    const top = scroller.getBoundingClientRect().top;
    var offsetY = scrollY.get() + event.clientY! - top;
    var idx = offsetY / presetHeight;
    const searchResult = cachedSearch.get();
    return searchResult[(Math.floor(idx))]!;
}

function sendEventToPresetWithId(id: string, name: string, event: JQuery.Event | null) {
    const preset = document.getElementById(id);
    preset?.dispatchEvent(new CustomEvent(name, { detail: { originalEvent: event } }));
}

function sendEventToPreset(name: string, event: JQuery.Event): string {
    const presetId = findPresetTarget(event);
    sendEventToPresetWithId(presetId, name, event);
    return presetId;
}

glue("nonmapsPresetSearch",
    (id, innerState) => {
        const searchResult = cachedSearch.get();
        const scroller = document.getElementById("preset-search-results-scroller");

        if (scroller) {
            var presetHeight = getPresetHeight();
            var paneHeight = presetHeight * (searchResult.length + numPresetsBeforeFirstVisible);
            var maxScroll = paneHeight - scroller!.clientHeight;
            scrollY.set(Math.max(0, Math.min(maxScroll, scrollY.get())));
        }

        return {
            collapsed: searchCollapsed.get(),
            numResults: searchResult.length,
            hasSearchQuery: searchQuery.get().length > 0
        }
    }, () => {
        syncCollapsedState(true);
        return {};
    }, {
    "sortByNumberAttributes"() {
        return {
            class: ["sort-by-number ", searchOptions.get().sorting[0].by == SortBy.Number ? "primary" : "secondary"]
        }
    },
    "sortByNameAttributes"() {
        return {
            class: ["sort-by-name ", searchOptions.get().sorting[0].by == SortBy.Name ? "primary" : "secondary"]
        }
    },
    "sortByTimeAttributes"() {
        return {
            class: ["sort-by-time ", searchOptions.get().sorting[0].by == SortBy.Time ? "primary" : "secondary"]
        }
    },
    "numberSortDirectionAttributes"() {
        return {
            class: ["sort ", searchOptions.get().sorting.find(z => z.by == SortBy.Number)!.direction == SortDirection.Asc ? "asc" : "desc"]
        }
    },
    "nameSortDirectionAttributes"() {
        return {
            class: ["sort ", searchOptions.get().sorting.find(z => z.by == SortBy.Name)!.direction == SortDirection.Asc ? "asc" : "desc"]
        }
    },
    "timeSortDirectionAttributes"() {
        return {
            class: ["sort ", searchOptions.get().sorting.find(z => z.by == SortBy.Time)!.direction == SortDirection.Asc ? "asc" : "desc"]
        }
    }
},
    {
        "click .header .symbol"() {
            searchCollapsed.set(!searchCollapsed.get());
            syncCollapsedState(searchCollapsed.get()!);
        },
        "click .header .title"() {
            $('#html-search').css("z-index", window['nextZIndex']());
        },
        "keyup .search-string"(event) {
            searchQuery.set((event as JQuery.KeyUpEvent).target.value);
        },
        "click .search-string+.reset"() {
            searchQuery.set("");
            $(".search-string-container .search-string").val("");
        },
        "click .sort-by-name"() {
            changeSortOrder(SortBy.Name);
        },
        "click .sort-by-number"() {
            changeSortOrder(SortBy.Number);
        },
        "click .sort-by-time"() {
            changeSortOrder(SortBy.Time);
        },
        'click #preset-search'() {
            $("#preset-search").get(0)?.focus();
        },
        'click .search-string'(event) {
            $("input.search-string").get(0)!.focus();
            event.stopImmediatePropagation();
        },
        'keydown #preset-search'(event) {
            var keyboardEvent = event as JQuery.KeyDownEvent;
            if (Array.of("ArrowUp", "ArrowDown", "PageUp", "PageDown").includes(keyboardEvent.key)) {
                selectPreset(keyboardEvent.key);
                event.preventDefault();
                event.stopImmediatePropagation();
            }
            else if (keyboardEvent.key == "Enter") {
                loadCurrentPreset();
                event.preventDefault();
                event.stopImmediatePropagation();
            }
            else {
                $("input.search-string").get(0)!.focus();
            }
        }
    });


Template['nonmapsPresetSearch'].onRendered(() => {
    Tracker.autorun(() => cachedSearch.set(performSearch()));
});

glue("presetSearchColors",
    (id, innerState) => { return { showSettings: innerState?.showSettings.get() } },
    () => { return { showSettings: new ReactiveVar<boolean>(false) } },
    {
        "searchSettingsAttributes"(presenter, innerState) {
            return {
                id: "search-settings",
                class: ["settings ", innerState!.showSettings.get() ? "active" : "inactive"]
            }
        }
    }
    , {
        "click .settings"(event, presenter, innerState) {
            var v = !innerState!.showSettings.get();
            innerState!.showSettings.set(v);

            if (v)
                $(".settings-pane").slideDown({
                    start: function () {
                        $(this).css({
                            display: "flex"
                        })
                    }
                });
            else
                $(".settings-pane").slideUp();
        }
    });

glue("searchSettings",
    (id) => {
        return {
            operatorAnd: searchOptions.get().operator == SearchOperator.And,
            operatorOr: searchOptions.get().operator == SearchOperator.Or,
            searchInName: searchOptions.get().searchInName,
            searchInComment: searchOptions.get().searchInComment,
            searchInDeviceName: searchOptions.get().searchInDeviceName,
        }
    }, null, null, {
    "change #op-and"() {
        var op = searchOptions.get();
        op.operator = SearchOperator.And;
        searchOptions.set(op);
    },
    "change #op-or"() {
        var op = searchOptions.get();
        op.operator = SearchOperator.Or;
        searchOptions.set(op);
    },
    "change #search-in-name"(event) {
        var op = searchOptions.get();
        op.searchInName = (event as JQuery.ClickEvent).target.checked;
        searchOptions.set(op);
    },
    "change #search-in-comment"(event) {
        var op = searchOptions.get();
        op.searchInComment = (event as JQuery.ClickEvent).target.checked;
        searchOptions.set(op);
    },
    "change #search-in-devicename"(event) {
        var op = searchOptions.get();
        op.searchInDeviceName = (event as JQuery.ClickEvent).target.checked;
        searchOptions.set(op);
    }
});

glue("presetSearchColor",
    (id) => { return { id: id } },
    null, {
    "attributes"(presenter) {
        return { class: [presenter.id, searchColors.get().includes(presenter.id) ? " selected" : " unselected"] }
    }
}, {
    "click"(event, presenter) {
        searchColors.set(searchColors.get().includes(presenter.id) ? searchColors.get().filter(v => v != presenter.id) : searchColors.get().concat([presenter.id]));
    }
});


glue("bankSelect", (id: string, innerState) => {
    syncSelectedBanks();
    return {
        buttonName: generateBankButtonTitle(),
        banks: syncedDatabase.queryItem("/preset-manager")!['banks']
    }
}, null,
    {
        "paneAttributes"() {
            var w = $(".banks").width();
            return {
                class: "select-pane",
                style: "width: " + w + "px; display: " + (showBankSelectPane.get() ? "block" : "none"),
                tabindex: "3"
            }
        },
        "previousBankAttribute"(presenter) {
            var a = { id: "select-previous-bank", style: "display: none" };
            if (searchBanks.get().length == 1) {
                const b = searchBanks.get()[0];
                if (presenter.banks.indexOf(b) > 0) {
                    a['style'] = "display: flex";
                    a['class'] = "no-close-bank-menu";
                }
                else {
                    a['style'] = "display: flex";
                    a['class'] = "disabled no-close-bank-menu";
                }
            }
            return a;
        },
        "nextBankAttribute"(presenter) {
            var a = { id: "select-next-bank", style: "display: none" };
            if (searchBanks.get().length == 1) {
                const b = searchBanks.get()[0];
                if (presenter.banks.indexOf(b) < presenter.banks.length - 1) {
                    a['style'] = "display: flex";
                    a['class'] = "no-close-bank-menu";
                }
                else {
                    a['style'] = "display: flex";
                    a['class'] = "disabled no-close-bank-menu";
                }
            }
            return a;
        },
        "nextPreviousBankAttribute"(presenter) {
            var a = { class: "no-close-bank-menu next-prev-bank-buttons", style: "display: none" };
            if (searchBanks.get().length == 1) {
                const b = searchBanks.get()[0];
                if (presenter.banks.indexOf(b) < presenter.banks.length - 1) {
                    a['style'] = "display: flex";
                }
            }
            return a;
        }
    }
    , {
        "click #select-previous-bank"(event, presenter) {
            if (searchBanks.get().length == 1) {
                const b = searchBanks.get()[0];
                const idx = presenter.banks.indexOf(b);
                if (idx > 0) {
                    searchBanks.set([presenter.banks[idx - 1]]);
                }
            }
            event.stopImmediatePropagation();
            event.preventDefault();
        },
        "click #select-next-bank"(event, presenter) {
            if (searchBanks.get().length == 1) {
                const b = searchBanks.get()[0];
                const idx = presenter.banks.indexOf(b);
                if (idx < presenter.banks.length - 1) {
                    searchBanks.set([presenter.banks[idx + 1]]);
                }
            }
            event.stopImmediatePropagation();
            event.preventDefault();
        },
        "click .toggle"() {
            showBankSelectPane.set(!showBankSelectPane.get());

            if (showBankSelectPane.get()) {
                $(".select-pane")[0].focus();
            }
        },
        "click .select-all"() {
            searchBanks.set([...syncedDatabase.queryItem("/preset-manager")!['banks']]);
        },
        "click .select-none"() {
            searchBanks.set(new Array<String>());
        },
        'click'(event) {
            $(".select-pane").get(0)?.focus();
            event.stopPropagation();
        },
    });


glue("bankOption", (id: string) => {
    const b = syncedDatabase.queryItem("/bank/" + id)!;
    return {
        id: id,
        number: b['order-number'].toString().padStart(2, "0"),
        name: b['name']
    }
}, null, {
    "checkAttributes"(presenter) {
        return { 'class': searchBanks.get().includes(presenter.id) ? "check" : "check hidden" }
    }
}, {
    "click"(event, presenter) {
        var old = searchBanks.get();
        searchBanks.set(old.includes(presenter.id) ? old.filter(v => v != presenter.id) : old.concat([presenter.id]));
    }
});

enum PointerOperation {
    Scroll,
    DragPreset,
    SelectOrLoad,
    ContextMenu
}

class Position {
    public x: number;
    public y: number;
}

function getHysteresis() {
    return document.getElementById("hysteresis")?.clientWidth ?? 10;
}

class SearchResultsInnerState {
    constructor(public touchStartTime: null | number = null) {
    }

    public startContextMenuTimeout(x: number, y: number) {
        this.cancelContextMenuTimeout();
        this.touchStartTime = Date.now();
        this.touchStartPosition = { x: x, y: y };
    }

    public cancelContextMenuTimeout() {
        this.touchStartTime = null;
    }

    public openContextMenu() {
        if (this.touchStartTime && Date.now() - this.touchStartTime > 500) {
            if (this.possibleOperations.has(PointerOperation.ContextMenu)) {
                const list = document.getElementById("preset-search-results-scroller-pane")!;
                list.dispatchEvent(new ContextMenuEvent(this.touchStartPosition?.x!, this.touchStartPosition?.y!));
                this.onContextMenu();
            }
        }
        this.touchStartTime = null;
    }

    public onPointerDown(event: JQuery.Event): boolean {
        if (this.pointerDown)
            return false;

        this.possibleOperations.add(PointerOperation.Scroll);
        this.possibleOperations.add(PointerOperation.DragPreset);
        this.possibleOperations.add(PointerOperation.SelectOrLoad);
        this.possibleOperations.add(PointerOperation.ContextMenu);
        this.pointerDown = { x: event.clientX!, y: event.clientY! };
        this.xHysteresisDone = false;
        this.currentPresetTarget.set(null);
        return true;
    }

    public onPointerMove(event: JQuery.Event) {
        const hysteresis = getHysteresis();

        if (this.pointerDown) {
            if (Math.abs(event.clientY! - this.pointerDown.y) >= hysteresis) {
                this.possibleOperations.delete(PointerOperation.DragPreset);
                this.possibleOperations.delete(PointerOperation.SelectOrLoad);
                this.possibleOperations.delete(PointerOperation.ContextMenu);
            }
            else if (Math.abs(event.clientX! - this.pointerDown.x) >= hysteresis) {
                this.xHysteresisDone = true;
                this.possibleOperations.delete(PointerOperation.Scroll);
                this.possibleOperations.delete(PointerOperation.SelectOrLoad);
                this.possibleOperations.delete(PointerOperation.ContextMenu);
            }

            if (this.xHysteresisDone && this.currentPresetTarget.get() == null) {
                this.currentPresetTarget.set(sendEventToPreset("startDrag", event));
            }
            else if (this.currentPresetTarget.get()) {
                sendEventToPresetWithId(this.currentPresetTarget.get()!, "performDrag", event);
            }
        }
    }

    public onPointerUp(event: JQuery.Event) {
        this.pointerDown = null;

        if (this.currentPresetTarget.get()) {
            sendEventToPresetWithId(this.currentPresetTarget.get()!, "stopDrag", event);
        }

        this.currentPresetTarget.set(null);

        if (this.possibleOperations.has(PointerOperation.SelectOrLoad)) {
            sendEventToPreset("selectOrLoad", event);
        }
    }

    public onLostCapture() {
        this.pointerDown = null;
    }

    public cancelDragging() {
        if (this.currentPresetTarget.get()) {
            sendEventToPresetWithId(this.currentPresetTarget.get()!, "cancelDrag", null);
        }

        this.currentPresetTarget.set(null);
    }

    public onContextMenu() {
        this.possibleOperations.delete(PointerOperation.DragPreset);
        this.possibleOperations.delete(PointerOperation.SelectOrLoad);
        this.possibleOperations.delete(PointerOperation.Scroll);
    }

    public isContextMenuAllowed(): boolean {
        return this.possibleOperations.has(PointerOperation.ContextMenu);
    }

    public isDragging() {
        return this.currentPresetTarget.get() != null;
    }

    private possibleOperations = new Set<PointerOperation>();
    private pointerDown: Position | null = null;
    private touchStartPosition: Position | null = null;
    private xHysteresisDone = false;
    private currentPresetTarget = new ReactiveVar<string | null>(null);
}


class ContextMenuEvent extends MouseEvent {
    constructor(x: number, y: number) {
        super("contextmenu", {
            clientX: x,
            clientY: y,
            bubbles: true
        });
    }
}

glue("searchResults",
    () => {
        const maxPresetsVisibleOnOnePage = 100;
        return {
            shownPresets: Array.from(Array(maxPresetsVisibleOnOnePage).keys())
        }
    },
    () => new SearchResultsInnerState(),
    {
        "scrollerAttributes"(presenter) {
            if (showBankSelectPane.get())
                return {
                    id: "preset-search-results-scroller",
                    style: "overflow: hidden;"
                }

            return {
                id: "preset-search-results-scroller",
                style: "overflow: auto;"
            }
        },
        "scrollerPaneAttributes"(presenter) {
            var presetHeight = getPresetHeight();
            var paneHeight = presetHeight * (cachedSearch.get().length);

            return {
                id: "preset-search-results-scroller-pane",
                style: "height: " + paneHeight + "px"
            }
        },
        "resultsAttributes"(presenter, innerState) {
            const ph = getPresetHeight();
            const scroll = scrollY.get();
            const offset = scroll / ph;
            const firstPresetIdx = Math.floor(offset);
            const numPresets = cachedSearch.get().length + numPresetsBeforeFirstVisible;
            const h = (numPresets - firstPresetIdx) * ph;

            return {
                id: "preset-search-results",
                style: "top:" + getSearchResultScrollOffset() + "px; height: " + h + "px;",
                class: innerState!.isDragging() ? "" : "no-pointer-events"
            }
        }
    },
    {
        "touchstart #preset-search-results-scroller-pane"(event, _presenter, innerState) {
            const o = event['originalEvent'];
            if (o.touches.length > 1) {
                event.preventDefault();
                event.stopImmediatePropagation();
                return;
            }
            innerState!.startContextMenuTimeout(o.touches[0].clientX, o.touches[0].clientY);
        },
        "touchcancel"(event, _presenter, innerState) {
            const o = event['originalEvent'];
            if (o.touches.length == 0)
                innerState!.cancelContextMenuTimeout();
        },
        "touchend"(event, _presenter, innerState) {
            const o = event['originalEvent'];
            if (o.touches.length == 0)
                innerState!.openContextMenu();
        },
        "pointerdown #preset-search-results-scroller-pane"(event, presenter, innerState) {
            const list = document.getElementById("preset-search-results-scroller-pane")!;
            if (innerState!.onPointerDown(event))
                list.setPointerCapture(event['originalEvent'].pointerId);

        },
        "pointermove"(event, presenter, innerState) {
            const o = event['originalEvent'];
            const list = document.getElementById("preset-search-results-scroller-pane")!;
            if (list.hasPointerCapture(o.pointerId))
                innerState!.onPointerMove(event);
        },
        "pointerup"(event, presenter, innerState) {
            const o = event['originalEvent'];
            const list = document.getElementById("preset-search-results-scroller-pane")!;
            if (list.hasPointerCapture(o.pointerId)) {
                list.releasePointerCapture(event['originalEvent'].pointerId);
                innerState!.onPointerUp(event);
            }
        },
        "lostpointercapture"(event, presenter, innerState) {
            innerState!.onLostCapture();
            innerState!.cancelContextMenuTimeout();
            innerState!.cancelDragging();
        },
        "scroll #preset-search-results-scroller"(event, presenter, innerState) {
            innerState!.cancelContextMenuTimeout();
            innerState!.cancelDragging();
            scrollY.set(Math.max(0, event['target'].scrollTop));
            $("#preset-search-results").css("top", getSearchResultScrollOffset() + "px");
            Tracker.flush();
        },
        "contextmenu"(event, presenter, innerState) {
            if (!innerState!.isContextMenuAllowed()) {
                event.stopImmediatePropagation();
                event.preventDefault();
                return;
            }
            innerState!.onContextMenu();
        }
    });


$['contextMenu']({
    selector: '#preset-search-results-scroller-pane',
    reposition: false,
    presetUUID: "",
    events: {
        show: function (e) {
            currentContextMenuItem.set(currentContextMenuItem.get().concat([e.presetUUID]));
        },
        hide: function (e) {
            currentContextMenuItem.set(currentContextMenuItem.get().filter(v => v != e.presetUUID));
        }
    },
    build: (_element, event: any) => {
        const scroller = document.getElementById("preset-search-results-scroller")!;
        const top = scroller.getBoundingClientRect().top;
        const originalEvent = event['originalEvent'];
        var presetHeight = getPresetHeight();

        var offsetY = 0;
        if (originalEvent) {
            offsetY = scrollY.get() + originalEvent.clientY - top;
        }
        else {
            offsetY = scrollY.get() + event.pageY - top;
        }

        var idx = offsetY / presetHeight;
        const searchResult = cachedSearch.get();
        const presetId = searchResult[(Math.floor(idx))]!;

        if (multipleSelection.get() && !multipleSelection.get()?.includes(presetId))
            return false;

        event.data.presetUUID = presetId;

        function addColorBoxToMenuItem(_opt: any, $itemElement: Element, itemKey: string) {
            var s = $itemElement[0].querySelector("span.color");
            if (!s || s.length == 0)
                $itemElement.prepend('<span class="' + itemKey + ' color"></span>');
        }

        function setSelectedPresetColors(itemKey: string) {
            if (multipleSelection.get()) {
                for (var p of multipleSelection.get()!) {
                    playgroundProxy.setPresetAttribute(p, "color", itemKey);
                }
            } else {
                playgroundProxy.setPresetAttribute(presetId, "color", itemKey);
            }
        }

        return {
            items: {
                "multiple": {
                    name: multipleSelection.get() ? "Disable Multiple Selection" : "Start Multiple Selection",
                    callback: function () {
                        multipleSelectionStartedByShiftClick.set(false);
                        multipleSelection.get() ? multipleSelection.set(null) : multipleSelection.set([presetId])
                    }
                },
                "bank": {
                    name: "Show in Bank",
                    callback: function () {
                        window['scrollToNonMapsPreset'](presetId);
                    }
                },
                "info": {
                    name: "Preset Info",
                    visible: function (key, opt) {
                        return !window['isPresetInfoVisible']();
                    },
                    callback: function () {
                        playgroundProxy.selectPreset(presetId);
                        window['showPresetInfo'](presetId);
                    }
                },
                "select-all": {
                    name: "Select All",
                    callback: function () {
                        multipleSelectionStartedByShiftClick.set(true);
                        multipleSelection.set([...cachedSearch.get()]);
                    }
                },
                "compare-to-editbuffer": {
                    name: "Compare to Editbuffer...",
                    visible: function (key, opt) {
                        return !multipleSelection.get();
                    },
                    callback: function () {
                        window['comparePresetToEditbuffer'](presetId);
                    }
                },
                "compare-to-preset": {
                    name: "Compare...",
                    visible: function (key, opt) {
                        return multipleSelection.get() && multipleSelection.get()?.length == 2;
                    },
                    callback: function () {
                        window['comparePresets'](multipleSelection.get()![0], multipleSelection.get()![1]);
                    }
                },
                "color-tag": {
                    name: "Color Tag",
                    className: "color-tags",
                    items: {
                        green: { name: "Green", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors },
                        blue: { name: "Blue", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors },
                        yellow: { name: "Yellow", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors },
                        orange: { name: "Orange", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors },
                        purple: { name: "Purple", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors },
                        red: { name: "Red", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors },
                        none: { name: "None", icon: addColorBoxToMenuItem, callback: setSelectedPresetColors }
                    }
                }
            }
        }
    }
});

