/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* The prefs in this file are shipped with the GRE and should apply to all
 * embedding situations. Application-specific preferences belong somewhere else,
 * for example xpfe/bootstrap/browser-prefs.js
 *
 * Platform-specific #ifdefs at the end of this file override the generic
 * entries at the top.
 */

/*
 * SYNTAX HINTS:
 *
 *  - Dashes are delimiters; use underscores instead.
 *  - The first character after a period must be alphabetic.
 *  - Computed values (e.g. 50 * 1024) don't work.
 */

#ifdef XP_MACOSX
// Whether to run in native HiDPI mode on machines with "Retina"/HiDPI display;
//   <= 0 : hidpi mode disabled, display will just use pixel-based upscaling
//   == 1 : hidpi supported if all screens share the same backingScaleFactor
//   >= 2 : hidpi supported even with mixed backingScaleFactors (somewhat broken)
pref("gfx.hidpi.enabled", 2);
#endif

// Log severe performance warnings to the error console and profiles.
// This should be use to quickly find which slow paths are used by test cases.
pref("gfx.perf-warnings.enabled", false);

// 0 = Off, 1 = Full, 2 = Tagged Images Only.
// See eCMSMode in gfx/thebes/gfxPlatform.h
pref("gfx.color_management.mode", 2);
pref("gfx.color_management.display_profile", "");
pref("gfx.color_management.rendering_intent", 0);
pref("gfx.color_management.enablev4", false);

pref("gfx.downloadable_fonts.enabled", true);
pref("gfx.downloadable_fonts.fallback_delay", 3000);
pref("gfx.downloadable_fonts.fallback_delay_short", 100);

// disable downloadable font cache so that behavior is consistently
// the uncached load behavior across pages (useful for testing reflow problems)
pref("gfx.downloadable_fonts.disable_cache", false);

pref("gfx.downloadable_fonts.woff2.enabled", true);

#ifdef ANDROID
pref("gfx.bundled_fonts.enabled", true);
pref("gfx.bundled_fonts.force-enabled", false);
#endif

// Do we fire a notification about missing fonts, so the front-end can decide
// whether to try and do something about it (e.g. download additional fonts)?
pref("gfx.missing_fonts.notify", false);

// prefs controlling the font (name/cmap) loader that runs shortly after startup
pref("gfx.font_loader.families_per_slice", 3); // read in info 3 families at a time
#ifdef XP_WIN
pref("gfx.font_loader.delay", 120000);         // 2 minutes after startup
pref("gfx.font_loader.interval", 1000);        // every 1 second until complete
#else
pref("gfx.font_loader.delay", 8000);           // 8 secs after startup
pref("gfx.font_loader.interval", 50);          // run every 50 ms
#endif

// whether to always search all font cmaps during system font fallback
pref("gfx.font_rendering.fallback.always_use_cmaps", false);

// cache shaped word results
pref("gfx.font_rendering.wordcache.charlimit", 32);

// cache shaped word results
pref("gfx.font_rendering.wordcache.maxentries", 10000);

pref("gfx.font_rendering.graphite.enabled", true);

#ifdef XP_WIN
pref("gfx.font_rendering.directwrite.force-enabled", false);
pref("gfx.font_rendering.directwrite.use_gdi_table_loading", true);
#endif

pref("gfx.font_rendering.opentype_svg.enabled", true);

#ifdef XP_WIN
// comma separated list of backends to use in order of preference
// e.g., pref("gfx.canvas.azure.backends", "direct2d,skia,cairo");
pref("gfx.canvas.azure.backends", "direct2d1.1,skia,cairo");
pref("gfx.content.azure.backends", "direct2d1.1,cairo");
#else
#ifdef XP_MACOSX
pref("gfx.content.azure.backends", "cg,skia");
pref("gfx.canvas.azure.backends", "cg,skia");
// Accelerated cg canvas where available (10.7+)
pref("gfx.canvas.azure.accelerated", true);
#else
pref("gfx.canvas.azure.backends", "skia");
pref("gfx.content.azure.backends", "cairo");
#endif
#endif

pref("gfx.work-around-driver-bugs", true);
pref("gfx.prefer-mesa-llvmpipe", false);

pref("gfx.draw-color-bars", false);

pref("gfx.logging.painted-pixel-count.enabled", false);
pref("gfx.logging.texture-usage.enabled", false);
pref("gfx.logging.peak-texture-usage.enabled", false);

pref("gfx.ycbcr.accurate-conversion", false);

// provide ability to turn on support for canvas focus rings
pref("canvas.focusring.enabled", true);
pref("canvas.customfocusring.enabled", false);
pref("canvas.hitregions.enabled", false);
pref("canvas.filters.enabled", true);
// Add support for canvas path objects
pref("canvas.path.enabled", true);
pref("canvas.capturestream.enabled", true);

// Disable the ImageBitmap-extensions in the release build.
#ifdef RELEASE_BUILD
pref("canvas.imagebitmap_extensions.enabled", false);
#else
pref("canvas.imagebitmap_extensions.enabled", true);
#endif

// use Mac OS X Appearance panel text smoothing setting when rendering text, disabled by default
pref("gfx.use_text_smoothing_setting", false);


pref("intl.accept_languages",               "chrome://global/locale/intl.properties");
pref("intl.menuitems.alwaysappendaccesskeys","chrome://global/locale/intl.properties");
pref("intl.menuitems.insertseparatorbeforeaccesskeys","chrome://global/locale/intl.properties");
pref("intl.charset.detector",               "chrome://global/locale/intl.properties");
pref("intl.charset.fallback.override",      "");
pref("intl.charset.fallback.tld",           true);
pref("intl.ellipsis",                       "chrome://global-platform/locale/intl.properties");
pref("intl.locale.matchOS",                 false);
// fallback charset list for Unicode conversion (converting from Unicode)
// currently used for mail send only to handle symbol characters (e.g Euro, trademark, smartquotes)
// for ISO-8859-1
pref("intl.fallbackCharsetList.ISO-8859-1", "windows-1252");
pref("font.language.group",                 "chrome://global/locale/intl.properties");

// Android-specific pref to use key-events-only mode for IME-unaware webapps.
#ifdef MOZ_WIDGET_ANDROID
pref("intl.ime.hack.on_ime_unaware_apps.fire_key_events_for_composition", true);
#else
pref("intl.ime.hack.on_ime_unaware_apps.fire_key_events_for_composition", false);
#endif

// If you use legacy Chinese IME which puts an ideographic space to composition
// string as placeholder, this pref might be useful.  If this is true and when
// web contents forcibly commits composition (e.g., moving focus), the
// ideographic space will be ignored (i.e., commits with empty string).
pref("intl.ime.remove_placeholder_character_at_commit", false);

// these locales have right-to-left UI
pref("intl.uidirection.ar", "rtl");
pref("intl.uidirection.he", "rtl");
pref("intl.uidirection.fa", "rtl");
pref("intl.uidirection.ug", "rtl");
pref("intl.uidirection.ur", "rtl");

// use en-US hyphenation by default for content tagged with plain lang="en"
pref("intl.hyphenation-alias.en", "en-us");
// and for other subtags of en-*, if no specific patterns are available
pref("intl.hyphenation-alias.en-*", "en-us");

pref("intl.hyphenation-alias.af-*", "af");
pref("intl.hyphenation-alias.bg-*", "bg");
pref("intl.hyphenation-alias.ca-*", "ca");
pref("intl.hyphenation-alias.cy-*", "cy");
pref("intl.hyphenation-alias.da-*", "da");
pref("intl.hyphenation-alias.eo-*", "eo");
pref("intl.hyphenation-alias.es-*", "es");
pref("intl.hyphenation-alias.et-*", "et");
pref("intl.hyphenation-alias.fi-*", "fi");
pref("intl.hyphenation-alias.fr-*", "fr");
pref("intl.hyphenation-alias.gl-*", "gl");
pref("intl.hyphenation-alias.hr-*", "hr");
pref("intl.hyphenation-alias.hsb-*", "hsb");
pref("intl.hyphenation-alias.hu-*", "hu");
pref("intl.hyphenation-alias.ia-*", "ia");
pref("intl.hyphenation-alias.is-*", "is");
pref("intl.hyphenation-alias.it-*", "it");
pref("intl.hyphenation-alias.kmr-*", "kmr");
pref("intl.hyphenation-alias.la-*", "la");
pref("intl.hyphenation-alias.lt-*", "lt");
pref("intl.hyphenation-alias.mn-*", "mn");
pref("intl.hyphenation-alias.nl-*", "nl");
pref("intl.hyphenation-alias.pl-*", "pl");
pref("intl.hyphenation-alias.pt-*", "pt");
pref("intl.hyphenation-alias.ru-*", "ru");
pref("intl.hyphenation-alias.sl-*", "sl");
pref("intl.hyphenation-alias.sv-*", "sv");
pref("intl.hyphenation-alias.tr-*", "tr");
pref("intl.hyphenation-alias.uk-*", "uk");

// use reformed (1996) German patterns by default unless specifically tagged as de-1901
// (these prefs may soon be obsoleted by better BCP47-based tag matching, but for now...)
pref("intl.hyphenation-alias.de", "de-1996");
pref("intl.hyphenation-alias.de-*", "de-1996");
pref("intl.hyphenation-alias.de-AT-1901", "de-1901");
pref("intl.hyphenation-alias.de-DE-1901", "de-1901");
pref("intl.hyphenation-alias.de-CH-*", "de-CH");

// patterns from TeX are tagged as "sh" (Serbo-Croatian) macrolanguage;
// alias "sr" (Serbian) and "bs" (Bosnian) to those patterns
// (Croatian has its own separate patterns).
pref("intl.hyphenation-alias.sr", "sh");
pref("intl.hyphenation-alias.bs", "sh");
pref("intl.hyphenation-alias.sh-*", "sh");
pref("intl.hyphenation-alias.sr-*", "sh");
pref("intl.hyphenation-alias.bs-*", "sh");

// Norwegian has two forms, Bokmål and Nynorsk, with "no" as a macrolanguage encompassing both.
// For "no", we'll alias to "nb" (Bokmål) as that is the more widely used written form.
pref("intl.hyphenation-alias.no", "nb");
pref("intl.hyphenation-alias.no-*", "nb");
pref("intl.hyphenation-alias.nb-*", "nb");
pref("intl.hyphenation-alias.nn-*", "nn");

pref("font.name.serif.x-math", "Latin Modern Math");
pref("font.name-list.serif.x-math", "Latin Modern Math, XITS Math, Cambria Math, Libertinus Math, DejaVu Math TeX Gyre, TeX Gyre Bonum Math, TeX Gyre Pagella Math, TeX Gyre Schola, TeX Gyre Termes Math, STIX Math, Asana Math, STIXGeneral, DejaVu Serif, DejaVu Sans, serif");
pref("font.name.sans-serif.x-math", "sans-serif");
pref("font.name.monospace.x-math", "monospace");

// Some CJK fonts have bad underline offset, their CJK character glyphs are overlapped (or adjoined)  to its underline.
// These fonts are ignored the underline offset, instead of it, the underline is lowered to bottom of its em descent.
pref("font.blacklist.underline_offset", "FangSong,Gulim,GulimChe,MingLiU,MingLiU-ExtB,MingLiU_HKSCS,MingLiU-HKSCS-ExtB,MS Gothic,MS Mincho,MS PGothic,MS PMincho,MS UI Gothic,PMingLiU,PMingLiU-ExtB,SimHei,SimSun,SimSun-ExtB,Hei,Kai,Apple LiGothic,Apple LiSung,Osaka");

#ifdef MOZ_B2G
// Whitelist of fonts that ship with B2G that do not include space lookups in
// default features. This allows us to skip analyzing the GSUB/GPOS tables
// unless features are explicitly enabled.
// Use NSPR_LOG_MODULES=fontinit:5 to dump out details of space lookups
pref("font.whitelist.skip_default_features_space_check", "Fira Sans,Fira Mono");
#endif

pref("images.dither", "auto");

// ------------------
//  Text Direction
// ------------------
// 1 = directionLTRBidi *
// 2 = directionRTLBidi
pref("bidi.direction", 1);
// ------------------
//  Text Type
// ------------------
// 1 = charsettexttypeBidi *
// 2 = logicaltexttypeBidi
// 3 = visualtexttypeBidi
pref("bidi.texttype", 1);
// ------------------
//  Numeral Style
// ------------------
// 0 = nominalnumeralBidi *
// 1 = regularcontextnumeralBidi
// 2 = hindicontextnumeralBidi
// 3 = arabicnumeralBidi
// 4 = hindinumeralBidi
// 5 = persiancontextnumeralBidi
// 6 = persiannumeralBidi
pref("bidi.numeral", 0);
// ------------------
//  Support Mode
// ------------------
// 1 = mozillaBidisupport *
// 2 = OsBidisupport
// 3 = disableBidisupport
pref("bidi.support", 1);
// Whether delete and backspace should immediately delete characters not
// visually adjacent to the caret, or adjust the visual position of the caret
// on the first keypress and delete the character on a second keypress
pref("bidi.edit.delete_immediately", true);

// Bidi caret movement style:
// 0 = logical
// 1 = visual
// 2 = visual, but logical during selection
pref("bidi.edit.caret_movement_style", 2);

// Setting this pref to |true| forces Bidi UI menu items and keyboard shortcuts
// to be exposed, and enables the directional caret hook. By default, only
// expose it for bidi-associated system locales.
pref("bidi.browser.ui", false);

// Override DPI. A value of -1 means use the maximum of 96 and the system DPI.
// A value of 0 means use the system DPI. A positive value is used as the DPI.
// This sets the physical size of a device pixel and thus controls the
// interpretation of physical units such as "pt".
pref("layout.css.dpi", -1);

// Set the number of device pixels per CSS pixel. A value <= 0 means choose
// automatically based on user settings for the platform (e.g., "UI scale factor"
// on Mac). A positive value is used as-is. This effectively controls the size
// of a CSS "px". This is only used for windows on the screen, not for printing.
pref("layout.css.devPixelsPerPx", "-1.0");


// pref to control browser frame rate, in Hz. A value <= 0 means choose
// automatically based on knowledge of the platform (or 60Hz if no platform-
// specific information is available).
pref("layout.frame_rate", -1);

// pref to control precision of the frame rate timer. When true,
// we use a "precise" timer, which means each notification fires
// Nms after the start of the last notification. That means if the
// processing of the notification is slow, the timer can fire immediately
// after we've just finished processing the last notification, which might
// lead to starvation problems.
// When false, we use a "slack" timer which fires Nms after the *end*
// of the last notification. This can give less tight frame rates
// but provides more time for other operations when the browser is
// heavily loaded.
pref("layout.frame_rate.precise", false);

// Default font types and sizes by locale
pref("font.default.ar", "sans-serif");
pref("font.minimum-size.ar", 0);
pref("font.size.variable.ar", 16);
pref("font.size.fixed.ar", 13);

pref("font.default.el", "serif");
pref("font.minimum-size.el", 0);
pref("font.size.variable.el", 16);
pref("font.size.fixed.el", 13);

pref("font.default.he", "sans-serif");
pref("font.minimum-size.he", 0);
pref("font.size.variable.he", 16);
pref("font.size.fixed.he", 13);

pref("font.default.ja", "sans-serif");
pref("font.minimum-size.ja", 0);
pref("font.size.variable.ja", 16);
pref("font.size.fixed.ja", 16);

pref("font.default.ko", "sans-serif");
pref("font.minimum-size.ko", 0);
pref("font.size.variable.ko", 16);
pref("font.size.fixed.ko", 16);

pref("font.default.th", "sans-serif");
pref("font.minimum-size.th", 0);
pref("font.size.variable.th", 16);
pref("font.size.fixed.th", 13);

pref("font.default.x-cyrillic", "serif");
pref("font.minimum-size.x-cyrillic", 0);
pref("font.size.variable.x-cyrillic", 16);
pref("font.size.fixed.x-cyrillic", 13);

pref("font.default.x-devanagari", "serif");
pref("font.minimum-size.x-devanagari", 0);
pref("font.size.variable.x-devanagari", 16);
pref("font.size.fixed.x-devanagari", 13);

pref("font.default.x-tamil", "serif");
pref("font.minimum-size.x-tamil", 0);
pref("font.size.variable.x-tamil", 16);
pref("font.size.fixed.x-tamil", 13);

pref("font.default.x-armn", "serif");
pref("font.minimum-size.x-armn", 0);
pref("font.size.variable.x-armn", 16);
pref("font.size.fixed.x-armn", 13);

pref("font.default.x-beng", "serif");
pref("font.minimum-size.x-beng", 0);
pref("font.size.variable.x-beng", 16);
pref("font.size.fixed.x-beng", 13);

pref("font.default.x-cans", "serif");
pref("font.minimum-size.x-cans", 0);
pref("font.size.variable.x-cans", 16);
pref("font.size.fixed.x-cans", 13);

pref("font.default.x-ethi", "serif");
pref("font.minimum-size.x-ethi", 0);
pref("font.size.variable.x-ethi", 16);
pref("font.size.fixed.x-ethi", 13);

pref("font.default.x-geor", "serif");
pref("font.minimum-size.x-geor", 0);
pref("font.size.variable.x-geor", 16);
pref("font.size.fixed.x-geor", 13);

pref("font.default.x-gujr", "serif");
pref("font.minimum-size.x-gujr", 0);
pref("font.size.variable.x-gujr", 16);
pref("font.size.fixed.x-gujr", 13);

pref("font.default.x-guru", "serif");
pref("font.minimum-size.x-guru", 0);
pref("font.size.variable.x-guru", 16);
pref("font.size.fixed.x-guru", 13);

pref("font.default.x-khmr", "serif");
pref("font.minimum-size.x-khmr", 0);
pref("font.size.variable.x-khmr", 16);
pref("font.size.fixed.x-khmr", 13);

pref("font.default.x-mlym", "serif");
pref("font.minimum-size.x-mlym", 0);
pref("font.size.variable.x-mlym", 16);
pref("font.size.fixed.x-mlym", 13);

pref("font.default.x-orya", "serif");
pref("font.minimum-size.x-orya", 0);
pref("font.size.variable.x-orya", 16);
pref("font.size.fixed.x-orya", 13);

pref("font.default.x-telu", "serif");
pref("font.minimum-size.x-telu", 0);
pref("font.size.variable.x-telu", 16);
pref("font.size.fixed.x-telu", 13);

pref("font.default.x-knda", "serif");
pref("font.minimum-size.x-knda", 0);
pref("font.size.variable.x-knda", 16);
pref("font.size.fixed.x-knda", 13);

pref("font.default.x-sinh", "serif");
pref("font.minimum-size.x-sinh", 0);
pref("font.size.variable.x-sinh", 16);
pref("font.size.fixed.x-sinh", 13);

pref("font.default.x-tibt", "serif");
pref("font.minimum-size.x-tibt", 0);
pref("font.size.variable.x-tibt", 16);
pref("font.size.fixed.x-tibt", 13);

pref("font.default.x-unicode", "serif");
pref("font.minimum-size.x-unicode", 0);
pref("font.size.variable.x-unicode", 16);
pref("font.size.fixed.x-unicode", 13);

pref("font.default.x-western", "serif");
pref("font.minimum-size.x-western", 0);
pref("font.size.variable.x-western", 16);
pref("font.size.fixed.x-western", 13);

pref("font.default.zh-CN", "sans-serif");
pref("font.minimum-size.zh-CN", 0);
pref("font.size.variable.zh-CN", 16);
pref("font.size.fixed.zh-CN", 16);

pref("font.default.zh-HK", "sans-serif");
pref("font.minimum-size.zh-HK", 0);
pref("font.size.variable.zh-HK", 16);
pref("font.size.fixed.zh-HK", 16);

pref("font.default.zh-TW", "sans-serif");
pref("font.minimum-size.zh-TW", 0);
pref("font.size.variable.zh-TW", 16);
pref("font.size.fixed.zh-TW", 16);

// mathml.css sets font-size to "inherit" and font-family to "serif" so only
// font.name.*.x-math and font.minimum-size.x-math are really relevant.
pref("font.default.x-math", "serif");
pref("font.minimum-size.x-math", 0);
pref("font.size.variable.x-math", 16);
pref("font.size.fixed.x-math", 13);

/*
 * A value greater than zero enables font size inflation for
 * pan-and-zoom UIs, so that the fonts in a block are at least the size
 * that, if a block's width is scaled to match the device's width, the
 * fonts in the block are big enough that at most the pref value ems of
 * text fit in *the width of the device*.
 *
 * When both this pref and the next are set, the larger inflation is
 * used.
 */
pref("font.size.inflation.emPerLine", 0);
/*
 * A value greater than zero enables font size inflation for
 * pan-and-zoom UIs, so that if a block's width is scaled to match the
 * device's width, the fonts in a block are at least the font size
 * given.  The value given is in twips, i.e., 1/20 of a point, or 1/1440
 * of an inch.
 *
 * When both this pref and the previous are set, the larger inflation is
 * used.
 */
pref("font.size.inflation.minTwips", 0);
/*
 * In products with multi-mode pan-and-zoom and non-pan-and-zoom UIs,
 * this pref forces font inflation to always be enabled in all modes.
 * That is, any heuristics used to detect pan-and-zoom
 * vs. non-pan-and-zoom modes are disabled and all content is treated
 * as pan-and-zoom mode wrt font inflation.
 *
 * This pref has no effect if font inflation is not enabled through
 * either of the prefs above.  It has no meaning in single-mode UIs.
 */
pref("font.size.inflation.forceEnabled", false);
/*
 * In products with multi-mode pan-and-zoom and non-pan-and-zoom UIs,
 * this pref disables font inflation in master-process contexts where
 * existing heuristics can't be used determine enabled-ness.
 *
 * This pref has no effect if font inflation is not enabled through
 * either of the prefs above.  The "forceEnabled" pref above overrides
 * this pref.
 */
pref("font.size.inflation.disabledInMasterProcess", false);
/*
 * Since the goal of font size inflation is to avoid having to
 * repeatedly scroll side to side to read a block of text, and there are
 * a number of page layouts where a relatively small chunk of text is
 * better of not being inflated according to the same algorithm we use
 * for larger chunks of text, we want a threshold for an amount of text
 * that triggers font size inflation.  This preference controls that
 * threshold.
 *
 * It controls the threshold used within an *approximation* of the
 * number of lines of text we use.  In particular, if we assume that
 * each character (collapsing collapsible whitespace) has a width the
 * same as the em-size of the font (when, normally, it's actually quite
 * a bit smaller on average), this preference gives the percentage of a
 * number of lines of text we'd need to trigger inflation.  This means
 * that a percentage of 100 means that we'd need a number of characters
 * (we know the font size and the width) equivalent to one line of
 * square text (which is actually a lot less than a real line of text).
 *
 * A value of 0 means there's no character length threshold.
 */
pref("font.size.inflation.lineThreshold", 400);

/*
 * Defines the font size inflation mapping intercept parameter.
 *
 * Font size inflation computes a minimum font size, m, based on
 * other preferences (see font.size.inflation.minTwips and
 * font.size.inflation.emPerLine, above) and the width of the
 * frame in which the text resides. Using this minimum, a specified
 * font size, s, is mapped to an inflated font size, i, using an
 * equation that varies depending on the value of the font size
 * inflation mapping intercept parameter, P:
 *
 * If the intercept parameter is negative, then the following mapping
 * function is used:
 *
 * i = m + s
 *
 * If the intercept parameter is non-negative, then the mapping function
 * is a function such that its graph meets the graph of i = s at the
 * point where both i and s are (1 + P/2) * m for values of s that are
 * large enough. This means that when s=0, i is always equal to m.
 */
pref("font.size.inflation.mappingIntercept", 1);

/*
 * This controls the percentage that fonts will be inflated, if font
 * size inflation is enabled. Essentially, if we have a specified font
 * size, s, and an inflated font size, i, this specifies that the ratio
 * i/s * 100 should never exceed the value of this preference.
 *
 * In order for this preference to have any effect, its value must be
 * greater than 100, since font inflation can never decrease the ratio
 * i/s.
 */
pref("font.size.inflation.maxRatio", 0);

#ifdef XP_WIN

pref("font.name.serif.ar", "Times New Roman");
pref("font.name.sans-serif.ar", "Segoe UI");
pref("font.name-list.sans-serif.ar", "Segoe UI, Tahoma, Arial");
pref("font.name.monospace.ar", "Courier New");
pref("font.name.cursive.ar", "Comic Sans MS");

pref("font.name.serif.el", "Times New Roman");
pref("font.name.sans-serif.el", "Arial");
pref("font.name.monospace.el", "Courier New");
pref("font.name.cursive.el", "Comic Sans MS");

pref("font.name.serif.he", "Narkisim");
pref("font.name.sans-serif.he", "Arial");
pref("font.name.monospace.he", "Fixed Miriam Transparent");
pref("font.name.cursive.he", "Guttman Yad");
pref("font.name-list.serif.he", "Narkisim, David");
pref("font.name-list.monospace.he", "Fixed Miriam Transparent, Miriam Fixed, Rod, Courier New");
pref("font.name-list.cursive.he", "Guttman Yad, Ktav, Arial");

pref("font.name.serif.ja", "MS PMincho");
pref("font.name.sans-serif.ja", "MS PGothic");
pref("font.name.monospace.ja", "MS Gothic");
pref("font.name-list.serif.ja", "MS PMincho, MS Mincho, MS PGothic, MS Gothic,Meiryo");
pref("font.name-list.sans-serif.ja", "MS PGothic, MS Gothic, MS PMincho, MS Mincho,Meiryo");
pref("font.name-list.monospace.ja", "MS Gothic, MS Mincho, MS PGothic, MS PMincho,Meiryo");

pref("font.name.serif.ko", "Batang");
pref("font.name.sans-serif.ko", "Gulim");
pref("font.name.monospace.ko", "GulimChe");
pref("font.name.cursive.ko", "Gungsuh");

pref("font.name-list.serif.ko", "Batang, Gulim");
pref("font.name-list.sans-serif.ko", "Gulim");
pref("font.name-list.monospace.ko", "GulimChe");
pref("font.name-list.cursive.ko", "Gungsuh");

pref("font.name.serif.th", "Tahoma");
pref("font.name.sans-serif.th", "Tahoma");
pref("font.name.monospace.th", "Tahoma");
pref("font.name.cursive.th", "Tahoma");

pref("font.name.serif.x-cyrillic", "Times New Roman");
pref("font.name.sans-serif.x-cyrillic", "Arial");
pref("font.name.monospace.x-cyrillic", "Courier New");
pref("font.name.cursive.x-cyrillic", "Comic Sans MS");

pref("font.name.serif.x-unicode", "Times New Roman");
pref("font.name.sans-serif.x-unicode", "Arial");
pref("font.name.monospace.x-unicode", "Courier New");
pref("font.name.cursive.x-unicode", "Comic Sans MS");

pref("font.name.serif.x-western", "Times New Roman");
pref("font.name.sans-serif.x-western", "Arial");
pref("font.name.monospace.x-western", "Courier New");
pref("font.name.cursive.x-western", "Comic Sans MS");

pref("font.name.serif.zh-CN", "SimSun");
pref("font.name.sans-serif.zh-CN", "Microsoft YaHei");
pref("font.name.monospace.zh-CN", "SimSun");
pref("font.name.cursive.zh-CN", "KaiTi");
pref("font.name-list.serif.zh-CN", "MS Song, SimSun, SimSun-ExtB");
pref("font.name-list.sans-serif.zh-CN", "Microsoft YaHei, SimHei");
pref("font.name-list.monospace.zh-CN", "MS Song, SimSun, SimSun-ExtB");
pref("font.name-list.cursive.zh-CN", "KaiTi, KaiTi_GB2312");

// Per Taiwanese users' demand. They don't want to use TC fonts for
// rendering Latin letters. (bug 88579)
pref("font.name.serif.zh-TW", "Times New Roman");
pref("font.name.sans-serif.zh-TW", "Arial");
pref("font.name.monospace.zh-TW", "MingLiU");
pref("font.name.cursive.zh-TW", "DFKai-SB");
pref("font.name-list.serif.zh-TW", "PMingLiu, MingLiU, MingLiU-ExtB");
pref("font.name-list.sans-serif.zh-TW", "PMingLiU, MingLiU, MingLiU-ExtB");
pref("font.name-list.monospace.zh-TW", "MingLiU, MingLiU-ExtB");

// hkscsm3u.ttf (HKSCS-2001) :  http://www.microsoft.com/hk/hkscs
// Hong Kong users have the same demand about glyphs for Latin letters (bug 88579)
pref("font.name.serif.zh-HK", "Times New Roman");
pref("font.name.sans-serif.zh-HK", "Arial");
pref("font.name.monospace.zh-HK", "MingLiu_HKSCS");
pref("font.name.cursive.zh-HK", "DFKai-SB");
pref("font.name-list.serif.zh-HK", "MingLiu_HKSCS, Ming(for ISO10646), MingLiU, MingLiU_HKSCS-ExtB");
pref("font.name-list.sans-serif.zh-HK", "MingLiU_HKSCS, Ming(for ISO10646), MingLiU, MingLiU_HKSCS-ExtB");
pref("font.name-list.monospace.zh-HK", "MingLiU_HKSCS, Ming(for ISO10646), MingLiU, MingLiU_HKSCS-ExtB");

pref("font.name.serif.x-devanagari", "Kokila");
pref("font.name.sans-serif.x-devanagari", "Nirmala UI");
pref("font.name.monospace.x-devanagari", "Mangal");
pref("font.name-list.serif.x-devanagari", "Kokila, Raghindi");
pref("font.name-list.sans-serif.x-devanagari", "Nirmala UI, Mangal");
pref("font.name-list.monospace.x-devanagari", "Mangal, Nirmala UI");

pref("font.name.serif.x-tamil", "Latha");
pref("font.name.sans-serif.x-tamil", "");
pref("font.name.monospace.x-tamil", "Latha");
pref("font.name-list.serif.x-tamil", "Latha");
pref("font.name-list.monospace.x-tamil", "Latha");

pref("font.name.serif.x-armn", "Sylfaen");
pref("font.name.sans-serif.x-armn", "Arial AMU");
pref("font.name.monospace.x-armn", "Arial AMU");
pref("font.name-list.serif.x-armn", "Sylfaen");
pref("font.name-list.monospace.x-armn", "Arial AMU");

pref("font.name.serif.x-beng", "Vrinda");
pref("font.name.sans-serif.x-beng", "Vrinda");
pref("font.name.monospace.x-beng", "Mitra Mono");
pref("font.name-list.serif.x-beng", "Vrinda, Akaash, Likhan, Ekushey Punarbhaba");
pref("font.name-list.sans-serif.x-beng", "Vrinda, Akaash, Likhan, Ekushey Punarbhaba");
pref("font.name-list.monospace.x-beng", "Likhan, Mukti Narrow");

pref("font.name.serif.x-cans", "Aboriginal Serif");
pref("font.name.sans-serif.x-cans", "Aboriginal Sans");
pref("font.name.monospace.x-cans", "Aboriginal Sans");
pref("font.name-list.serif.x-cans", "Aboriginal Serif, BJCree Uni");
pref("font.name-list.monospace.x-cans", "Aboriginal Sans, OskiDakelh, Pigiarniq, Uqammaq");

pref("font.name.serif.x-ethi", "Visual Geez Unicode");
pref("font.name.sans-serif.x-ethi", "GF Zemen Unicode");
pref("font.name.cursive.x-ethi", "Visual Geez Unicode Title");
pref("font.name.monospace.x-ethi", "Ethiopia Jiret");
pref("font.name-list.serif.x-ethi", "Visual Geez Unicode, Visual Geez Unicode Agazian");
pref("font.name-list.monospace.x-ethi", "Ethiopia Jiret");

pref("font.name.serif.x-geor", "Sylfaen");
pref("font.name.sans-serif.x-geor", "BPG Classic 99U");
pref("font.name.monospace.x-geor", "BPG Classic 99U");
pref("font.name-list.serif.x-geor", "Sylfaen, BPG Paata Khutsuri U, TITUS Cyberbit Basic");
pref("font.name-list.monospace.x-geor", "BPG Classic 99U");

pref("font.name.serif.x-gujr", "Shruti");
pref("font.name.sans-serif.x-gujr", "Shruti");
pref("font.name.monospace.x-gujr", "Shruti");
pref("font.name-list.serif.x-gujr", "Shruti");
pref("font.name-list.monospace.x-gujr", "Shruti");

pref("font.name.serif.x-guru", "Raavi");
pref("font.name.sans-serif.x-guru", "");
pref("font.name.monospace.x-guru", "Raavi");
pref("font.name-list.serif.x-guru", "Raavi, Saab");
pref("font.name-list.monospace.x-guru", "Raavi, Saab");

pref("font.name.serif.x-khmr", "PhnomPenh OT");
pref("font.name.sans-serif.x-khmr", "Khmer OS");
pref("font.name.monospace.x-khmr", "Khmer OS");
pref("font.name-list.serif.x-khmr", "PhnomPenh OT,.Mondulkiri U GR 1.5, Khmer OS");
pref("font.name-list.monospace.x-khmr", "Khmer OS, Khmer OS System");

pref("font.name.serif.x-mlym", "Rachana_w01");
pref("font.name.sans-serif.x-mlym", "Rachana_w01");
pref("font.name.monospace.x-mlym", "Rachana_w01");
pref("font.name-list.serif.x-mlym", "AnjaliOldLipi, Kartika, ThoolikaUnicode");
pref("font.name-list.sans-serif.x-mlym", "AnjaliOldLipi, Kartika, ThoolikaUnicode");
pref("font.name-list.monospace.x-mlym", "AnjaliOldLipi, Kartika, ThoolikaUnicode");

pref("font.name.serif.x-orya", "ori1Uni");
pref("font.name.sans-serif.x-orya", "ori1Uni");
pref("font.name.monospace.x-orya", "ori1Uni");
pref("font.name-list.serif.x-orya", "Kalinga, ori1Uni");
pref("font.name-list.sans-serif.x-orya", "Kalinga, ori1Uni");
pref("font.name-list.monospace.x-orya", "Kalinga, ori1Uni");

pref("font.name.serif.x-telu", "Gautami");
pref("font.name.sans-serif.x-telu", "Gautami");
pref("font.name.monospace.x-telu", "Gautami");
pref("font.name-list.serif.x-telu", "Gautami, Akshar Unicode");
pref("font.name-list.sans-serif.x-telu", "Gautami, Akshar Unicode");
pref("font.name-list.monospace.x-telu", "Gautami, Akshar Unicode");

pref("font.name.serif.x-knda", "Tunga");
pref("font.name.sans-serif.x-knda", "Tunga");
pref("font.name.monospace.x-knda", "Tunga");
pref("font.name-list.serif.x-knda", "Tunga, AksharUnicode");
pref("font.name-list.sans-serif.x-knda", "Tunga, AksharUnicode");
pref("font.name-list.monospace.x-knda", "Tunga, AksharUnicode");

pref("font.name.serif.x-sinh", "Iskoola Pota");
pref("font.name.sans-serif.x-sinh", "Iskoola Pota");
pref("font.name.monospace.x-sinh", "Iskoola Pota");
pref("font.name-list.serif.x-sinh", "Iskoola Pota, AksharUnicode");
pref("font.name-list.sans-serif.x-sinh", "Iskoola Pota, AksharUnicode");
pref("font.name-list.monospace.x-sinh", "Iskoola Pota, AksharUnicode");

pref("font.name.serif.x-tibt", "Tibetan Machine Uni");
pref("font.name.sans-serif.x-tibt", "Tibetan Machine Uni");
pref("font.name.monospace.x-tibt", "Tibetan Machine Uni");
pref("font.name-list.serif.x-tibt", "Tibetan Machine Uni, Jomolhari, Microsoft Himalaya");
pref("font.name-list.sans-serif.x-tibt", "Tibetan Machine Uni, Jomolhari, Microsoft Himalaya");
pref("font.name-list.monospace.x-tibt", "Tibetan Machine Uni, Jomolhari, Microsoft Himalaya");

pref("font.minimum-size.th", 10);

pref("font.default.x-devanagari", "sans-serif");
pref("font.name.serif.x-math", "Latin Modern Math");
// We have special support for Monotype Symbol on Windows.
pref("font.name-list.serif.x-math", "Latin Modern Math, XITS Math, Cambria Math, Libertinus Math, DejaVu Math TeX Gyre, TeX Gyre Bonum Math, TeX Gyre Pagella Math, TeX Gyre Schola, TeX Gyre Termes Math, STIX Math, Asana Math, STIXGeneral, DejaVu Serif, DejaVu Sans, Symbol, Times New Roman");
pref("font.name.sans-serif.x-math", "Arial");
pref("font.name.monospace.x-math", "Courier New");
pref("font.name.cursive.x-math", "Comic Sans MS");

// cleartype settings - false implies default system settings

// use cleartype rendering for downloadable fonts (win xp only)
pref("gfx.font_rendering.cleartype.use_for_downloadable_fonts", true);

// use cleartype rendering for all fonts always (win xp only)
pref("gfx.font_rendering.cleartype.always_use_for_content", false);

// ClearType tuning parameters for directwrite/d2d.
//
// Allows overriding of underlying registry values in:
//   HKCU/Software/Microsoft/Avalon.Graphics/<display> (contrast and level)
//   HKLM/Software/Microsoft/Avalon.Graphics/<display> (gamma, pixel structure)
// and selection of the ClearType/antialiasing mode.
//
// A value of -1 implies use the default value, otherwise value ranges
// follow registry settings:
//   gamma [1000, 2200]  default: based on screen, typically 2200 (== 2.2)
//   enhanced contrast [0, 1000] default: 50
//   cleartype level [0, 100] default: 100
//   pixel structure [0, 2] default: 0 (flat/RGB/BGR)
//   rendering mode [0, 5] default: 0
//     0 = use default for font & size;
//     1 = aliased;
//     2 = GDI Classic;
//     3 = GDI Natural Widths;
//     4 = Natural;
//     5 = Natural Symmetric
//
// See:
//   http://msdn.microsoft.com/en-us/library/aa970267.aspx
//   http://msdn.microsoft.com/en-us/library/dd368190%28v=VS.85%29.aspx
// Note: DirectWrite uses the "Enhanced Contrast Level" value rather than the
// "Text Contrast Level" value

pref("gfx.font_rendering.cleartype_params.gamma", -1);
pref("gfx.font_rendering.cleartype_params.enhanced_contrast", -1);
pref("gfx.font_rendering.cleartype_params.cleartype_level", -1);
pref("gfx.font_rendering.cleartype_params.pixel_structure", -1);
pref("gfx.font_rendering.cleartype_params.rendering_mode", -1);

// A comma-separated list of font family names. Fonts in these families will
// be forced to use "GDI Classic" ClearType mode, provided the value
// of gfx.font_rendering.cleartype_params.rendering_mode is -1
// (i.e. a specific rendering_mode has not been explicitly set).
// Currently we apply this setting to the sans-serif Microsoft "core Web fonts".
pref("gfx.font_rendering.cleartype_params.force_gdi_classic_for_families",
     "Arial,Consolas,Courier New,Microsoft Sans Serif,Segoe UI,Tahoma,Trebuchet MS,Verdana");
// The maximum size at which we will force GDI classic mode using
// force_gdi_classic_for_families.
pref("gfx.font_rendering.cleartype_params.force_gdi_classic_max_size", 15);

pref("ui.key.menuAccessKeyFocuses", true);

// override double-click word selection behavior.
pref("layout.word_select.eat_space_to_next_word", true);

// scrollbar snapping region
pref("slider.snapMultiplier", 6);

// print_extra_margin enables platforms to specify an extra gap or margin
// around the content of the page for Print Preview only
pref("print.print_extra_margin", 90); // twips (90 twips is an eigth of an inch)

// Whether to extend the native dialog with information on printing frames.
pref("print.extend_native_print_dialog", true);

// Locate plugins by scanning the Adobe Acrobat installation directory with a minimum version
pref("plugin.scan.Acrobat", "5.0");

// Locate plugins by scanning the Quicktime installation directory with a minimum version
pref("plugin.scan.Quicktime", "5.0");

// Locate and scan the Window Media Player installation directory for plugins with a minimum version
pref("plugin.scan.WindowsMediaPlayer", "7.0");

// Locate plugins by the directories specified in the Windows registry for PLIDs
// Which is currently HKLM\Software\MozillaPlugins\xxxPLIDxxx\Path
pref("plugin.scan.plid.all", true);

// Whether sending WM_MOUSEWHEEL and WM_MOUSEHWHEEL to plugins on Windows.
pref("plugin.mousewheel.enabled", true);

// Switch the keyboard layout per window
pref("intl.keyboard.per_window_layout", false);

#ifdef NS_ENABLE_TSF
// Enable/Disable TSF support on Vista or later.
pref("intl.tsf.enable", true);

// Support IMEs implemented with IMM in TSF mode.
pref("intl.tsf.support_imm", true);

// Enables/Disables hack for specific TIP.

// Whether creates native caret for ATOK or not.
pref("intl.tsf.hack.atok.create_native_caret", true);
// Whether use composition start position for the result of
// ITfContextView::GetTextExt() if the specified range is larger than
// composition start offset.
// For Free ChangJie 2010
pref("intl.tsf.hack.free_chang_jie.do_not_return_no_layout_error", true);
// For Microsoft Pinyin and Microsoft Wubi
pref("intl.tsf.hack.ms_simplified_chinese.do_not_return_no_layout_error", true);
// For Microsoft ChangJie and Microsoft Quick
pref("intl.tsf.hack.ms_traditional_chinese.do_not_return_no_layout_error", true);
// For Easy Changjei
pref("intl.tsf.hack.easy_changjei.do_not_return_no_layout_error", true);
// Whether use previous character rect for the result of
// ITfContextView::GetTextExt() if the specified range is the first character
// of selected clause of composition string.
pref("intl.tsf.hack.ms_japanese_ime.do_not_return_no_layout_error_at_first_char", true);
// Whether use previous character rect for the result of
// ITfContextView::GetTextExt() if the specified range is the caret of
// composition string.
pref("intl.tsf.hack.ms_japanese_ime.do_not_return_no_layout_error_at_caret", true);
// Whether hack ITextStoreACP::QueryInsert() or not.  The method should return
// new selection after specified length text is inserted at specified range.
// However, Microsoft's some Chinese TIPs expect that the result is same as
// specified range.  If following prefs are true, ITextStoreACP::QueryInsert()
// returns specified range only when one of the TIPs is active.
// For Microsoft Pinyin and Microsoft Wubi
pref("intl.tsf.hack.ms_simplified_chinese.query_insert_result", true);
// For Microsoft ChangJie and Microsoft Quick
pref("intl.tsf.hack.ms_traditional_chinese.query_insert_result", true);
#endif

// If composition_font is set, Gecko sets the font to IME.  IME may use
// the fonts on their window like candidate window.  If they are empty,
// Gecko uses the system default font which is set to the IM context.
// The font name must not start with '@'.  When the writing mode is vertical,
// Gecko inserts '@' to the start of the font name automatically.
// FYI: Changing these prefs requires to restart.
pref("intl.imm.composition_font", "");

// Japanist 2003's candidate window is broken if the font is "@System" which
// is default composition font for vertical writing mode.
// You can specify font to use on candidate window of Japanist 2003.
// This value must not start with '@'.
// FYI: Changing this pref requires to restart.
pref("intl.imm.composition_font.japanist_2003", "MS PGothic");

// Even if IME claims that they support vertical writing mode but it may not
// support vertical writing mode for its candidate window.  This pref allows
// to ignore the claim.
// FYI: Changing this pref requires to restart.
pref("intl.imm.vertical_writing.always_assume_not_supported", false);

// We cannot retrieve active IME name with IMM32 API if a TIP of TSF is active.
// This pref can specify active IME name when Japanese TIP is active.
// For example:
//   Google Japanese Input: "Google 日本語入力 IMM32 モジュール"
//   ATOK 2011: "ATOK 2011" (similarly, e.g., ATOK 2013 is "ATOK 2013")
pref("intl.imm.japanese.assume_active_tip_name_as", "");

#endif

#ifdef XP_MACOSX

// default fonts (in UTF8 and using canonical names)
// to determine canonical font names, use a debug build and
// enable NSPR logging for module fontInfoLog:5
// canonical names immediately follow '(fontinit) family:' in the log

pref("font.name.serif.ar", "Al Bayan");
pref("font.name.sans-serif.ar", "Geeza Pro");
pref("font.name.monospace.ar", "Geeza Pro");
pref("font.name.cursive.ar", "DecoType Naskh");
pref("font.name.fantasy.ar", "KufiStandardGK");
pref("font.name-list.serif.ar", "Al Bayan");
pref("font.name-list.sans-serif.ar", "Geeza Pro");
pref("font.name-list.monospace.ar", "Geeza Pro");
pref("font.name-list.cursive.ar", "DecoType Naskh");
pref("font.name-list.fantasy.ar", "KufiStandardGK");

pref("font.name.serif.el", "Times");
pref("font.name.sans-serif.el", "Helvetica");
pref("font.name.monospace.el", "Courier New");
pref("font.name.cursive.el", "Lucida Grande");
pref("font.name.fantasy.el", "Lucida Grande");
pref("font.name-list.serif.el", "Times,Times New Roman");
pref("font.name-list.sans-serif.el", "Helvetica,Lucida Grande");
pref("font.name-list.monospace.el", "Courier New,Lucida Grande");
pref("font.name-list.cursive.el", "Times,Lucida Grande");
pref("font.name-list.fantasy.el", "Times,Lucida Grande");

pref("font.name.serif.he", "Times New Roman");
pref("font.name.sans-serif.he", "Arial");
pref("font.name.monospace.he", "Courier New");
pref("font.name.cursive.he", "Times New Roman");
pref("font.name.fantasy.he", "Times New Roman");
pref("font.name-list.serif.he", "Times New Roman");
pref("font.name-list.sans-serif.he", "Arial");
pref("font.name-list.monospace.he", "Courier New");
pref("font.name-list.cursive.he", "Times New Roman");
pref("font.name-list.fantasy.he", "Times New Roman");

pref("font.name.serif.ja", "Hiragino Mincho ProN");
pref("font.name.sans-serif.ja", "Hiragino Kaku Gothic ProN");
pref("font.name.monospace.ja", "Osaka-Mono");
pref("font.name-list.serif.ja", "Hiragino Mincho ProN,Hiragino Mincho Pro");
pref("font.name-list.sans-serif.ja", "Hiragino Kaku Gothic ProN,Hiragino Kaku Gothic Pro");
pref("font.name-list.monospace.ja", "Osaka-Mono");

pref("font.name.serif.ko", "AppleMyungjo");
pref("font.name.sans-serif.ko", "Apple SD Gothic Neo");
pref("font.name.monospace.ko", "Apple SD Gothic Neo");
pref("font.name-list.serif.ko", "AppleMyungjo");
pref("font.name-list.sans-serif.ko", "Apple SD Gothic Neo,AppleGothic");
pref("font.name-list.monospace.ko", "Apple SD Gothic Neo,AppleGothic");

pref("font.name.serif.th", "Thonburi");
pref("font.name.sans-serif.th", "Thonburi");
pref("font.name.monospace.th", "Ayuthaya");
pref("font.name-list.serif.th", "Thonburi");
pref("font.name-list.sans-serif.th", "Thonburi");
pref("font.name-list.monospace.th", "Ayuthaya");

pref("font.name.serif.x-armn", "Mshtakan");
pref("font.name.sans-serif.x-armn", "Mshtakan");
pref("font.name.monospace.x-armn", "Mshtakan");
pref("font.name-list.serif.x-armn", "Mshtakan");
pref("font.name-list.sans-serif.x-armn", "Mshtakan");
pref("font.name-list.monospace.x-armn", "Mshtakan");

// SolaimanLipi, Rupali http://ekushey.org/?page/mac_download
pref("font.name.serif.x-beng", "Bangla MN");
pref("font.name.sans-serif.x-beng", "Bangla Sangam MN");
pref("font.name.monospace.x-beng", "Bangla Sangam MN");
pref("font.name-list.serif.x-beng", "Bangla MN");
pref("font.name-list.sans-serif.x-beng", "Bangla Sangam MN");
pref("font.name-list.monospace.x-beng", "Bangla Sangam MN");

pref("font.name.serif.x-cans", "Euphemia UCAS");
pref("font.name.sans-serif.x-cans", "Euphemia UCAS");
pref("font.name.monospace.x-cans", "Euphemia UCAS");
pref("font.name-list.serif.x-cans", "Euphemia UCAS");
pref("font.name-list.sans-serif.x-cans", "Euphemia UCAS");
pref("font.name-list.monospace.x-cans", "Euphemia UCAS");

pref("font.name.serif.x-cyrillic", "Times");
pref("font.name.sans-serif.x-cyrillic", "Helvetica");
pref("font.name.monospace.x-cyrillic", "Monaco");
pref("font.name.cursive.x-cyrillic", "Geneva");
pref("font.name.fantasy.x-cyrillic", "Charcoal CY");
pref("font.name-list.serif.x-cyrillic", "Times,Times New Roman");
pref("font.name-list.sans-serif.x-cyrillic", "Helvetica,Arial");
pref("font.name-list.monospace.x-cyrillic", "Monaco,Courier New");
pref("font.name-list.cursive.x-cyrillic", "Geneva");
pref("font.name-list.fantasy.x-cyrillic", "Charcoal CY");

pref("font.name.serif.x-devanagari", "Devanagari MT");
pref("font.name.sans-serif.x-devanagari", "Devanagari Sangam MN");
pref("font.name.monospace.x-devanagari", "Devanagari Sangam MN");
pref("font.name-list.serif.x-devanagari", "Devanagari MT");
pref("font.name-list.sans-serif.x-devanagari", "Devanagari Sangam MN,Devanagari MT");
pref("font.name-list.monospace.x-devanagari", "Devanagari Sangam MN,Devanagari MT");

// Abyssinica SIL http://scripts.sil.org/AbyssinicaSIL_Download
pref("font.name.serif.x-ethi", "Kefa");
pref("font.name.sans-serif.x-ethi", "Kefa");
pref("font.name.monospace.x-ethi", "Kefa");
pref("font.name-list.serif.x-ethi", "Kefa,Abyssinica SIL");
pref("font.name-list.sans-serif.x-ethi", "Kefa,Abyssinica SIL");
pref("font.name-list.monospace.x-ethi", "Kefa,Abyssinica SIL");

// no suitable fonts for georgian ship with mac os x
// however some can be freely downloaded
// TITUS Cyberbit Basic http://titus.fkidg1.uni-frankfurt.de/unicode/tituut.asp
// Zuzumbo http://homepage.mac.com/rsiradze/FileSharing91.html
pref("font.name.serif.x-geor", "TITUS Cyberbit Basic");
pref("font.name.sans-serif.x-geor", "Zuzumbo");
pref("font.name.monospace.x-geor", "Zuzumbo");
pref("font.name-list.serif.x-geor", "TITUS Cyberbit Basic");
pref("font.name-list.sans-serif.x-geor", "Zuzumbo");
pref("font.name-list.monospace.x-geor", "Zuzumbo");

pref("font.name.serif.x-gujr", "Gujarati MT");
pref("font.name.sans-serif.x-gujr", "Gujarati Sangam MN");
pref("font.name.monospace.x-gujr", "Gujarati Sangam MN");
pref("font.name-list.serif.x-gujr", "Gujarati MT");
pref("font.name-list.sans-serif.x-gujr", "Gujarati Sangam MN,Gujarati MT");
pref("font.name-list.monospace.x-gujr", "Gujarati Sangam MN,Gujarati MT");

pref("font.name.serif.x-guru", "Gurmukhi MT");
pref("font.name.sans-serif.x-guru", "Gurmukhi MT");
pref("font.name.monospace.x-guru", "Gurmukhi MT");
pref("font.name-list.serif.x-guru", "Gurmukhi MT");
pref("font.name-list.sans-serif.x-guru", "Gurmukhi MT");
pref("font.name-list.monospace.x-guru", "Gurmukhi MT");

pref("font.name.serif.x-khmr", "Khmer MN");
pref("font.name.sans-serif.x-khmr", "Khmer Sangam MN");
pref("font.name.monospace.x-khmr", "Khmer Sangam MN");
pref("font.name-list.serif.x-khmr", "Khmer MN");
pref("font.name-list.sans-serif.x-khmr", "Khmer Sangam MN");
pref("font.name-list.monospace.x-khmr", "Khmer Sangam MN");

pref("font.name.serif.x-mlym", "Malayalam MN");
pref("font.name.sans-serif.x-mlym", "Malayalam Sangam MN");
pref("font.name.monospace.x-mlym", "Malayalam Sangam MN");
pref("font.name-list.serif.x-mlym", "Malayalam MN");
pref("font.name-list.sans-serif.x-mlym", "Malayalam Sangam MN");
pref("font.name-list.monospace.x-mlym", "Malayalam Sangam MN");

pref("font.name.serif.x-orya", "Oriya MN");
pref("font.name.sans-serif.x-orya", "Oriya Sangam MN");
pref("font.name.monospace.x-orya", "Oriya Sangam MN");
pref("font.name-list.serif.x-orya", "Oriya MN");
pref("font.name-list.sans-serif.x-orya", "Oriya Sangam MN");
pref("font.name-list.monospace.x-orya", "Oriya Sangam MN");

// Pothana http://web.nickshanks.com/typography/telugu/
pref("font.name.serif.x-telu", "Telugu MN");
pref("font.name.sans-serif.x-telu", "Telugu Sangam MN");
pref("font.name.monospace.x-telu", "Telugu Sangam MN");
pref("font.name-list.serif.x-telu", "Telugu MN,Pothana");
pref("font.name-list.sans-serif.x-telu", "Telugu Sangam MN,Pothana");
pref("font.name-list.monospace.x-telu", "Telugu Sangam MN,Pothana");

// Kedage http://web.nickshanks.com/typography/kannada/
pref("font.name.serif.x-knda", "Kannada MN");
pref("font.name.sans-serif.x-knda", "Kannada Sangam MN");
pref("font.name.monospace.x-knda", "Kannada Sangam MN");
pref("font.name-list.serif.x-knda", "Kannada MN,Kedage");
pref("font.name-list.sans-serif.x-knda", "Kannada Sangam MN,Kedage");
pref("font.name-list.monospace.x-knda", "Kannada Sangam MN,Kedage");

pref("font.name.serif.x-sinh", "Sinhala MN");
pref("font.name.sans-serif.x-sinh", "Sinhala Sangam MN");
pref("font.name.monospace.x-sinh", "Sinhala Sangam MN");
pref("font.name-list.serif.x-sinh", "Sinhala MN");
pref("font.name-list.sans-serif.x-sinh", "Sinhala Sangam MN");
pref("font.name-list.monospace.x-sinh", "Sinhala Sangam MN");

pref("font.name.serif.x-tamil", "InaiMathi");
pref("font.name.sans-serif.x-tamil", "InaiMathi");
pref("font.name.monospace.x-tamil", "InaiMathi");
pref("font.name-list.serif.x-tamil", "InaiMathi");
pref("font.name-list.sans-serif.x-tamil", "InaiMathi");
pref("font.name-list.monospace.x-tamil", "InaiMathi");

// Kailasa ships with mac os x >= 10.5
pref("font.name.serif.x-tibt", "Kailasa");
pref("font.name.sans-serif.x-tibt", "Kailasa");
pref("font.name.monospace.x-tibt", "Kailasa");
pref("font.name-list.serif.x-tibt", "Kailasa");
pref("font.name-list.sans-serif.x-tibt", "Kailasa");
pref("font.name-list.monospace.x-tibt", "Kailasa");

pref("font.name.serif.x-unicode", "Times");
pref("font.name.sans-serif.x-unicode", "Helvetica");
pref("font.name.monospace.x-unicode", "Courier");
pref("font.name.cursive.x-unicode", "Apple Chancery");
pref("font.name.fantasy.x-unicode", "Papyrus");
pref("font.name-list.serif.x-unicode", "Times");
pref("font.name-list.sans-serif.x-unicode", "Helvetica");
pref("font.name-list.monospace.x-unicode", "Courier");
pref("font.name-list.cursive.x-unicode", "Apple Chancery");
pref("font.name-list.fantasy.x-unicode", "Papyrus");

pref("font.name.serif.x-western", "Times");
pref("font.name.sans-serif.x-western", "Helvetica");
pref("font.name.monospace.x-western", "Courier");
pref("font.name.cursive.x-western", "Apple Chancery");
pref("font.name.fantasy.x-western", "Papyrus");
pref("font.name-list.serif.x-western", "Times,Times New Roman");
pref("font.name-list.sans-serif.x-western", "Helvetica,Arial");
pref("font.name-list.monospace.x-western", "Courier,Courier New");
pref("font.name-list.cursive.x-western", "Apple Chancery");
pref("font.name-list.fantasy.x-western", "Papyrus");

pref("font.name.serif.zh-CN", "Times");
pref("font.name.sans-serif.zh-CN", "Helvetica");
pref("font.name.monospace.zh-CN", "Courier");
pref("font.name.cursive.zh-CN", "Kaiti SC");
pref("font.name-list.serif.zh-CN", "Times,STSong,Heiti SC");
pref("font.name-list.sans-serif.zh-CN", "Helvetica,PingFang SC,STHeiti,Heiti SC");
pref("font.name-list.monospace.zh-CN", "Courier,PingFang SC,STHeiti,Heiti SC");

pref("font.name.serif.zh-TW", "Times");
pref("font.name.sans-serif.zh-TW", "Helvetica");
pref("font.name.monospace.zh-TW", "Courier");
pref("font.name.cursive.zh-TW", "Kaiti TC");
pref("font.name-list.serif.zh-TW", "Times,LiSong Pro,Heiti TC");
pref("font.name-list.sans-serif.zh-TW", "Helvetica,PingFang TC,Heiti TC,LiHei Pro");
pref("font.name-list.monospace.zh-TW", "Courier,PingFang TC,Heiti TC,LiHei Pro");

pref("font.name.serif.zh-HK", "Times");
pref("font.name.sans-serif.zh-HK", "Helvetica");
pref("font.name.monospace.zh-HK", "Courier");
pref("font.name.cursive.zh-HK", "Kaiti TC");
pref("font.name-list.serif.zh-HK", "Times,LiSong Pro,Heiti TC");
pref("font.name-list.sans-serif.zh-HK", "Helvetica,PingFang TC,Heiti TC,LiHei Pro");
pref("font.name-list.monospace.zh-HK", "Courier,PingFang TC,Heiti TC,LiHei Pro");

// XP_MACOSX changes to default font sizes
pref("font.minimum-size.th", 10);
pref("font.size.variable.zh-CN", 15);
pref("font.size.variable.zh-HK", 15);
pref("font.size.variable.zh-TW", 15);

pref("font.name.serif.x-math", "Latin Modern Math");
// Apple's Symbol is Unicode so use it
pref("font.name-list.serif.x-math", "Latin Modern Math, XITS Math, Cambria Math, Libertinus Math, DejaVu Math TeX Gyre, TeX Gyre Bonum Math, TeX Gyre Pagella Math, TeX Gyre Schola, TeX Gyre Termes Math, STIX Math, Asana Math, STIXGeneral, DejaVu Serif, DejaVu Sans, Symbol, Times");
pref("font.name.sans-serif.x-math", "Helvetica");
pref("font.name.monospace.x-math", "Courier");
pref("font.name.cursive.x-math", "Apple Chancery");
pref("font.name.fantasy.x-math", "Papyrus");

// Individual font faces to be treated as independent families,
// listed as <Postscript name of face:Owning family name>
pref("font.single-face-list", "Osaka-Mono:Osaka");

// optimization hint for fonts with localized names to be read in at startup, otherwise read in at lookup miss
// names are canonical family names (typically English names)
pref("font.preload-names-list", "Hiragino Kaku Gothic ProN,Hiragino Mincho ProN,STSong");

// Override font-weight values for some problematic families Apple ships
// (see bug 931426).
// The name here is the font's PostScript name, which can be checked in
// the Font Book utility or other tools.
pref("font.weight-override.AppleSDGothicNeo-Thin", 100); // Ensure Thin < UltraLight < Light
pref("font.weight-override.AppleSDGothicNeo-UltraLight", 200);
pref("font.weight-override.AppleSDGothicNeo-Light", 300);
pref("font.weight-override.AppleSDGothicNeo-Heavy", 900); // Ensure Heavy > ExtraBold (800)

pref("font.weight-override.Avenir-Book", 300); // Ensure Book < Roman (400)
pref("font.weight-override.Avenir-BookOblique", 300);
pref("font.weight-override.Avenir-MediumOblique", 500); // Harmonize MediumOblique with Medium
pref("font.weight-override.Avenir-Black", 900); // Ensure Black > Heavy (800)
pref("font.weight-override.Avenir-BlackOblique", 900);

pref("font.weight-override.AvenirNext-MediumItalic", 500); // Harmonize MediumItalic with Medium
pref("font.weight-override.AvenirNextCondensed-MediumItalic", 500);

pref("font.weight-override.HelveticaNeue-Light", 300); // Ensure Light > Thin (200)
pref("font.weight-override.HelveticaNeue-LightItalic", 300);
pref("font.weight-override.HelveticaNeue-MediumItalic", 500); // Harmonize MediumItalic with Medium

#endif // XP_MACOSX

#ifndef ANDROID
#ifndef XP_MACOSX
#ifdef XP_UNIX

// font names

pref("font.name.serif.ar", "serif");
pref("font.name.sans-serif.ar", "sans-serif");
pref("font.name.monospace.ar", "monospace");
pref("font.size.fixed.ar", 12);

pref("font.name.serif.el", "serif");
pref("font.name.sans-serif.el", "sans-serif");
pref("font.name.monospace.el", "monospace");
pref("font.size.fixed.el", 12);

pref("font.name.serif.he", "serif");
pref("font.name.sans-serif.he", "sans-serif");
pref("font.name.monospace.he", "monospace");
pref("font.size.fixed.he", 12);

pref("font.name.serif.ja", "serif");
pref("font.name.sans-serif.ja", "sans-serif");
pref("font.name.monospace.ja", "monospace");

pref("font.name.serif.ko", "serif");
pref("font.name.sans-serif.ko", "sans-serif");
pref("font.name.monospace.ko", "monospace");

pref("font.name.serif.th", "serif");
pref("font.name.sans-serif.th", "sans-serif");
pref("font.name.monospace.th", "monospace");
pref("font.minimum-size.th", 13);

pref("font.name.serif.x-armn", "serif");
pref("font.name.sans-serif.x-armn", "sans-serif");
pref("font.name.monospace.x-armn", "monospace");

pref("font.name.serif.x-beng", "serif");
pref("font.name.sans-serif.x-beng", "sans-serif");
pref("font.name.monospace.x-beng", "monospace");

pref("font.name.serif.x-cans", "serif");
pref("font.name.sans-serif.x-cans", "sans-serif");
pref("font.name.monospace.x-cans", "monospace");

pref("font.name.serif.x-cyrillic", "serif");
pref("font.name.sans-serif.x-cyrillic", "sans-serif");
pref("font.name.monospace.x-cyrillic", "monospace");
pref("font.size.fixed.x-cyrillic", 12);

pref("font.name.serif.x-devanagari", "serif");
pref("font.name.sans-serif.x-devanagari", "sans-serif");
pref("font.name.monospace.x-devanagari", "monospace");

pref("font.name.serif.x-ethi", "serif");
pref("font.name.sans-serif.x-ethi", "sans-serif");
pref("font.name.monospace.x-ethi", "monospace");

pref("font.name.serif.x-geor", "serif");
pref("font.name.sans-serif.x-geor", "sans-serif");
pref("font.name.monospace.x-geor", "monospace");

pref("font.name.serif.x-gujr", "serif");
pref("font.name.sans-serif.x-gujr", "sans-serif");
pref("font.name.monospace.x-gujr", "monospace");

pref("font.name.serif.x-guru", "serif");
pref("font.name.sans-serif.x-guru", "sans-serif");
pref("font.name.monospace.x-guru", "monospace");

pref("font.name.serif.x-khmr", "serif");
pref("font.name.sans-serif.x-khmr", "sans-serif");
pref("font.name.monospace.x-khmr", "monospace");

pref("font.name.serif.x-knda", "serif");
pref("font.name.sans-serif.x-knda", "sans-serif");
pref("font.name.monospace.x-knda", "monospace");

pref("font.name.serif.x-mlym", "serif");
pref("font.name.sans-serif.x-mlym", "sans-serif");
pref("font.name.monospace.x-mlym", "monospace");

pref("font.name.serif.x-orya", "serif");
pref("font.name.sans-serif.x-orya", "sans-serif");
pref("font.name.monospace.x-orya", "monospace");

pref("font.name.serif.x-sinh", "serif");
pref("font.name.sans-serif.x-sinh", "sans-serif");
pref("font.name.monospace.x-sinh", "monospace");

pref("font.name.serif.x-tamil", "serif");
pref("font.name.sans-serif.x-tamil", "sans-serif");
pref("font.name.monospace.x-tamil", "monospace");

pref("font.name.serif.x-telu", "serif");
pref("font.name.sans-serif.x-telu", "sans-serif");
pref("font.name.monospace.x-telu", "monospace");

pref("font.name.serif.x-tibt", "serif");
pref("font.name.sans-serif.x-tibt", "sans-serif");
pref("font.name.monospace.x-tibt", "monospace");

pref("font.name.serif.x-unicode", "serif");
pref("font.name.sans-serif.x-unicode", "sans-serif");
pref("font.name.monospace.x-unicode", "monospace");
pref("font.size.fixed.x-unicode", 12);

pref("font.name.serif.x-western", "serif");
pref("font.name.sans-serif.x-western", "sans-serif");
pref("font.name.monospace.x-western", "monospace");
pref("font.size.fixed.x-western", 12);

pref("font.name.serif.zh-CN", "serif");
pref("font.name.sans-serif.zh-CN", "sans-serif");
pref("font.name.monospace.zh-CN", "monospace");

pref("font.name.serif.zh-HK", "serif");
pref("font.name.sans-serif.zh-HK", "sans-serif");
pref("font.name.monospace.zh-HK", "monospace");

pref("font.name.serif.zh-TW", "serif");
pref("font.name.sans-serif.zh-TW", "sans-serif");
pref("font.name.monospace.zh-TW", "monospace");

#ifdef MOZ_WIDGET_GTK
pref("gfx.font_rendering.fontconfig.fontlist.enabled", true);

// maximum number of fonts to substitute for a generic
pref("gfx.font_rendering.fontconfig.max_generic_substitutions", 3);
#endif

#endif
#endif
#endif

#if defined(ANDROID) || defined(MOZ_B2G)

pref("font.size.fixed.ar", 12);

pref("font.default.el", "sans-serif");
pref("font.size.fixed.el", 12);

pref("font.size.fixed.he", 12);

pref("font.default.x-cyrillic", "sans-serif");
pref("font.size.fixed.x-cyrillic", 12);

pref("font.default.x-unicode", "sans-serif");
pref("font.size.fixed.x-unicode", 12);

pref("font.default.x-western", "sans-serif");
pref("font.size.fixed.x-western", 12);

# ANDROID || MOZ_B2G
#endif

#if defined(MOZ_B2G)
// Gonk, FxOS Simulator, B2G Desktop and Mulet.

// TODO: some entries could probably be cleaned up.

// ar

pref("font.name.serif.el", "Droid Serif"); // not Charis SIL Compact, only has a few Greek chars
pref("font.name.sans-serif.el", "Fira Sans");
pref("font.name.monospace.el", "Fira Mono");

pref("font.name.serif.he", "Charis SIL Compact");
pref("font.name.sans-serif.he", "Fira Sans");
pref("font.name.monospace.he", "Fira Mono");
pref("font.name-list.sans-serif.he", "Droid Sans Hebrew, Fira Sans");

pref("font.name.serif.ja", "Charis SIL Compact");
pref("font.name.sans-serif.ja", "Fira Sans");
pref("font.name.monospace.ja", "MotoyaLMaru");
pref("font.name-list.sans-serif.ja", "Fira Sans, MotoyaLMaru, MotoyaLCedar, Droid Sans Japanese");
pref("font.name-list.monospace.ja", "MotoyaLMaru, MotoyaLCedar, Fira Mono");

pref("font.name.serif.ko", "Charis SIL Compact");
pref("font.name.sans-serif.ko", "Fira Sans");
pref("font.name.monospace.ko", "Fira Mono");

pref("font.name.serif.th", "Charis SIL Compact");
pref("font.name.sans-serif.th", "Fira Sans");
pref("font.name.monospace.th", "Fira Mono");
pref("font.name-list.sans-serif.th", "Fira Sans, Noto Sans Thai, Droid Sans Thai");

pref("font.name.serif.x-cyrillic", "Charis SIL Compact");
pref("font.name.sans-serif.x-cyrillic", "Fira Sans");
pref("font.name.monospace.x-cyrillic", "Fira Mono");

pref("font.name.serif.x-unicode", "Charis SIL Compact");
pref("font.name.sans-serif.x-unicode", "Fira Sans");
pref("font.name.monospace.x-unicode", "Fira Mono");

pref("font.name.serif.x-western", "Charis SIL Compact");
pref("font.name.sans-serif.x-western", "Fira Sans");
pref("font.name.monospace.x-western", "Fira Mono");

pref("font.name.serif.zh-CN", "Charis SIL Compact");
pref("font.name.sans-serif.zh-CN", "Fira Sans");
pref("font.name.monospace.zh-CN", "Fira Mono");
pref("font.name-list.sans-serif.zh-CN", "Fira Sans,Droid Sans Fallback");

pref("font.name.serif.zh-HK", "Charis SIL Compact");
pref("font.name.sans-serif.zh-HK", "Fira Sans");
pref("font.name.monospace.zh-HK", "Fira Mono");
pref("font.name-list.sans-serif.zh-HK", "Fira Sans,Droid Sans Fallback");

pref("font.name.serif.zh-TW", "Charis SIL Compact");
pref("font.name.sans-serif.zh-TW", "Fira Sans");
pref("font.name.monospace.zh-TW", "Fira Mono");
pref("font.name-list.sans-serif.zh-TW", "Fira Sans,Droid Sans Fallback");

pref("font.name.serif.x-math", "Latin Modern Math");
pref("font.name-list.serif.x-math", "Latin Modern Math, XITS Math, Cambria Math, Libertinus Math, DejaVu Math TeX Gyre, TeX Gyre Bonum Math, TeX Gyre Pagella Math, TeX Gyre Schola, TeX Gyre Termes Math, STIX Math, Asana Math, STIXGeneral, DejaVu Serif, DejaVu Sans, Charis SIL Compact");
pref("font.name.sans-serif.x-math", "Fira Sans");
pref("font.name.monospace.x-math", "Fira Mono");

#elif defined(ANDROID)
// We use the bundled fonts for Firefox for Android

// ar

pref("font.name.serif.el", "Droid Serif"); // not Charis SIL Compact, only has a few Greek chars
pref("font.name.sans-serif.el", "Clear Sans");
pref("font.name.monospace.el", "Droid Sans Mono");
pref("font.name-list.serif.el", "Noto Serif");
pref("font.name-list.sans-serif.el", "Clear Sans, Roboto, Droid Sans");

pref("font.name.serif.he", "Droid Serif");
pref("font.name.sans-serif.he", "Clear Sans");
pref("font.name.monospace.he", "Droid Sans Mono");
pref("font.name-list.serif.he", "Noto Serif");
pref("font.name-list.sans-serif.he", "Droid Sans Hebrew, Clear Sans, Droid Sans");

pref("font.name.serif.ja", "Charis SIL Compact");
pref("font.name.sans-serif.ja", "Clear Sans");
pref("font.name.monospace.ja", "MotoyaLMaru");
pref("font.name-list.serif.ja", "Noto Serif, Droid Serif");
pref("font.name-list.sans-serif.ja", "Clear Sans, Roboto, Droid Sans, MotoyaLMaru, MotoyaLCedar, Noto Sans JP, Droid Sans Japanese");
pref("font.name-list.monospace.ja", "MotoyaLMaru, MotoyaLCedar, Droid Sans Mono");

pref("font.name.serif.ko", "Charis SIL Compact");
pref("font.name.sans-serif.ko", "Clear Sans");
pref("font.name.monospace.ko", "Droid Sans Mono");
pref("font.name-list.serif.ko", "Noto Serif, Droid Serif, HYSerif");
pref("font.name-list.sans-serif.ko", "SmartGothic, NanumGothic, Noto Sans KR, DroidSansFallback, Droid Sans Fallback");

pref("font.name.serif.th", "Charis SIL Compact");
pref("font.name.sans-serif.th", "Clear Sans");
pref("font.name.monospace.th", "Droid Sans Mono");
pref("font.name-list.serif.th", "Noto Serif, Droid Serif");
pref("font.name-list.sans-serif.th", "Droid Sans Thai, Clear Sans, Droid Sans");

pref("font.name.serif.x-cyrillic", "Charis SIL Compact");
pref("font.name.sans-serif.x-cyrillic", "Clear Sans");
pref("font.name.monospace.x-cyrillic", "Droid Sans Mono");
pref("font.name-list.serif.x-cyrillic", "Noto Serif, Droid Serif");
pref("font.name-list.sans-serif.x-cyrillic", "Clear Sans, Roboto, Droid Sans");

pref("font.name.serif.x-unicode", "Charis SIL Compact");
pref("font.name.sans-serif.x-unicode", "Clear Sans");
pref("font.name.monospace.x-unicode", "Droid Sans Mono");
pref("font.name-list.serif.x-unicode", "Noto Serif, Droid Serif");
pref("font.name-list.sans-serif.x-unicode", "Clear Sans, Roboto, Droid Sans");

pref("font.name.serif.x-western", "Charis SIL Compact");
pref("font.name.sans-serif.x-western", "Clear Sans");
pref("font.name.monospace.x-western", "Droid Sans Mono");
pref("font.name-list.serif.x-western", "Noto Serif, Droid Serif");
pref("font.name-list.sans-serif.x-western", "Clear Sans, Roboto, Droid Sans");

pref("font.name.serif.zh-CN", "Charis SIL Compact");
pref("font.name.sans-serif.zh-CN", "Clear Sans");
pref("font.name.monospace.zh-CN", "Droid Sans Mono");
pref("font.name-list.serif.zh-CN", "Noto Serif, Droid Serif, Droid Sans Fallback");
pref("font.name-list.sans-serif.zh-CN", "Roboto, Droid Sans, Noto Sans SC, Droid Sans Fallback");
pref("font.name-list.monospace.zh-CN", "Droid Sans Fallback");

pref("font.name.serif.zh-HK", "Charis SIL Compact");
pref("font.name.sans-serif.zh-HK", "Clear Sans");
pref("font.name.monospace.zh-HK", "Droid Sans Mono");
pref("font.name-list.serif.zh-HK", "Noto Serif, Droid Serif, Droid Sans Fallback");
pref("font.name-list.sans-serif.zh-HK", "Roboto, Droid Sans, Noto Sans TC, Noto Sans SC, Droid Sans Fallback");
pref("font.name-list.monospace.zh-HK", "Droid Sans Fallback");

pref("font.name.serif.zh-TW", "Charis SIL Compact");
pref("font.name.sans-serif.zh-TW", "Clear Sans");
pref("font.name.monospace.zh-TW", "Droid Sans Mono");
pref("font.name-list.serif.zh-TW", "Noto Serif, Droid Serif, Droid Sans Fallback");
pref("font.name-list.sans-serif.zh-TW", "Roboto, Droid Sans, Noto Sans TC, Noto Sans SC, Droid Sans Fallback");
pref("font.name-list.monospace.zh-TW", "Droid Sans Fallback");

pref("font.name.serif.x-math", "Latin Modern Math");
pref("font.name-list.serif.x-math", "Latin Modern Math, XITS Math, Cambria Math, Libertinus Math, DejaVu Math TeX Gyre, TeX Gyre Bonum Math, TeX Gyre Pagella Math, TeX Gyre Schola, TeX Gyre Termes Math, STIX Math, Asana Math, STIXGeneral, DejaVu Serif, DejaVu Sans, Charis SIL Compact");
pref("font.name.sans-serif.x-math", "Clear Sans");
pref("font.name.monospace.x-math", "Droid Sans Mono");

#endif

#if OS_ARCH==AIX

// Override default Japanese fonts
pref("font.name.serif.ja", "dt-interface system-jisx0208.1983-0");
pref("font.name.sans-serif.ja", "dt-interface system-jisx0208.1983-0");
pref("font.name.monospace.ja", "dt-interface user-jisx0208.1983-0");

// Override default Cyrillic fonts
pref("font.name.serif.x-cyrillic", "dt-interface system-iso8859-5");
pref("font.name.sans-serif.x-cyrillic", "dt-interface system-iso8859-5");
pref("font.name.monospace.x-cyrillic", "dt-interface user-iso8859-5");

// Override default Unicode fonts
pref("font.name.serif.x-unicode", "dt-interface system-ucs2.cjk_japan-0");
pref("font.name.sans-serif.x-unicode", "dt-interface system-ucs2.cjk_japan-0");
pref("font.name.monospace.x-unicode", "dt-interface user-ucs2.cjk_japan-0");


#endif

// WebGL prefs
#ifdef ANDROID
// Disable MSAA on mobile.
pref("gl.msaa-level", 0);
#else
pref("gl.msaa-level", 2);
#endif
pref("gl.require-hardware", false);
#ifdef XP_MACOSX
pref("gl.multithreaded", true);
#endif

pref("webgl.force-enabled", false);
pref("webgl.disabled", false);
pref("webgl.disable-angle", false);
pref("webgl.disable-wgl", false);
pref("webgl.min_capability_mode", false);
pref("webgl.disable-extensions", false);
pref("webgl.msaa-force", false);
pref("webgl.prefer-16bpp", false);
pref("webgl.default-no-alpha", false);
pref("webgl.force-layers-readback", false);
pref("webgl.lose-context-on-memory-pressure", false);
pref("webgl.can-lose-context-in-foreground", true);
pref("webgl.restore-context-when-visible", true);
pref("webgl.max-warnings-per-context", 32);
pref("webgl.enable-draft-extensions", false);
pref("webgl.enable-privileged-extensions", false);
pref("webgl.bypass-shader-validation", false);
pref("webgl.disable-fail-if-major-performance-caveat", false);
pref("webgl.disable-DOM-blit-uploads", false);
pref("webgl.webgl2-compat-mode", false);

pref("webgl.enable-webgl2", true);

#ifdef RELEASE_BUILD
// Keep this disabled on Release and Beta for now. (see bug 1171228)
pref("webgl.enable-debug-renderer-info", false);
#else
pref("webgl.enable-debug-renderer-info", true);
#endif

pref("webgl.renderer-string-override", "");
pref("webgl.vendor-string-override", "");

#ifdef XP_WIN
pref("webgl.angle.try-d3d11", true);
pref("webgl.angle.force-d3d11", false);
pref("webgl.angle.force-warp", false);
pref("webgl.dxgl.enabled", true);
pref("webgl.dxgl.needs-finish", false);
#endif

pref("gfx.offscreencanvas.enabled", false);

#ifdef MOZ_WIDGET_GONK
pref("gfx.gralloc.fence-with-readpixels", false);
#endif

// Stagefright prefs
pref("stagefright.force-enabled", false);
pref("stagefright.disabled", false);

// Whether to disable acceleration for all widgets.
pref("layers.acceleration.disabled", false);
// Preference that when switched at runtime will run a series of benchmarks
// and output the result to stderr.
pref("layers.bench.enabled", false);

// Whether to force acceleration on, ignoring blacklists.
#ifdef ANDROID
// bug 838603 -- on Android, accidentally blacklisting OpenGL layers
// means a startup crash for everyone.
// Temporarily force-enable GL compositing.  This is default-disabled
// deep within the bowels of the widgetry system.  Remove me when GL
// compositing isn't default disabled in widget/android.
pref("layers.acceleration.force-enabled", true);
#else
pref("layers.acceleration.force-enabled", false);
#endif

pref("layers.acceleration.draw-fps", false);

// Enable DEAA antialiasing for transformed layers in the compositor
#if !defined(MOZ_WIDGET_GONK) && !defined(MOZ_WIDGET_ANDROID)
// Desktop prefs
pref("layers.deaa.enabled", true);
#else
// Mobile prefs
pref("layers.deaa.enabled", false);
#endif

pref("layers.dump", false);
#ifdef MOZ_DUMP_PAINTING
// If we're dumping layers, also dump the texture data
pref("layers.dump-texture", false);
pref("layers.dump-decision", false);
pref("layers.dump-client-layers", false);
pref("layers.dump-host-layers", false);
#endif
pref("layers.draw-borders", false);
pref("layers.draw-tile-borders", false);
pref("layers.draw-bigimage-borders", false);
pref("layers.frame-counter", false);
pref("layers.enable-tiles", false);
pref("layers.single-tile.enabled", true);
pref("layers.low-precision-buffer", false);
pref("layers.progressive-paint", false);
pref("layers.tile-width", 256);
pref("layers.tile-height", 256);
pref("layers.child-process-shutdown", true);
// Max number of layers per container. See Overwrite in mobile prefs.
pref("layers.max-active", -1);
// If this is set the tile size will only be treated as a suggestion.
// On B2G we will round this to the stride of the underlying allocation.
// On any platform we may later use the screen size and ignore
// tile-width/tile-height entirely. Its recommended to turn this off
// if you change the tile size.
pref("layers.tiles.adjust", true);

// Compositor target frame rate. NOTE: If vsync is enabled the compositor
// frame rate will still be capped.
// -1 -> default (match layout.frame_rate or 60 FPS)
// 0  -> full-tilt mode: Recomposite even if not transaction occured.
pref("layers.offmainthreadcomposition.frame-rate", -1);

#ifdef XP_MACOSX
pref("layers.enable-tiles", true);
pref("layers.tile-width", 512);
pref("layers.tile-height", 512);
pref("layers.tiles.edge-padding", false);
#endif

#ifdef MOZ_WIDGET_ANDROID
pref("layers.tiles.edge-padding", true);
#endif

// Whether to animate simple opacity and transforms on the compositor
pref("layers.offmainthreadcomposition.async-animations", true);

// Whether to log information about off main thread animations to stderr
pref("layers.offmainthreadcomposition.log-animations", false);

pref("layers.bufferrotation.enabled", true);

pref("layers.componentalpha.enabled", true);

// Use the DT-backend implemented PushLayer
pref("gfx.content.use-native-pushlayer", false);

#ifdef ANDROID
pref("gfx.apitrace.enabled",false);
#endif

#ifdef MOZ_X11
pref("gfx.content.use-native-pushlayer", true);
#ifdef MOZ_WIDGET_GTK
pref("gfx.xrender.enabled",false);
#endif
#endif

#ifdef XP_WIN
pref("gfx.content.use-native-pushlayer", true);

// Whether to disable the automatic detection and use of direct2d.
pref("gfx.direct2d.disabled", false);

// Whether to attempt to enable Direct2D regardless of automatic detection or
// blacklisting
pref("gfx.direct2d.force-enabled", false);

pref("layers.prefer-opengl", false);
pref("layers.prefer-d3d9", false);
pref("layers.allow-d3d9-fallback", true);
pref("layers.d3d11.force-warp", false);
pref("layers.d3d11.disable-warp", true);

#endif

// Copy-on-write canvas
pref("layers.shared-buffer-provider.enabled", true);

// Force all possible layers to be always active layers
pref("layers.force-active", false);

// Never use gralloc surfaces, even when they're available on this
// platform and are the optimal surface type.
pref("layers.gralloc.disable", false);

#ifdef XP_WIN
// On 32-bit Windows, fire a low-memory notification if we have less than this
// many mb of virtual address space available.
pref("memory.low_virtual_memory_threshold_mb", 128);

// On Windows 32- or 64-bit, fire a low-memory notification if we have less
// than this many mb of commit space (physical memory plus page file) left.
pref("memory.low_commit_space_threshold_mb", 128);

// On Windows 32- or 64-bit, fire a low-memory notification if we have less
// than this many mb of physical memory available on the whole machine.
pref("memory.low_physical_memory_threshold_mb", 0);

// On Windows 32- or 64-bit, don't fire a low-memory notification because of
// low available physical memory or low commit space more than once every
// low_memory_notification_interval_ms.
pref("memory.low_memory_notification_interval_ms", 10000);
#endif

// How long must we wait before declaring that a window is a "ghost" (i.e., a
// likely leak)?  This should be longer than it usually takes for an eligible
// window to be collected via the GC/CC.
pref("memory.ghost_window_timeout_seconds", 60);

// Disable freeing dirty pages when minimizing memory.
pref("memory.free_dirty_pages", false);

// Disable the Linux-specific, system-wide memory reporter.
#ifdef XP_LINUX
pref("memory.system_memory_reporter", false);
#endif

// Don't dump memory reports on OOM, by default.
pref("memory.dump_reports_on_oom", false);

// Number of stack frames to capture in createObjectURL for about:memory.
pref("memory.blob_report.stack_frames", 0);