"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports["default"] = void 0;

var _ApiClient = _interopRequireDefault(require("../ApiClient"));

var _BetGetResponse = _interopRequireDefault(require("../model/BetGetResponse"));

var _BetRequest = _interopRequireDefault(require("../model/BetRequest"));

var _BetResponse = _interopRequireDefault(require("../model/BetResponse"));

var _CreateDeckResponse = _interopRequireDefault(require("../model/CreateDeckResponse"));

var _CreateGameRequest = _interopRequireDefault(require("../model/CreateGameRequest"));

var _CreateGameResponse = _interopRequireDefault(require("../model/CreateGameResponse"));

var _CreatePlayerRequest = _interopRequireDefault(require("../model/CreatePlayerRequest"));

var _CreatePlayerResponse = _interopRequireDefault(require("../model/CreatePlayerResponse"));

var _GetPlayerResponse = _interopRequireDefault(require("../model/GetPlayerResponse"));

var _HighscoreResponse = _interopRequireDefault(require("../model/HighscoreResponse"));

var _HitResponse = _interopRequireDefault(require("../model/HitResponse"));

var _InsuranceRequest = _interopRequireDefault(require("../model/InsuranceRequest"));

var _SplitResponse = _interopRequireDefault(require("../model/SplitResponse"));

var _StandResponse = _interopRequireDefault(require("../model/StandResponse"));

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { "default": obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } }

function _createClass(Constructor, protoProps, staticProps) { if (protoProps) _defineProperties(Constructor.prototype, protoProps); if (staticProps) _defineProperties(Constructor, staticProps); Object.defineProperty(Constructor, "prototype", { writable: false }); return Constructor; }

/**
* Default service.
* @module api/DefaultApi
* @version 1.0
*/
var DefaultApi = /*#__PURE__*/function () {
  /**
  * Constructs a new DefaultApi. 
  * @alias module:api/DefaultApi
  * @class
  * @param {module:ApiClient} [apiClient] Optional API client implementation to use,
  * default to {@link module:ApiClient#instance} if unspecified.
  */
  function DefaultApi(apiClient) {
    _classCallCheck(this, DefaultApi);

    this.apiClient = apiClient || _ApiClient["default"].instance;
  }
  /**
   * Callback function to receive the result of the createDrawDeck operation.
   * @callback module:api/DefaultApi~createDrawDeckCallback
   * @param {String} error Error message, if any.
   * @param {module:model/CreateDeckResponse} data The data returned by the service call.
   * @param {String} response The complete HTTP response.
   */

  /**
   * Creates a new Deck object
   * @param {module:api/DefaultApi~createDrawDeckCallback} callback The callback function, accepting three arguments: error, data, response
   * data is of type: {@link module:model/CreateDeckResponse}
   */


  _createClass(DefaultApi, [{
    key: "createDrawDeck",
    value: function createDrawDeck(callback) {
      var postBody = null;
      var pathParams = {};
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _CreateDeckResponse["default"];
      return this.apiClient.callApi('/v2/deck', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the createGame operation.
     * @callback module:api/DefaultApi~createGameCallback
     * @param {String} error Error message, if any.
     * @param {module:model/CreateGameResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Creates a new Game.
     * @param {module:model/CreateGameRequest} createGameRequest 
     * @param {module:api/DefaultApi~createGameCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/CreateGameResponse}
     */

  }, {
    key: "createGame",
    value: function createGame(createGameRequest, callback) {
      var postBody = createGameRequest; // verify the required parameter 'createGameRequest' is set

      if (createGameRequest === undefined || createGameRequest === null) {
        throw new Error("Missing the required parameter 'createGameRequest' when calling createGame");
      }

      var pathParams = {};
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = ['application/json'];
      var accepts = ['application/json'];
      var returnType = _CreateGameResponse["default"];
      return this.apiClient.callApi('/v2/game', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the createPlayer operation.
     * @callback module:api/DefaultApi~createPlayerCallback
     * @param {String} error Error message, if any.
     * @param {module:model/CreatePlayerResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Creates a new Player object
     * @param {module:model/CreatePlayerRequest} createPlayerRequest 
     * @param {module:api/DefaultApi~createPlayerCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/CreatePlayerResponse}
     */

  }, {
    key: "createPlayer",
    value: function createPlayer(createPlayerRequest, callback) {
      var postBody = createPlayerRequest; // verify the required parameter 'createPlayerRequest' is set

      if (createPlayerRequest === undefined || createPlayerRequest === null) {
        throw new Error("Missing the required parameter 'createPlayerRequest' when calling createPlayer");
      }

      var pathParams = {};
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = ['application/json'];
      var accepts = ['application/json'];
      var returnType = _CreatePlayerResponse["default"];
      return this.apiClient.callApi('/v2/player', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the doubleBet operation.
     * @callback module:api/DefaultApi~doubleBetCallback
     * @param {String} error Error message, if any.
     * @param {module:model/HitResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Doubles the bet and takes another card. This option is only available when the first two cards are 9,10,11 in total.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {Number} betId Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
     * @param {module:api/DefaultApi~doubleBetCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/HitResponse}
     */

  }, {
    key: "doubleBet",
    value: function doubleBet(gameId, betId, callback) {
      var postBody = null; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling doubleBet");
      } // verify the required parameter 'betId' is set


      if (betId === undefined || betId === null) {
        throw new Error("Missing the required parameter 'betId' when calling doubleBet");
      }

      var pathParams = {
        'gameId': gameId,
        'betId': betId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _HitResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet/{betId}/double', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the getBet operation.
     * @callback module:api/DefaultApi~getBetCallback
     * @param {String} error Error message, if any.
     * @param {module:model/BetGetResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Returns information about the dealer's cards, the dealer's total Value and who won the game.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {Number} betId Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
     * @param {module:api/DefaultApi~getBetCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/BetGetResponse}
     */

  }, {
    key: "getBet",
    value: function getBet(gameId, betId, callback) {
      var postBody = null; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling getBet");
      } // verify the required parameter 'betId' is set


      if (betId === undefined || betId === null) {
        throw new Error("Missing the required parameter 'betId' when calling getBet");
      }

      var pathParams = {
        'gameId': gameId,
        'betId': betId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _BetGetResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet/{betId}', 'GET', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the getPlayer operation.
     * @callback module:api/DefaultApi~getPlayerCallback
     * @param {String} error Error message, if any.
     * @param {module:model/GetPlayerResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Returns the cash a player owns
     * @param {Number} playerId Player ID, retrieved by POST /v2/player
     * @param {module:api/DefaultApi~getPlayerCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/GetPlayerResponse}
     */

  }, {
    key: "getPlayer",
    value: function getPlayer(playerId, callback) {
      var postBody = null; // verify the required parameter 'playerId' is set

      if (playerId === undefined || playerId === null) {
        throw new Error("Missing the required parameter 'playerId' when calling getPlayer");
      }

      var pathParams = {
        'playerId': playerId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _GetPlayerResponse["default"];
      return this.apiClient.callApi('/v2/player/{playerId}', 'GET', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the highscore operation.
     * @callback module:api/DefaultApi~highscoreCallback
     * @param {String} error Error message, if any.
     * @param {module:model/HighscoreResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Returns the highscore.
     * @param {module:api/DefaultApi~highscoreCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/HighscoreResponse}
     */

  }, {
    key: "highscore",
    value: function highscore(callback) {
      var postBody = null;
      var pathParams = {};
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _HighscoreResponse["default"];
      return this.apiClient.callApi('/v2/highscore', 'GET', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the hit operation.
     * @callback module:api/DefaultApi~hitCallback
     * @param {String} error Error message, if any.
     * @param {module:model/HitResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Hit - takes another card.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {Number} betId Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
     * @param {module:api/DefaultApi~hitCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/HitResponse}
     */

  }, {
    key: "hit",
    value: function hit(gameId, betId, callback) {
      var postBody = null; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling hit");
      } // verify the required parameter 'betId' is set


      if (betId === undefined || betId === null) {
        throw new Error("Missing the required parameter 'betId' when calling hit");
      }

      var pathParams = {
        'gameId': gameId,
        'betId': betId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _HitResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet/{betId}/hit', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the insurance operation.
     * @callback module:api/DefaultApi~insuranceCallback
     * @param {String} error Error message, if any.
     * @param {module:model/StandResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Answers the dealer's question for an insurance. This option is only available when the dealer's first card as an Ace.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {Number} betId Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
     * @param {module:model/InsuranceRequest} insuranceRequest 
     * @param {module:api/DefaultApi~insuranceCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/StandResponse}
     */

  }, {
    key: "insurance",
    value: function insurance(gameId, betId, insuranceRequest, callback) {
      var postBody = insuranceRequest; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling insurance");
      } // verify the required parameter 'betId' is set


      if (betId === undefined || betId === null) {
        throw new Error("Missing the required parameter 'betId' when calling insurance");
      } // verify the required parameter 'insuranceRequest' is set


      if (insuranceRequest === undefined || insuranceRequest === null) {
        throw new Error("Missing the required parameter 'insuranceRequest' when calling insurance");
      }

      var pathParams = {
        'gameId': gameId,
        'betId': betId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = ['application/json'];
      var accepts = ['application/json'];
      var returnType = _StandResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet/{betId}/insurance', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the placeBet operation.
     * @callback module:api/DefaultApi~placeBetCallback
     * @param {String} error Error message, if any.
     * @param {module:model/BetResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Places the initial bet on a game. Minimum 1, maximum 1000.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {module:model/BetRequest} betRequest 
     * @param {module:api/DefaultApi~placeBetCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/BetResponse}
     */

  }, {
    key: "placeBet",
    value: function placeBet(gameId, betRequest, callback) {
      var postBody = betRequest; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling placeBet");
      } // verify the required parameter 'betRequest' is set


      if (betRequest === undefined || betRequest === null) {
        throw new Error("Missing the required parameter 'betRequest' when calling placeBet");
      }

      var pathParams = {
        'gameId': gameId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = ['application/json'];
      var accepts = ['application/json'];
      var returnType = _BetResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the split operation.
     * @callback module:api/DefaultApi~splitCallback
     * @param {String} error Error message, if any.
     * @param {module:model/SplitResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Split your hand into two hands. Also draws 2 additional cards for each hand. This option is only available when the first two cards are of the same rank.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {Number} betId Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
     * @param {module:api/DefaultApi~splitCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/SplitResponse}
     */

  }, {
    key: "split",
    value: function split(gameId, betId, callback) {
      var postBody = null; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling split");
      } // verify the required parameter 'betId' is set


      if (betId === undefined || betId === null) {
        throw new Error("Missing the required parameter 'betId' when calling split");
      }

      var pathParams = {
        'gameId': gameId,
        'betId': betId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _SplitResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet/{betId}/split', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
    /**
     * Callback function to receive the result of the stand operation.
     * @callback module:api/DefaultApi~standCallback
     * @param {String} error Error message, if any.
     * @param {module:model/StandResponse} data The data returned by the service call.
     * @param {String} response The complete HTTP response.
     */

    /**
     * Stand - Finishes the game, thus let's the dealer take cards and finally pays the winner.
     * @param {Number} gameId Game ID, retrieved by POST /v2/game
     * @param {Number} betId Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
     * @param {module:api/DefaultApi~standCallback} callback The callback function, accepting three arguments: error, data, response
     * data is of type: {@link module:model/StandResponse}
     */

  }, {
    key: "stand",
    value: function stand(gameId, betId, callback) {
      var postBody = null; // verify the required parameter 'gameId' is set

      if (gameId === undefined || gameId === null) {
        throw new Error("Missing the required parameter 'gameId' when calling stand");
      } // verify the required parameter 'betId' is set


      if (betId === undefined || betId === null) {
        throw new Error("Missing the required parameter 'betId' when calling stand");
      }

      var pathParams = {
        'gameId': gameId,
        'betId': betId
      };
      var queryParams = {};
      var headerParams = {};
      var formParams = {};
      var authNames = [];
      var contentTypes = [];
      var accepts = ['application/json'];
      var returnType = _StandResponse["default"];
      return this.apiClient.callApi('/v2/game/{gameId}/bet/{betId}/stand', 'POST', pathParams, queryParams, headerParams, formParams, postBody, authNames, contentTypes, accepts, returnType, null, callback);
    }
  }]);

  return DefaultApi;
}();

exports["default"] = DefaultApi;