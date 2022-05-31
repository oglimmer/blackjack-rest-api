"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; return _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function (obj) { return typeof obj; } : function (obj) { return obj && "function" == typeof Symbol && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }, _typeof(obj); }

define("ace/mode/gitignore_highlight_rules", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text_highlight_rules"], function (e, t, n) {
  "use strict";

  var r = e("../lib/oop"),
      i = e("./text_highlight_rules").TextHighlightRules,
      s = function s() {
    this.$rules = {
      start: [{
        token: "comment",
        regex: /^\s*#.*$/
      }, {
        token: "keyword",
        regex: /^\s*!.*$/
      }]
    }, this.normalizeRules();
  };

  s.metaData = {
    fileTypes: ["gitignore"],
    name: "Gitignore"
  }, r.inherits(s, i), t.GitignoreHighlightRules = s;
}), define("ace/mode/gitignore", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text", "ace/mode/gitignore_highlight_rules"], function (e, t, n) {
  "use strict";

  var r = e("../lib/oop"),
      i = e("./text").Mode,
      s = e("./gitignore_highlight_rules").GitignoreHighlightRules,
      o = function o() {
    this.HighlightRules = s, this.$behaviour = this.$defaultBehaviour;
  };

  r.inherits(o, i), function () {
    this.lineCommentStart = "#", this.$id = "ace/mode/gitignore";
  }.call(o.prototype), t.Mode = o;
});

(function () {
  window.require(["ace/mode/gitignore"], function (m) {
    if ((typeof module === "undefined" ? "undefined" : _typeof(module)) == "object" && (typeof exports === "undefined" ? "undefined" : _typeof(exports)) == "object" && module) {
      module.exports = m;
    }
  });
})();