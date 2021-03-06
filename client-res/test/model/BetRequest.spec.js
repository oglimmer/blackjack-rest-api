/**
 * Blackjack Gaming Service
 * A REST API to play Blackjack. Written in C++ with oat++.
 *
 * The version of the OpenAPI document: 1.0
 * 
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 *
 */

(function(root, factory) {
  if (typeof define === 'function' && define.amd) {
    // AMD.
    define(['expect.js', process.cwd()+'/src/index'], factory);
  } else if (typeof module === 'object' && module.exports) {
    // CommonJS-like environments that support module.exports, like Node.
    factory(require('expect.js'), require(process.cwd()+'/src/index'));
  } else {
    // Browser globals (root is window)
    factory(root.expect, root.BlackjackGamingService);
  }
}(this, function(expect, BlackjackGamingService) {
  'use strict';

  var instance;

  beforeEach(function() {
    instance = new BlackjackGamingService.BetRequest();
  });

  var getProperty = function(object, getter, property) {
    // Use getter method if present; otherwise, get the property directly.
    if (typeof object[getter] === 'function')
      return object[getter]();
    else
      return object[property];
  }

  var setProperty = function(object, setter, property, value) {
    // Use setter method if present; otherwise, set the property directly.
    if (typeof object[setter] === 'function')
      object[setter](value);
    else
      object[property] = value;
  }

  describe('BetRequest', function() {
    it('should create an instance of BetRequest', function() {
      // uncomment below and update the code to test BetRequest
      //var instance = new BlackjackGamingService.BetRequest();
      //expect(instance).to.be.a(BlackjackGamingService.BetRequest);
    });

    it('should have the property playerId (base name: "playerId")', function() {
      // uncomment below and update the code to test the property playerId
      //var instance = new BlackjackGamingService.BetRequest();
      //expect(instance).to.be();
    });

    it('should have the property bet (base name: "bet")', function() {
      // uncomment below and update the code to test the property bet
      //var instance = new BlackjackGamingService.BetRequest();
      //expect(instance).to.be();
    });

  });

}));
