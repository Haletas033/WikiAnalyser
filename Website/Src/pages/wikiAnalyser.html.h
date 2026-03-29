//
// Created by halet on 3/29/2026.
//

#ifndef WIKIANALYSER_HTML_H
#define WIKIANALYSER_HTML_H

#include "Tag.h"
#include "utils/misc/misc.h"
#include "utils/nav/nav.h"

inline void wikiAnalyser() {
    Tag header("header");
    header.put(nav::buildNavbar());

    Tag main("main");
    main.addAttr("class", "container");

    main.put(Tag ("img").addAttr("src", "imgs/WikiAnalyser.webp").addAttr("alt", "WikiAnalyser"));

    Tag features("article");
    features.put(h2("Features:"));

    Tag featuresList("ul");
    featuresList.put(Tag ("li").text("Parse huge XML files safely and efficiently"));
    featuresList.put(Tag ("li").text("Work with any XML dump that matches <page><title>title</title>main content</page>"));
    featuresList.put(Tag ("li").text("Built from scratch in C90 only using Win32, Curl, and the C Standard library"));
    featuresList.put(Tag ("li").text("Build an article struct and write Zig to fill it in for every article"));
    features.put(featuresList);
    main.put(features);

    Tag philosophy("article");
    philosophy.put(h2("Philosophy / The Goal"));

    philosophy.put(p(R"(
        WikiAnalyser is an open-source project written in C90. It aims to be a quick and simple solution for analysing large amounts of pages; Mainly Wikipedia.
        WikiAnalyser provides a way to fetch Wikipedia data, clean it up, create an article struct, write zig to fill those fields in, and draw graphs all in one.
        WikiAnalyser is fully open to any ideas, feedback, fixes, or contributions.
    )"));
    main.put(philosophy);

    Tag getStarted("article");
    getStarted.put(h2("Get Started:")).addAttr("class", "getStarted");

    Tag getStartedLinksDiv("div");
    getStartedLinksDiv
        .put(Tag ("a").addAttr("href", "https://github.com/Haletas033/WikiAnalyser/releases/download/release/WikiAnalyser.zip").text("Download Latest"))
        .put(Tag ("span").text("|"))
        .put(Tag ("a").addAttr("href", "https://github.com/Haletas033/HaletasWeb").text("Visit Repo"));
    getStarted.put(getStartedLinksDiv);
    main.put(getStarted);


    Tag wikiAnalyserHead("head");
    wikiAnalyserHead
            .put(Tag("meta").addAttr("charset", "UTF-8"))
            .put(Tag("meta").addAttr("name", "viewport").addAttr("content", "width=device-width, initial-scale=1.0"))
            .put(Tag("link").addAttr("rel", "stylesheet").addAttr("href", "styles/wikiAnalyser.css"));


    WriteHTML("wikiAnalyser.html", wikiAnalyserHead, header, main, "WikiAnalyser", Tag("style").text(misc::extraStyles));
}

#endif //WIKIANALYSER_HTML_H
