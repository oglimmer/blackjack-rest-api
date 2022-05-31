"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; return _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function (obj) { return typeof obj; } : function (obj) { return obj && "function" == typeof Symbol && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }, _typeof(obj); }

define("ace/snippets/makefile", ["require", "exports", "module"], function (e, t, n) {
  "use strict";

  t.snippetText = "snippet ifeq\n	ifeq (${1:cond0},${2:cond1})\n		${3:code}\n	endif\n", t.scope = "makefile";
});

(function () {
  window.require(["ace/snippets/makefile"], function (m) {
    if ((typeof module === "undefined" ? "undefined" : _typeof(module)) == "object" && (typeof exports === "undefined" ? "undefined" : _typeof(exports)) == "object" && module) {
      module.exports = m;
    }
  });
})();