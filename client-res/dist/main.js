"use strict";

function _typeof(obj) { "@babel/helpers - typeof"; return _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function (obj) { return typeof obj; } : function (obj) { return obj && "function" == typeof Symbol && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; }, _typeof(obj); }

var _index = require("./index.js");

function _regeneratorRuntime() { "use strict"; /*! regenerator-runtime -- Copyright (c) 2014-present, Facebook, Inc. -- license (MIT): https://github.com/facebook/regenerator/blob/main/LICENSE */ _regeneratorRuntime = function _regeneratorRuntime() { return exports; }; var exports = {}, Op = Object.prototype, hasOwn = Op.hasOwnProperty, $Symbol = "function" == typeof Symbol ? Symbol : {}, iteratorSymbol = $Symbol.iterator || "@@iterator", asyncIteratorSymbol = $Symbol.asyncIterator || "@@asyncIterator", toStringTagSymbol = $Symbol.toStringTag || "@@toStringTag"; function define(obj, key, value) { return Object.defineProperty(obj, key, { value: value, enumerable: !0, configurable: !0, writable: !0 }), obj[key]; } try { define({}, ""); } catch (err) { define = function define(obj, key, value) { return obj[key] = value; }; } function wrap(innerFn, outerFn, self, tryLocsList) { var protoGenerator = outerFn && outerFn.prototype instanceof Generator ? outerFn : Generator, generator = Object.create(protoGenerator.prototype), context = new Context(tryLocsList || []); return generator._invoke = function (innerFn, self, context) { var state = "suspendedStart"; return function (method, arg) { if ("executing" === state) throw new Error("Generator is already running"); if ("completed" === state) { if ("throw" === method) throw arg; return doneResult(); } for (context.method = method, context.arg = arg;;) { var delegate = context.delegate; if (delegate) { var delegateResult = maybeInvokeDelegate(delegate, context); if (delegateResult) { if (delegateResult === ContinueSentinel) continue; return delegateResult; } } if ("next" === context.method) context.sent = context._sent = context.arg;else if ("throw" === context.method) { if ("suspendedStart" === state) throw state = "completed", context.arg; context.dispatchException(context.arg); } else "return" === context.method && context.abrupt("return", context.arg); state = "executing"; var record = tryCatch(innerFn, self, context); if ("normal" === record.type) { if (state = context.done ? "completed" : "suspendedYield", record.arg === ContinueSentinel) continue; return { value: record.arg, done: context.done }; } "throw" === record.type && (state = "completed", context.method = "throw", context.arg = record.arg); } }; }(innerFn, self, context), generator; } function tryCatch(fn, obj, arg) { try { return { type: "normal", arg: fn.call(obj, arg) }; } catch (err) { return { type: "throw", arg: err }; } } exports.wrap = wrap; var ContinueSentinel = {}; function Generator() {} function GeneratorFunction() {} function GeneratorFunctionPrototype() {} var IteratorPrototype = {}; define(IteratorPrototype, iteratorSymbol, function () { return this; }); var getProto = Object.getPrototypeOf, NativeIteratorPrototype = getProto && getProto(getProto(values([]))); NativeIteratorPrototype && NativeIteratorPrototype !== Op && hasOwn.call(NativeIteratorPrototype, iteratorSymbol) && (IteratorPrototype = NativeIteratorPrototype); var Gp = GeneratorFunctionPrototype.prototype = Generator.prototype = Object.create(IteratorPrototype); function defineIteratorMethods(prototype) { ["next", "throw", "return"].forEach(function (method) { define(prototype, method, function (arg) { return this._invoke(method, arg); }); }); } function AsyncIterator(generator, PromiseImpl) { function invoke(method, arg, resolve, reject) { var record = tryCatch(generator[method], generator, arg); if ("throw" !== record.type) { var result = record.arg, value = result.value; return value && "object" == _typeof(value) && hasOwn.call(value, "__await") ? PromiseImpl.resolve(value.__await).then(function (value) { invoke("next", value, resolve, reject); }, function (err) { invoke("throw", err, resolve, reject); }) : PromiseImpl.resolve(value).then(function (unwrapped) { result.value = unwrapped, resolve(result); }, function (error) { return invoke("throw", error, resolve, reject); }); } reject(record.arg); } var previousPromise; this._invoke = function (method, arg) { function callInvokeWithMethodAndArg() { return new PromiseImpl(function (resolve, reject) { invoke(method, arg, resolve, reject); }); } return previousPromise = previousPromise ? previousPromise.then(callInvokeWithMethodAndArg, callInvokeWithMethodAndArg) : callInvokeWithMethodAndArg(); }; } function maybeInvokeDelegate(delegate, context) { var method = delegate.iterator[context.method]; if (undefined === method) { if (context.delegate = null, "throw" === context.method) { if (delegate.iterator["return"] && (context.method = "return", context.arg = undefined, maybeInvokeDelegate(delegate, context), "throw" === context.method)) return ContinueSentinel; context.method = "throw", context.arg = new TypeError("The iterator does not provide a 'throw' method"); } return ContinueSentinel; } var record = tryCatch(method, delegate.iterator, context.arg); if ("throw" === record.type) return context.method = "throw", context.arg = record.arg, context.delegate = null, ContinueSentinel; var info = record.arg; return info ? info.done ? (context[delegate.resultName] = info.value, context.next = delegate.nextLoc, "return" !== context.method && (context.method = "next", context.arg = undefined), context.delegate = null, ContinueSentinel) : info : (context.method = "throw", context.arg = new TypeError("iterator result is not an object"), context.delegate = null, ContinueSentinel); } function pushTryEntry(locs) { var entry = { tryLoc: locs[0] }; 1 in locs && (entry.catchLoc = locs[1]), 2 in locs && (entry.finallyLoc = locs[2], entry.afterLoc = locs[3]), this.tryEntries.push(entry); } function resetTryEntry(entry) { var record = entry.completion || {}; record.type = "normal", delete record.arg, entry.completion = record; } function Context(tryLocsList) { this.tryEntries = [{ tryLoc: "root" }], tryLocsList.forEach(pushTryEntry, this), this.reset(!0); } function values(iterable) { if (iterable) { var iteratorMethod = iterable[iteratorSymbol]; if (iteratorMethod) return iteratorMethod.call(iterable); if ("function" == typeof iterable.next) return iterable; if (!isNaN(iterable.length)) { var i = -1, next = function next() { for (; ++i < iterable.length;) { if (hasOwn.call(iterable, i)) return next.value = iterable[i], next.done = !1, next; } return next.value = undefined, next.done = !0, next; }; return next.next = next; } } return { next: doneResult }; } function doneResult() { return { value: undefined, done: !0 }; } return GeneratorFunction.prototype = GeneratorFunctionPrototype, define(Gp, "constructor", GeneratorFunctionPrototype), define(GeneratorFunctionPrototype, "constructor", GeneratorFunction), GeneratorFunction.displayName = define(GeneratorFunctionPrototype, toStringTagSymbol, "GeneratorFunction"), exports.isGeneratorFunction = function (genFun) { var ctor = "function" == typeof genFun && genFun.constructor; return !!ctor && (ctor === GeneratorFunction || "GeneratorFunction" === (ctor.displayName || ctor.name)); }, exports.mark = function (genFun) { return Object.setPrototypeOf ? Object.setPrototypeOf(genFun, GeneratorFunctionPrototype) : (genFun.__proto__ = GeneratorFunctionPrototype, define(genFun, toStringTagSymbol, "GeneratorFunction")), genFun.prototype = Object.create(Gp), genFun; }, exports.awrap = function (arg) { return { __await: arg }; }, defineIteratorMethods(AsyncIterator.prototype), define(AsyncIterator.prototype, asyncIteratorSymbol, function () { return this; }), exports.AsyncIterator = AsyncIterator, exports.async = function (innerFn, outerFn, self, tryLocsList, PromiseImpl) { void 0 === PromiseImpl && (PromiseImpl = Promise); var iter = new AsyncIterator(wrap(innerFn, outerFn, self, tryLocsList), PromiseImpl); return exports.isGeneratorFunction(outerFn) ? iter : iter.next().then(function (result) { return result.done ? result.value : iter.next(); }); }, defineIteratorMethods(Gp), define(Gp, toStringTagSymbol, "Generator"), define(Gp, iteratorSymbol, function () { return this; }), define(Gp, "toString", function () { return "[object Generator]"; }), exports.keys = function (object) { var keys = []; for (var key in object) { keys.push(key); } return keys.reverse(), function next() { for (; keys.length;) { var key = keys.pop(); if (key in object) return next.value = key, next.done = !1, next; } return next.done = !0, next; }; }, exports.values = values, Context.prototype = { constructor: Context, reset: function reset(skipTempReset) { if (this.prev = 0, this.next = 0, this.sent = this._sent = undefined, this.done = !1, this.delegate = null, this.method = "next", this.arg = undefined, this.tryEntries.forEach(resetTryEntry), !skipTempReset) for (var name in this) { "t" === name.charAt(0) && hasOwn.call(this, name) && !isNaN(+name.slice(1)) && (this[name] = undefined); } }, stop: function stop() { this.done = !0; var rootRecord = this.tryEntries[0].completion; if ("throw" === rootRecord.type) throw rootRecord.arg; return this.rval; }, dispatchException: function dispatchException(exception) { if (this.done) throw exception; var context = this; function handle(loc, caught) { return record.type = "throw", record.arg = exception, context.next = loc, caught && (context.method = "next", context.arg = undefined), !!caught; } for (var i = this.tryEntries.length - 1; i >= 0; --i) { var entry = this.tryEntries[i], record = entry.completion; if ("root" === entry.tryLoc) return handle("end"); if (entry.tryLoc <= this.prev) { var hasCatch = hasOwn.call(entry, "catchLoc"), hasFinally = hasOwn.call(entry, "finallyLoc"); if (hasCatch && hasFinally) { if (this.prev < entry.catchLoc) return handle(entry.catchLoc, !0); if (this.prev < entry.finallyLoc) return handle(entry.finallyLoc); } else if (hasCatch) { if (this.prev < entry.catchLoc) return handle(entry.catchLoc, !0); } else { if (!hasFinally) throw new Error("try statement without catch or finally"); if (this.prev < entry.finallyLoc) return handle(entry.finallyLoc); } } } }, abrupt: function abrupt(type, arg) { for (var i = this.tryEntries.length - 1; i >= 0; --i) { var entry = this.tryEntries[i]; if (entry.tryLoc <= this.prev && hasOwn.call(entry, "finallyLoc") && this.prev < entry.finallyLoc) { var finallyEntry = entry; break; } } finallyEntry && ("break" === type || "continue" === type) && finallyEntry.tryLoc <= arg && arg <= finallyEntry.finallyLoc && (finallyEntry = null); var record = finallyEntry ? finallyEntry.completion : {}; return record.type = type, record.arg = arg, finallyEntry ? (this.method = "next", this.next = finallyEntry.finallyLoc, ContinueSentinel) : this.complete(record); }, complete: function complete(record, afterLoc) { if ("throw" === record.type) throw record.arg; return "break" === record.type || "continue" === record.type ? this.next = record.arg : "return" === record.type ? (this.rval = this.arg = record.arg, this.method = "return", this.next = "end") : "normal" === record.type && afterLoc && (this.next = afterLoc), ContinueSentinel; }, finish: function finish(finallyLoc) { for (var i = this.tryEntries.length - 1; i >= 0; --i) { var entry = this.tryEntries[i]; if (entry.finallyLoc === finallyLoc) return this.complete(entry.completion, entry.afterLoc), resetTryEntry(entry), ContinueSentinel; } }, "catch": function _catch(tryLoc) { for (var i = this.tryEntries.length - 1; i >= 0; --i) { var entry = this.tryEntries[i]; if (entry.tryLoc === tryLoc) { var record = entry.completion; if ("throw" === record.type) { var thrown = record.arg; resetTryEntry(entry); } return thrown; } } throw new Error("illegal catch attempt"); }, delegateYield: function delegateYield(iterable, resultName, nextLoc) { return this.delegate = { iterator: values(iterable), resultName: resultName, nextLoc: nextLoc }, "next" === this.method && (this.arg = undefined), ContinueSentinel; } }, exports; }

function asyncGeneratorStep(gen, resolve, reject, _next, _throw, key, arg) { try { var info = gen[key](arg); var value = info.value; } catch (error) { reject(error); return; } if (info.done) { resolve(value); } else { Promise.resolve(value).then(_next, _throw); } }

function _asyncToGenerator(fn) { return function () { var self = this, args = arguments; return new Promise(function (resolve, reject) { var gen = fn.apply(self, args); function _next(value) { asyncGeneratorStep(gen, resolve, reject, _next, _throw, "next", value); } function _throw(err) { asyncGeneratorStep(gen, resolve, reject, _next, _throw, "throw", err); } _next(undefined); }); }; }

var promisify = function promisify(func) {
  return function () {
    for (var _len = arguments.length, funcParams = new Array(_len), _key = 0; _key < _len; _key++) {
      funcParams[_key] = arguments[_key];
    }

    return new Promise(function (resolve, reject) {
      if (funcParams.length == 0) {
        func(function (e, d) {
          if (e) {
            reject(e);
          } else {
            resolve(d);
          }
        });
      }

      if (funcParams.length == 1) {
        func(funcParams[0], function (e, d) {
          if (e) {
            reject(e);
          } else {
            resolve(d);
          }
        });
      }

      if (funcParams.length == 2) {
        func(funcParams[0], funcParams[1], function (e, d) {
          if (e) {
            reject(e);
          } else {
            resolve(d);
          }
        });
      }

      if (funcParams.length == 3) {
        func(funcParams[0], funcParams[1], funcParams[2], function (e, d) {
          if (e) {
            reject(e);
          } else {
            resolve(d);
          }
        });
      }

      if (funcParams.length == 4) {
        func(funcParams[0], funcParams[1], funcParams[2], funcParams[3], function (e, d) {
          if (e) {
            reject(e);
          } else {
            resolve(d);
          }
        });
      }
    });
  };
};

var DefaultApi = _index.DefaultApi;
var CreatePlayerRequest = _index.CreatePlayerRequest;
var CreateGameRequest = _index.CreateGameRequest;
var BetRequest = _index.BetRequest;
var InsuranceRequest = _index.InsuranceRequest;
var api = new DefaultApi();

var fillHighscore = /*#__PURE__*/function () {
  var _ref = _asyncToGenerator( /*#__PURE__*/_regeneratorRuntime().mark(function _callee() {
    var highscore, i, s;
    return _regeneratorRuntime().wrap(function _callee$(_context) {
      while (1) {
        switch (_context.prev = _context.next) {
          case 0:
            _context.prev = 0;
            _context.next = 3;
            return promisify(api.highscore.bind(api))();

          case 3:
            highscore = _context.sent;

            for (i = 0; i < highscore.highscores.length; i++) {
              s = highscore.highscores[i];
              document.getElementById('cell' + i + '_0').innerHTML = s.pos;
              document.getElementById('cell' + i + '_1').innerHTML = s.name;
              document.getElementById('cell' + i + '_2').innerHTML = s.money;
            }

            _context.next = 10;
            break;

          case 7:
            _context.prev = 7;
            _context.t0 = _context["catch"](0);
            console.log(_context.t0);

          case 10:
          case "end":
            return _context.stop();
        }
      }
    }, _callee, null, [[0, 7]]);
  }));

  return function fillHighscore() {
    return _ref.apply(this, arguments);
  };
}();

if (document.getElementById('highscore')) {
  fillHighscore();
}

var stop = false;
document.getElementById('stop').addEventListener('click', /*#__PURE__*/function () {
  var _ref2 = _asyncToGenerator( /*#__PURE__*/_regeneratorRuntime().mark(function _callee2(event) {
    return _regeneratorRuntime().wrap(function _callee2$(_context2) {
      while (1) {
        switch (_context2.prev = _context2.next) {
          case 0:
            stop = true;

          case 1:
          case "end":
            return _context2.stop();
        }
      }
    }, _callee2);
  }));

  return function (_x) {
    return _ref2.apply(this, arguments);
  };
}());
document.getElementById('start').addEventListener('click', /*#__PURE__*/function () {
  var _ref3 = _asyncToGenerator( /*#__PURE__*/_regeneratorRuntime().mark(function _callee3(event) {
    var cashDiv, infoDiv, wrapper, createPlayerRequest, playerName, playerResp, deckResp, playerInfoResp, maxMoney, createGameRequest, gameResp, betRequest, betResp, resp, insuranceAnswer, insuranceRequest, cmdStr, betResultResp;
    return _regeneratorRuntime().wrap(function _callee3$(_context3) {
      while (1) {
        switch (_context3.prev = _context3.next) {
          case 0:
            cashDiv = document.getElementById('cash');
            infoDiv = document.getElementById('info');
            wrapper = new Function(editor.getValue() + "; return [bet,command,insurance];");
            createPlayerRequest = new CreatePlayerRequest();
            playerName = document.getElementById('playerName').value;

            if (playerName) {
              createPlayerRequest.name = playerName;
            }

            _context3.next = 8;
            return promisify(api.createPlayer.bind(api))(createPlayerRequest);

          case 8:
            playerResp = _context3.sent;
            console.log(playerResp);
            infoDiv.innerHTML = JSON.stringify(playerResp);
            _context3.next = 13;
            return promisify(api.createDrawDeck.bind(api))();

          case 13:
            deckResp = _context3.sent;
            console.log(deckResp);
            infoDiv.innerHTML = JSON.stringify(deckResp);
            _context3.next = 18;
            return promisify(api.getPlayer.bind(api))(playerResp.playerId);

          case 18:
            playerInfoResp = _context3.sent;
            console.log(playerInfoResp);
            infoDiv.innerHTML = JSON.stringify(playerInfoResp);
            maxMoney = playerInfoResp.cash;
            stop = false;

          case 23:
            if (!(!stop && playerInfoResp.cash > 0)) {
              _context3.next = 91;
              break;
            }

            createGameRequest = new CreateGameRequest();
            createGameRequest.deckId = deckResp.deckId;
            _context3.next = 28;
            return promisify(api.createGame.bind(api))(createGameRequest);

          case 28:
            gameResp = _context3.sent;
            console.log(gameResp);
            infoDiv.innerHTML = JSON.stringify(gameResp);
            betRequest = new BetRequest();
            betRequest.bet = wrapper()[0](playerInfoResp.cash);
            betRequest.playerId = playerResp.playerId;
            _context3.next = 36;
            return promisify(api.placeBet.bind(api))(gameResp.gameId, betRequest);

          case 36:
            betResp = _context3.sent;
            console.log(betResp);
            infoDiv.innerHTML = JSON.stringify(betResp);
            resp = betResp;

            if (!resp.followActions.includes("insurance")) {
              _context3.next = 49;
              break;
            }

            insuranceAnswer = wrapper()[2](resp);
            insuranceRequest = new InsuranceRequest();
            insuranceRequest.insurance = insuranceAnswer ? "yes" : "no";
            _context3.next = 46;
            return promisify(api.insurance.bind(api))(gameResp.gameId, betResp.betId, insuranceRequest);

          case 46:
            resp = _context3.sent;
            console.log(resp);
            infoDiv.innerHTML = JSON.stringify(resp);

          case 49:
            if (!(resp.followActions.length > 0)) {
              _context3.next = 77;
              break;
            }

            cmdStr = wrapper()[1](resp);

            if (!(cmdStr == "HIT")) {
              _context3.next = 57;
              break;
            }

            _context3.next = 54;
            return promisify(api.hit.bind(api))(gameResp.gameId, betResp.betId);

          case 54:
            resp = _context3.sent;
            _context3.next = 73;
            break;

          case 57:
            if (!(cmdStr == "STAND")) {
              _context3.next = 63;
              break;
            }

            _context3.next = 60;
            return promisify(api.stand.bind(api))(gameResp.gameId, betResp.betId);

          case 60:
            resp = _context3.sent;
            _context3.next = 73;
            break;

          case 63:
            if (!(cmdStr == "DOUBLE")) {
              _context3.next = 69;
              break;
            }

            _context3.next = 66;
            return promisify(api["double"].bind(api))(gameResp.gameId, betResp.betId);

          case 66:
            resp = _context3.sent;
            _context3.next = 73;
            break;

          case 69:
            if (!(cmdStr == "SPLIT")) {
              _context3.next = 73;
              break;
            }

            _context3.next = 72;
            return promisify(api.split.bind(api))(gameResp.gameId, betResp.betId);

          case 72:
            resp = _context3.sent;

          case 73:
            console.log(resp);
            infoDiv.innerHTML = JSON.stringify(resp);
            _context3.next = 49;
            break;

          case 77:
            _context3.next = 79;
            return promisify(api.getBet.bind(api))(gameResp.gameId, betResp.betId);

          case 79:
            betResultResp = _context3.sent;
            console.log(betResultResp);
            infoDiv.innerHTML = JSON.stringify(betResultResp);
            _context3.next = 84;
            return promisify(api.getPlayer.bind(api))(playerResp.playerId);

          case 84:
            playerInfoResp = _context3.sent;
            console.log(playerInfoResp);
            infoDiv.innerHTML = JSON.stringify(playerInfoResp);
            cashDiv.innerHTML = playerInfoResp.cash;

            if (maxMoney < playerInfoResp.cash) {
              maxMoney = playerInfoResp.cash;
            }

            _context3.next = 23;
            break;

          case 91:
            cashDiv.innerHTML = "" + maxMoney + " (max)";

          case 92:
          case "end":
            return _context3.stop();
        }
      }
    }, _callee3);
  }));

  return function (_x2) {
    return _ref3.apply(this, arguments);
  };
}());