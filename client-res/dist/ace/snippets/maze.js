"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; return _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function (obj) { return typeof obj; } : function (obj) { return obj && "function" == typeof Symbol && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }, _typeof(obj); }

define("ace/snippets/maze", ["require", "exports", "module"], function (e, t, n) {
  "use strict";

  t.snippetText = "snippet >\ndescription assignment\nscope maze\n	-> ${1}= ${2}\n\nsnippet >\ndescription if\nscope maze\n	-> IF ${2:**} THEN %${3:L} ELSE %${4:R}\n", t.scope = "maze";
});

(function () {
  window.require(["ace/snippets/maze"], function (m) {
    if ((typeof module === "undefined" ? "undefined" : _typeof(module)) == "object" && (typeof exports === "undefined" ? "undefined" : _typeof(exports)) == "object" && module) {
      module.exports = m;
    }
  });
})();