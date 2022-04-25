// prepare menuwrapper
const
    hasTabParameter = shared.params.has("tab"),
    menuUrl = shared.createUrl("./_menu.html", {
        url: location.pathname,
        path: document.body.dataset.path,
        tab: hasTabParameter ? shared.params.get("tab") : "documents",
        search: shared.search
    }),
    menuWrapper = document.querySelector("#menu-wrapper");

menuWrapper.innerHTML += `
    <button id="menu-main-button" class="menu" onclick="toggleMenu()">
        <div mask="menu"></div>
    </button>
    <iframe id="menu-frame" src="${menuUrl}" frameborder="0" scrolling="no"></iframe>
    <footer id="menu-footer">
        <button id="menu-totop-button" class="menu" onclick="scrollToTop()">
            <div mask="totop"></div>
        </button>
    </footer>
    <div class="main-grid" id="menu-overlay">
        <label id="menu-overlay-label" class="label"></label>
        <button id="menu-overlay-button" class="menu" onclick="closeOverlay()">
            <div mask="close"></div>
        </button>
        <div id="menu-overlay-content"></div>
    </div>
`;

const
    menuOverlay = menuWrapper.querySelector("#menu-overlay"),
    menuOverlayLabel = menuOverlay.querySelector("#menu-overlay-label"),
    menuOverlayContent = menuOverlay.querySelector("#menu-overlay-content"),
    menuOverlayAvailableItems = Array.from(document.querySelectorAll("figure.maximize"));

// required functions
function toggleMenu() {
    const frame = document.querySelector("#menu-frame");
    document.querySelector("#menu-main-button > [mask]").setAttribute("mask", frame.hasAttribute("visible") ? "menu" : "close");
    frame.toggleAttribute("visible");
}

function overlayLabel(node) {
    if("label" in node.dataset) {
        menuOverlayLabel.textContent = `${node.dataset.label} [${parseInt(node.dataset.index) + 1}/${menuOverlayAvailableItems.length}]`;
    } else {
        menuOverlayLabel.textContent = "";    
    }
}

function openOverlay(node) {
    overlayLabel(node);
    menuOverlayContent.appendChild(node.cloneNode(true));
    menuOverlayContent.firstChild.classList.add("is-maximized");
    menuOverlay.toggleAttribute("visible", true);
}

function closeOverlay() {
    menuOverlayLabel.textContent = "";
    menuOverlay.removeAttribute("visible");
    menuOverlayContent.innerHTML = "";
}

function replaceOverlay(node) {
    menuOverlayContent.innerHTML = "";
    overlayLabel(node);
    menuOverlayContent.appendChild(node.cloneNode(true));
}

// tidy up url
if(hasTabParameter) {
    const
        href = location.href.split("?")[0],
        search = location.search.slice(1).replace(/&?tab=\w+/, ""),
        result = href + (search.length > 0 ? `?${search}` : "");
    history.replaceState(history.state, document.title, result);
}

// pictures
for(const pic of document.querySelectorAll("picture[data-src]")) {
    const
    names = {
        dark: pic.dataset.src.replace("*", "dark"),
        light: pic.dataset.src.replace("*", "light"),
        print: pic.dataset.src.replace("*", shared.config["figure-print-default"]) // using defaul (shared config)
    },
    sources = {};
    for(const theme of ["dark", "light"]) {
        sources[theme] = pic.appendChild(document.createElement("source"));
        sources[theme].classList.add(theme);
        sources[theme].setAttribute("srcset", names[theme]);
        sources[theme].setAttribute("media", `(prefers-color-scheme: ${theme})`);
    }
    sources.print = pic.appendChild(document.createElement("img"));
    sources.print.setAttribute("src", names.print);
    pic.addEventListener("update", (event) => {
        switch(event.detail.type) {
            case "beforeprint": {
                sources.dark.setAttribute("srcset", names.print);
                sources.light.setAttribute("srcset", names.print);
                break;
            }
            case "afterprint":
            case "theme": {
                if(event.detail.theme === "auto") {
                    sources.dark.setAttribute("srcset", names.dark);
                    sources.light.setAttribute("srcset", names.light);
                } else {
                    sources.dark.setAttribute("srcset", names[event.detail.theme]);
                    sources.light.setAttribute("srcset", names[event.detail.theme]);
                };
                break;
            }
        }
    });
}

// columns
for(const node of document.querySelectorAll("[data-columns]")) {
    node.classList.add("columns");
    node.style.gridTemplateColumns = `repeat(${node.dataset.columns}, 1fr)`;
}

// highlight
for(const node of document.querySelectorAll("[data-highlight")) {
    node.classList.add("highlight", node.dataset.highlight);
    const mask = node.insertBefore(document.createElement("div"), node.firstChild);
    switch(node.dataset.highlight) {
        case "comment":
        case "note":
        case "warn":
        case "excursion":
        case "recap": {
            mask.textContent = shared.createUtf(`hl-${node.dataset.highlight}`);
            break;
        }
        default: {
            console.error("unknown [data-highlight] attribute", node);
            break;
        }
    }
};

// detail
for(const node of document.querySelectorAll("[data-detail")) {
    const detail = node.appendChild(document.createElement("div"));
    detail.classList.add("detail");
    detail.textContent = `[ ${node.dataset.detail} ]`;
}

// keyword
for(const node of document.querySelectorAll("[data-keyword")) {
    const keyword = node.textContent;
    node.classList.add("keyword");
    node.innerHTML = `<code class="key">${node.dataset.keyword}</code>${keyword}`;
}

// figures
for(const fig of menuOverlayAvailableItems) {
    fig.dataset.index = menuOverlayAvailableItems.indexOf(fig);
    const btn = fig.appendChild(document.createElement("button"));
    btn.classList.add("menu", "maximize");
    btn.innerHTML = '<div mask="maximize"></div>';
    btn.addEventListener("click", () => openOverlay(fig));
}

// window event bindings (print)
window.addEventListener("beforeprint", () => {
    const event = new CustomEvent("update", {detail: {type: "beforeprint"}});
    for(const pic of document.querySelectorAll("picture[data-src]")) {
        pic.dispatchEvent(event);
    }
});
window.addEventListener("afterprint", () => {
    const event = new CustomEvent("update", {detail: {type: "afterprint", theme: shared.theme}});
    for(const picture of document.querySelectorAll("picture[data-src]")) {
        picture.dispatchEvent(event);
    }
});

// key event bindings (overlay)
document.addEventListener("keydown", (event) => {
    if(!event.repeat)
    {
        if(menuOverlay.hasAttribute("visible")) switch(event.key) {
            case "Escape": {
                closeOverlay();
                break;
            }
            case "ArrowLeft": {
                const index = parseInt(menuOverlayContent.firstChild.dataset.index) - 1;
                if(index > -1) replaceOverlay(menuOverlayAvailableItems[index]);
                break;
            }
            case "ArrowRight": {
                const index = parseInt(menuOverlayContent.firstChild.dataset.index) + 1;
                if(index < menuOverlayAvailableItems.length) replaceOverlay(menuOverlayAvailableItems[index]);
                break;
            }
        }
    }
    
    //

});

// update selected tab
function selectTab(event) {
    const tabs = event.target.closest("dl.tabs")
    // deselect currently selected tab
    for(const node of tabs.querySelectorAll("[selected]")) node.removeAttribute("selected");
    // select specified tab
    for(const node of tabs.querySelectorAll(`[tab="${event.target.getAttribute("tab")}"]`)) node.toggleAttribute("selected", true);
}

// main init
shared.highlightCurrent(document.body.dataset.path);
const scrollToTop = shared.setScrolling("#content-wrapper", "#menu-totop-button");

// if(shared.search) console.log(shared.search.split(" "));

