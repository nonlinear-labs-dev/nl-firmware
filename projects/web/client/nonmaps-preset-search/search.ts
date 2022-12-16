import { ReactiveVar } from "meteor/reactive-var";
import { Template } from "meteor/templating";
import { Tracker } from "meteor/tracker";
import { globals, glue } from "../glue";
import "./search-result-preset";
import "./search.html";
import { cachedSearch, getPresetHeight, scrollY } from "./shared-state";

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
    console.log("start search");

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

    console.log("finished search with #results: ", ret.length);
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
    return scrollY.get() - scrollY.get() % getPresetHeight();
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
    var toggleButton = $(".banks .toggle")[0];
    if (toggleButton == event.target)
        return;

    if (isEventOutsideVisibleBankSelectPane(event))
        showBankSelectPane.set(false);
}, true);


document.addEventListener('wheel', (event) => {
    if (isEventOutsideVisibleBankSelectPane(event))
        event.stopImmediatePropagation();
}, true);

glue("nonmapsPresetSearch",
    (id, innerState) => {
        const searchResult = cachedSearch.get();
        const options = searchOptions.get().sorting;
        const maxPresetsVisibleOnOnePage = 100;
        const scroller = document.getElementById("preset-search-results-scroller");

        if (scroller) {
            var presetHeight = getPresetHeight();
            var paneHeight = presetHeight * searchResult.length;
            var maxScroll = paneHeight - scroller!.clientHeight;
            scrollY.set(Math.max(0, Math.min(maxScroll, scrollY.get())));
        }

        return {
            allPresets: searchResult,
            shownPresets: Array.from(Array(maxPresetsVisibleOnOnePage).keys()),
            collapsed: searchCollapsed.get(),
            numResults: searchResult.length,
            hasSearchQuery: searchQuery.get().length > 0
        }
    }, () => {
        syncCollapsedState(true);
        return {
            lastKnownPointerY: 0
        }
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
    },
    "presetSearchResultsScrollerAttributes"(presenter) {
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
    "presetSearchResultsScrollerPaneAttributes"(presenter) {
        var presetHeight = getPresetHeight();
        var paneHeight = presetHeight * presenter.allPresets.length;

        return {
            id: "preset-search-results-scroller-pane",
            style: "height: " + paneHeight + "px"
        }
    },
    "presetSearchResultsAttributes"(presenter, innerState) {
        const ph = getPresetHeight();
        const scroll = scrollY.get();
        const offset = scroll / ph;
        const firstPresetIdx = Math.floor(offset);
        const numPresets = presenter.allPresets.length;
        const h = (numPresets - firstPresetIdx) * ph;

        return {
            id: "preset-search-results",
            style: "top:" + getSearchResultScrollOffset() + "px; height: " + h + "px;"
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
        },
        "scrollY"(event: JQuery.Event, presenter, innerState) {
            const list = document.getElementById("preset-search-results-scroller")!;
            list.setPointerCapture(event['originalEvent'].detail.pointerId);
            list.scrollBy(0, event['originalEvent'].detail.diffY);
            innerState!.lastKnownPointerY = event['originalEvent'].detail.pageY;
        },
        "pointermove"(event: JQuery.Event, presenter, innerState) {
            const list = document.getElementById("preset-search-results-scroller")!;
            if (list && list.hasPointerCapture(event['originalEvent'].pointerId)) {
                const pageY = event['originalEvent'].pageY;
                const diff = innerState!.lastKnownPointerY - pageY;
                innerState!.lastKnownPointerY = pageY;
                list.scrollBy(0, diff);
            }
        },
        "pointerup"(event: JQuery.Event) {
            const list = document.getElementById("preset-search-results-scroller")!;
            if (list)
                list.releasePointerCapture(event['originalEvent'].pointerId);
        },
        "scroll #preset-search-results-scroller"(event) {
            scrollY.set(event['target'].scrollTop);
            $("#preset-search-results").css("top", getSearchResultScrollOffset() + "px");
            Tracker.flush();
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
                $(".settings-pane").slideDown();
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
                }
                else {
                    a['style'] = "display: flex";
                    a['class'] = "disabled";
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
                }
                else {
                    a['style'] = "display: flex";
                    a['class'] = "disabled";
                }
            }
            return a;
        },
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
