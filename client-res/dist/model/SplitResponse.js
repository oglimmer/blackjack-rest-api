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
 * The SplitResponse model module.
 * @module model/SplitResponse
 * @version 1.0
 */
var SplitResponse = /*#__PURE__*/function () {
  /**
   * Constructs a new <code>SplitResponse</code>.
   * @alias module:model/SplitResponse
   */
  function SplitResponse() {
    _classCallCheck(this, SplitResponse);

    SplitResponse.initialize(this);
  }
  /**
   * Initializes the fields of this object.
   * This method is used by the constructors of any subclasses, in order to implement multiple inheritance (mix-ins).
   * Only for internal use.
   */


  _createClass(SplitResponse, null, [{
    key: "initialize",
    value: function initialize(obj) {}
    /**
     * Constructs a <code>SplitResponse</code> from a plain JavaScript object, optionally creating a new instance.
     * Copies all relevant properties from <code>data</code> to <code>obj</code> if supplied or a new instance if not.
     * @param {Object} data The plain JavaScript object bearing properties of interest.
     * @param {module:model/SplitResponse} obj Optional instance to populate.
     * @return {module:model/SplitResponse} The populated <code>SplitResponse</code> instance.
     */

  }, {
    key: "constructFromObject",
    value: function constructFromObject(data, obj) {
      if (data) {
        obj = obj || new SplitResponse();

        if (data.hasOwnProperty('firstBetCard1')) {
          obj['firstBetCard1'] = _ApiClient["default"].convertToType(data['firstBetCard1'], 'String');
        }

        if (data.hasOwnProperty('firstBetCard2')) {
          obj['firstBetCard2'] = _ApiClient["default"].convertToType(data['firstBetCard2'], 'String');
        }

        if (data.hasOwnProperty('firstBetTotal')) {
          obj['firstBetTotal'] = _ApiClient["default"].convertToType(data['firstBetTotal'], 'Number');
        }

        if (data.hasOwnProperty('secondBetCard1')) {
          obj['secondBetCard1'] = _ApiClient["default"].convertToType(data['secondBetCard1'], 'String');
        }

        if (data.hasOwnProperty('secondBetCard2')) {
          obj['secondBetCard2'] = _ApiClient["default"].convertToType(data['secondBetCard2'], 'String');
        }

        if (data.hasOwnProperty('secondBetTotal')) {
          obj['secondBetTotal'] = _ApiClient["default"].convertToType(data['secondBetTotal'], 'Number');
        }

        if (data.hasOwnProperty('followActions')) {
          obj['followActions'] = _ApiClient["default"].convertToType(data['followActions'], ['String']);
        }

        if (data.hasOwnProperty('secondBetFollowAction')) {
          obj['secondBetFollowAction'] = _ApiClient["default"].convertToType(data['secondBetFollowAction'], ['String']);
        }

        if (data.hasOwnProperty('secondBetId')) {
          obj['secondBetId'] = _ApiClient["default"].convertToType(data['secondBetId'], 'Number');
        }
      }

      return obj;
    }
  }]);

  return SplitResponse;
}();
/**
 * @member {String} firstBetCard1
 */


SplitResponse.prototype['firstBetCard1'] = undefined;
/**
 * @member {String} firstBetCard2
 */

SplitResponse.prototype['firstBetCard2'] = undefined;
/**
 * @member {Number} firstBetTotal
 */

SplitResponse.prototype['firstBetTotal'] = undefined;
/**
 * @member {String} secondBetCard1
 */

SplitResponse.prototype['secondBetCard1'] = undefined;
/**
 * @member {String} secondBetCard2
 */

SplitResponse.prototype['secondBetCard2'] = undefined;
/**
 * @member {Number} secondBetTotal
 */

SplitResponse.prototype['secondBetTotal'] = undefined;
/**
 * @member {Array.<String>} followActions
 */

SplitResponse.prototype['followActions'] = undefined;
/**
 * @member {Array.<String>} secondBetFollowAction
 */

SplitResponse.prototype['secondBetFollowAction'] = undefined;
/**
 * @member {Number} secondBetId
 */

SplitResponse.prototype['secondBetId'] = undefined;
var _default = SplitResponse;
exports["default"] = _default;