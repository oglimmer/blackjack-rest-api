"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; return _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function (obj) { return typeof obj; } : function (obj) { return obj && "function" == typeof Symbol && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }, _typeof(obj); }

define("ace/mode/lucene_highlight_rules", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text_highlight_rules"], function (e, t, n) {
  "use strict";

  var r = e("../lib/oop"),
      i = e("./text_highlight_rules").TextHighlightRules,
      s = function s() {
    this.$rules = {
      start: [{
        token: "constant.language.escape",
        regex: /\\[\-+&|!(){}\[\]^"~*?:\\]/
      }, {
        token: "constant.character.negation",
        regex: "\\-"
      }, {
        token: "constant.character.interro",
        regex: "\\?"
      }, {
        token: "constant.character.required",
        regex: "\\+"
      }, {
        token: "constant.character.asterisk",
        regex: "\\*"
      }, {
        token: "constant.character.proximity",
        regex: "~(?:0\\.[0-9]+|[0-9]+)?"
      }, {
        token: "keyword.operator",
        regex: "(AND|OR|NOT|TO)\\b"
      }, {
        token: "paren.lparen",
        regex: "[\\(\\{\\[]"
      }, {
        token: "paren.rparen",
        regex: "[\\)\\}\\]]"
      }, {
        token: "keyword.operator",
        regex: /[><=^]/
      }, {
        token: "constant.numeric",
        regex: /\d[\d.-]*/
      }, {
        token: "string",
        regex: /"(?:\\"|[^"])*"/
      }, {
        token: "keyword",
        regex: /(?:\\.|[^\s\-+&|!(){}\[\]^"~*?:\\])+:/,
        next: "maybeRegex"
      }, {
        token: "term",
        regex: /\w+/
      }, {
        token: "text",
        regex: /\s+/
      }],
      maybeRegex: [{
        token: "text",
        regex: /\s+/
      }, {
        token: "string.regexp.start",
        regex: "/",
        next: "regex"
      }, {
        regex: "",
        next: "start"
      }],
      regex: [{
        token: "regexp.keyword.operator",
        regex: "\\\\(?:u[\\da-fA-F]{4}|x[\\da-fA-F]{2}|.)"
      }, {
        token: "string.regexp.end",
        regex: "/[sxngimy]*",
        next: "no_regex"
      }, {
        token: "invalid",
        regex: /\{\d+\b,?\d*\}[+*]|[+*$^?][+*]|[$^][?]|\?{3,}/
      }, {
        token: "constant.language.escape",
        regex: /\(\?[:=!]|\)|\{\d+\b,?\d*\}|[+*]\?|[()$^+*?.]/
      }, {
        token: "constant.language.escape",
        regex: "<d+-d+>|[~&@]"
      }, {
        token: "constant.language.delimiter",
        regex: /\|/
      }, {
        token: "constant.language.escape",
        regex: /\[\^?/,
        next: "regex_character_class"
      }, {
        token: "empty",
        regex: "$",
        next: "no_regex"
      }, {
        defaultToken: "string.regexp"
      }],
      regex_character_class: [{
        token: "regexp.charclass.keyword.operator",
        regex: "\\\\(?:u[\\da-fA-F]{4}|x[\\da-fA-F]{2}|.)"
      }, {
        token: "constant.language.escape",
        regex: "]",
        next: "regex"
      }, {
        token: "constant.language.escape",
        regex: "-"
      }, {
        token: "empty",
        regex: "$",
        next: "no_regex"
      }, {
        defaultToken: "string.regexp.charachterclass"
      }]
    };
  };

  r.inherits(s, i), t.LuceneHighlightRules = s;
}), define("ace/mode/lucene", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text", "ace/mode/lucene_highlight_rules"], function (e, t, n) {
  "use strict";

  var r = e("../lib/oop"),
      i = e("./text").Mode,
      s = e("./lucene_highlight_rules").LuceneHighlightRules,
      o = function o() {
    this.HighlightRules = s, this.$behaviour = this.$defaultBehaviour;
  };

  r.inherits(o, i), function () {
    this.$id = "ace/mode/lucene";
  }.call(o.prototype), t.Mode = o;
});

(function () {
  window.require(["ace/mode/lucene"], function (m) {
    if ((typeof module === "undefined" ? "undefined" : _typeof(module)) == "object" && (typeof exports === "undefined" ? "undefined" : _typeof(exports)) == "object" && module) {
      module.exports = m;
    }
  });
})();