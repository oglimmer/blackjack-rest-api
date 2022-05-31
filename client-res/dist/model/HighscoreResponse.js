"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _ApiClient = _interopRequireDefault(require("../ApiClient"));

var _HighscoreElementResponse = _interopRequireDefault(require("./HighscoreElementResponse"));

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } }

function _createClass(Constructor, protoProps, staticProps) { if (protoProps) _defineProperties(Constructor.prototype, protoProps); if (staticProps) _defineProperties(Constructor, staticProps); Object.defineProperty(Constructor, "prototype", { writable: false }); return Constructor; }

/**
 * The HighscoreResponse model module.
 * @module model/HighscoreResponse
 * @version 1.0
 */
var HighscoreResponse = /*#__PURE__*/function () {
  /**
   * Constructs a new <code>HighscoreResponse</code>.
   * @alias module:model/HighscoreResponse
   */
  function HighscoreResponse() {
    _classCallCheck(this, HighscoreResponse);

    HighscoreResponse.initialize(this);
  }
  /**
   * Initializes the fields of this object.
   * This method is used by the constructors of any subclasses, in order to implement multiple inheritance (mix-ins).
   * Only for internal use.
   */


  _createClass(HighscoreResponse, null, [{
    key: "initialize",
    value: function initialize(obj) {}
    /**
     * Constructs a <code>HighscoreResponse</code> from a plain JavaScript object, optionally creating a new instance.
     * Copies all relevant properties from <code>data</code> to <code>obj</code> if supplied or a new instance if not.
     * @param {Object} data The plain JavaScript object bearing properties of interest.
     * @param {module:model/HighscoreResponse} obj Optional instance to populate.
     * @return {module:model/HighscoreResponse} The populated <code>HighscoreResponse</code> instance.
     */

  }, {
    key: "constructFromObject",
    value: function constructFromObject(data, obj) {
      if (data) {
        obj = obj || new HighscoreResponse();

        if (data.hasOwnProperty('highscores')) {
          obj['highscores'] = _ApiClient["default"].convertToType(data['highscores'], [_HighscoreElementResponse["default"]]);
        }
      }

      return obj;
    }
  }]);

  return HighscoreResponse;
}();
/**
 * @member {Array.<module:model/HighscoreElementResponse>} highscores
 */


HighscoreResponse.prototype['highscores'] = undefined;
var _default = HighscoreResponse;
exports["default"] = _default;