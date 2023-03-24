// shared state and methods
const shared = (function() {
    // start phase - minimal handle
    const
        colorThemeQuery = matchMedia("(prefers-color-scheme: dark)"),
        handle = {
            params: new URLSearchParams(location.search),
            get developerEnabled() {
                return this.params.has("developer");
            },
            get theme() {
                if(!this.params.has("theme")) return "auto";
                return this.params.get("theme");
            },
            get colorThemeMatch() {
                return colorThemeQuery.matches ? "dark" : "light";
            },
            createUrl(href, params = {}, withTheme = true, withDeveloper = true) {
                // create url reflecting theme and possibly more parameters
                if(withDeveloper & this.developerEnabled) params.developer = true;
                const
                    theme = this.theme,
                    paramList = withTheme ? (theme === "auto" ? [] : [`theme=${theme}`]) : [],
                    keys = Object.keys(params);
                paramList.push(...keys.map((key) => `${key}=${params[key]}`));
                if(paramList.length === 0) return href;
                return [href, paramList.join("&")].join("?");
            }
        };
    // redirect phase (by <script data-redirect="url"></script>)
    if("redirect" in document.currentScript.dataset) {
        document.location.replace(handle.createUrl(document.currentScript.dataset.redirect));
    }
    // color theme phase
    function applyColorThemeToHTML() {
        const theme = handle.theme === "auto" ? handle.colorThemeMatch : handle.theme;
        document.querySelector("html").dataset.colorTheme = theme;
    }
    applyColorThemeToHTML(); // as early as possible
    // preparation phase
    function applyColorThemeToPictures() {
        const updatePictureEvent = new CustomEvent("update", {detail: {type: "theme", theme: handle.theme}});
        for(const picture of document.querySelectorAll("picture[data-src]")) picture.dispatchEvent(updatePictureEvent);
    }
    Object.assign(handle, {
        config: {
            // "figure-print-default": "print"
            "figure-print-default": "light",
            // known utf codes
            "utf-codes": {
                //
                "nl-logo": 0xE185,
                // macros
                "mc-a": 0xE000, "mc-b": 0xE001, "mc-c": 0xE002, "mc-d": 0xE003, "mc-e": 0xE200, "mc-f": 0xE201,
                // indicators
                "ind-mono": 0xE040, "ind-unison": 0xE041, "ind-scale": 0xE042,
                "ind-part-1": 0xE081, "ind-part-2": 0xE082,
                "ind-mute": 0xE0BA, "ind-midi": 0xE0C1, "ind-direct-load": 0xE0CA,
                "ind-split": 0xE0F2, "ind-layer": 0xE0F5, "ind-lock": 0xE20A, "ind-active": 0xE177,
                // highlight
                "hl-comment": 0xE173, "hl-note": 0xE161, "hl-warn": 0xE170, "hl-excursion": 0xE182, "hl-recap": 0xE184,
                // inputs
                "inp-radio": 0xE0A2, "inp-check": 0xE0A4,
                // menu icons
                "menu-icon": 0xE274, "menu-close": 0xE275, "menu-minimize": 0xE276, "menu-maximize": 0xE277, "menu-search": 0xE180,
                // links
                "a-ext": 0xE279,
                // triangles
                "tri-up": 0xE300, "tri-right": 0xE301, "tri-down": 0xE302, "tri-left": 0xE303
            }
        },
        // get search() {
        //     if(!this.params.has("search")) return "";
        //     return this.params.get("search");
        // },
        applyColorTheme() {
            applyColorThemeToHTML();
            applyColorThemeToPictures();
        },
        highlightCurrent(path) {
            // highlight current document, chapter, subchapter according to path
            if(!path) return;
            const list = path.split("/");
            list.forEach((key, index) => {
                for(const node of document.querySelectorAll(`[data-pathname="${key}"]`)) node.classList.add("current");
                if(index === list.length - 1) {
                    for(const node of document.querySelectorAll(`[data-pathname="${key}"]`)) {
                        node.classList.add("target");
                        node.scrollIntoView();
                    }
                }
            });
        },
        setScrolling(wrapperSelector, buttonSelector) {
            const
                wrapper = document.querySelector(wrapperSelector),
                button = document.querySelector(buttonSelector);
            wrapper.addEventListener("scroll", (event) => {
                button.toggleAttribute("visible", (wrapper.scrollTop > (0.5 * wrapper.parentElement.scrollHeight)));
            });
            return () => {
                wrapper.scrollTop = 0;
            };
        },
        createUtf(code) {
            if(code in this.config["utf-codes"]) {
                return String.fromCharCode(this.config["utf-codes"][code]);
            }
            return String.fromCharCode(parseInt(code));
        }
    });
    // main init phase (load event)
    window.addEventListener("load", () => {
        // prepare and apply color theme
        colorThemeQuery.addEventListener("change", () => handle.applyColorTheme());
        applyColorThemeToPictures();
        // developer
        if(handle.developerEnabled) {
            for(const item of document.querySelectorAll(".developer")) item.classList.remove("developer");
        }
        // make internal anchors reflect current settings (theme, developer)
        for(const anchor of document.querySelectorAll("a[href]")) {
            // ... if they are not related to settings
            if(!("setting" in anchor.dataset) && (anchor.host === location.host)) {
                anchor.setAttribute("href", handle.createUrl(anchor.getAttribute("href")));
            }
        }
        // utf
        for(const span of document.querySelectorAll("span[data-utf]")) {
            span.textContent = handle.createUtf(span.dataset.utf);
        }
    });
    // return handle
    return Object.freeze(handle);
})();
