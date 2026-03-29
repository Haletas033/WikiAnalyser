//
// Created by halet on 3/29/2026.
//

#ifndef WIKIANALYSER_CSS_H
#define WIKIANALYSER_CSS_H

inline void wikiAnalyserStyles() {
    Style img("img");
    img
        .put("display", "block")
        .put("margin-left", "auto")
        .put("margin-right", "auto")
        .put("width", "100%")
        .put("max-width", "1280px")
        .put("object-fit", "cover")
        .put("object-position", "center")
        .put("height", "400px");

    Style sections("article");
    sections
        .put("background-color", "#f8f9fa")
        .put("border", "1px solid #a2a9b1")
        .put("padding", "1em")
        .put("width", "100%")
        .put("max-width", "1280px")
        .put("margin", "auto")
        .put("margin-bottom", "1em");

    Style getStartedLinks(".getStarted div");
    getStartedLinks
        .put("display", "flex")
        .put("justify-content", "center")
        .put("gap", "1em");

    Style navBar("nav ul");
    navBar
        .put("display", "flex")
        .put("align-items", "center")
        .put("gap", "1em");

    Style navBarLi("nav ul li");
    navBarLi
        .put("list-style", "none")
        .put("font-size", "1.2em");

    Builder::Build("wikiAnalyser");
}

#endif //WIKIANALYSER_CSS_H
