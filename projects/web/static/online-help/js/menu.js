// update selected tab
function selectTab(tab) {
    const tabs = document.querySelector("#menu-tabs");
    // deselect currently selected tab
    for(const node of tabs.querySelectorAll("[selected]")) node.removeAttribute("selected");
    // select specified tab
    for(const node of tabs.querySelectorAll(`[tab="${tab}"]`)) node.toggleAttribute("selected", true);
}

const
    parentUrl = shared.params.get("url") || "./index.html",
    currentTheme = shared.theme,
    searchInput = document.querySelector("input#search"),
    documentFrame = document.querySelector("#documents-frame");

function onSearch(event) {
    event.stopPropagation();
    event.preventDefault();
    const search = encodeURIComponent(searchInput.value.trim());
    if(search) {
        const anchor = document.createElement("a");
        anchor.setAttribute("href", shared.createUrl(parentUrl, {search}));
        anchor.click();
    }
    searchInput.blur();
}

// make setting anchors refer to current file, highlight current theme
for(const anchor of document.querySelectorAll('[data-setting]')) {
    anchor.textContent = anchor.dataset.value;
    switch(anchor.dataset.setting) {
        case "theme": {
            const theme = anchor.dataset.value;
            if(theme === "auto") {
                anchor.setAttribute("href", shared.createUrl(parentUrl, {tab: "settings"}, false));
            } else {
                anchor.setAttribute("href", shared.createUrl(parentUrl, {theme, tab: "settings"}, false));
            }
            if(currentTheme === theme) {
                anchor.classList.add("selected");
            }
            break;
        }
        case "developer": {
            if(anchor.textContent === "off") {
                anchor.classList.toggle("selected", !shared.developerEnabled);
                anchor.setAttribute("href", shared.createUrl(parentUrl, {tab: "settings"}, true, false));
            } else {
                anchor.classList.toggle("selected", shared.developerEnabled);
                anchor.setAttribute("href", shared.createUrl(parentUrl, {tab: "settings", developer: true}, true, false));
            }
            break;
        }
        case "search": {
            anchor.setAttribute("href", shared.createUrl(parentUrl));
            if(anchor.dataset.value === "Search") {
                anchor.addEventListener("click", onSearch);
            }
        }
    }
    // anchor.addEventListener("mouseenter", e => {
    //     console.log(`${anchor.dataset.setting}=${anchor.dataset.value}: ${anchor.search}`);
    // });
}

// console.log(document.location.search);

searchInput.addEventListener("keydown", (event) => {
    switch(event.key.toLowerCase()) {
        case "enter":
            onSearch(event);
            break;
        case "escape":
            searchInput.blur();
            break;
    }
});

// load documents content page
documentFrame.setAttribute("src", shared.createUrl("./documents/_content.html", {
    path: shared.params.get("path"),
    search: shared.search
}));

// init
selectTab(shared.params.get("tab"));
if(shared.search) searchInput.value = shared.search;
