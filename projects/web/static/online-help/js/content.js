// init
shared.highlightCurrent(shared.params.get("path"));
const scrollToTop = shared.setScrolling("#toc-wrapper", "#toc-totop-button");

// if(shared.search) console.log(shared.search.split(" "));

// for(const item of document.querySelectorAll("nl-dir, nl-doc")) {
//     const hasContent = item.children.length > 0;
//     const label = item.insertBefore(document.createElement("label"), item.firstChild);
//     if(hasContent) {
//         const symbol = label.appendChild(document.createElement("span"));
//         symbol.innerHTML = item.hasAttribute("open") ? "&#xE302" : "&#xE301;";
//     }
//     const anchor = label.appendChild(document.createElement("a"));
//     anchor.textContent = item.getAttribute("label");
//     anchor.classList.add("label");
//     if(item.hasAttribute("href")) {
//         anchor.setAttribute("href", item.getAttribute("href"));
//     }
// }
