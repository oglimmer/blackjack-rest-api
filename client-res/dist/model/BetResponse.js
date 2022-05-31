"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _ApiClient = _interopRequireDefault(require("../ApiClient"));

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } }

function _createClass(Constructor, protoProps, staticProps) { if (protoProps) _defineProperties(Constructor.prototype, protoProps); if (staticProps) _defineProperties(Constructor, staticProps); Object.defineProperty(Constructor, "prototype", { writable: false }); return Constructor; }

/**
 * The BetResponse model module.
 * @module model/BetResponse
 * @version 1.0
 */
var BetResponse = /*#__PURE__*/function () {
  /**
   * Constructs a new <code>BetResponse</code>.
   * @alias module:model/BetResponse
   */
  function BetResponse() {
    _classCallCheck(this, BetResponse);

    BetResponse.initialize(this);
  }
  /**
   * Initializes the fields of this object.
   * This method is used by the constructors of any subclasses, in order to implement multiple inheritance (mix-ins).
   * Only for internal use.
   */


  _createClass(BetResponse, null, [{
    key: "initialize",
    value: function initialize(obj) {}
    /**
     * Constructs a <code>BetResponse</code> from a plain JavaScript object, optionally creating a new instance.
     * Copies all relevant properties from <code>data</code> to <code>obj</code> if supplied or a new instance if not.
     * @param {Object} data The plain JavaScript object bearing properties of interest.
     * @param {module:model/BetResponse} obj Optional instance to populate.
     * @return {module:model/BetResponse} The populated <code>BetResponse</code> instance.
     */

  }, {
    key: "constructFromObject",
    value: function constructFromObject(data, obj) {
      if (data) {
        obj = obj || new BetResponse();

        if (data.hasOwnProperty('card1')) {
          obj['card1'] = _ApiClient["default"].convertToType(data['card1'], 'String');
        }

        if (data.hasOwnProperty('card2')) {
          obj['card2'] = _ApiClient["default"].convertToType(data['card2'], 'String');
        }

        if (data.hasOwnProperty('dealersCard')) {
          obj['dealersCard'] = _ApiClient["default"].convertToType(data['dealersCard'], 'String');
        }

        if (data.hasOwnProperty('yourTotal')) {
          obj['yourTotal'] = _ApiClient["default"].convertToType(data['yourTotal'], 'Number');
        }

        if (data.hasOwnProperty('followActions')) {
          obj['followActions'] = _ApiClient["default"].convertToType(data['followActions'], ['String']);
        }

        if (data.hasOwnProperty('betId')) {
          obj['betId'] = _ApiClient["default"].convertToType(data['betId'], 'Number');
        }
      }

      return obj;
    }
  }]);

  return BetResponse;
}();
/**
 * @member {String} card1
 */


BetResponse.prototype['card1'] = undefined;
/**
 * @member {String} card2
 */

BetResponse.prototype['card2'] = undefined;
/**
 * @member {String} dealersCard
 */

BetResponse.prototype['dealersCard'] = undefined;
/**
 * @member {Number} yourTotal
 */

BetResponse.prototype['yourTotal'] = undefined;
/**
 * @member {Array.<String>} followActions
 */

BetResponse.prototype['followActions'] = undefined;
/**
 * @member {Number} betId
 */

BetResponse.prototype['betId'] = undefined;
var _default = BetResponse;
exports["default"] = _default;