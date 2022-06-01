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
    instance = new BlackjackGamingService.SplitResponse();
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

  describe('SplitResponse', function() {
    it('should create an instance of SplitResponse', function() {
      // uncomment below and update the code to test SplitResponse
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be.a(BlackjackGamingService.SplitResponse);
    });

    it('should have the property firstBetCard1 (base name: "firstBetCard1")', function() {
      // uncomment below and update the code to test the property firstBetCard1
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property firstBetCard2 (base name: "firstBetCard2")', function() {
      // uncomment below and update the code to test the property firstBetCard2
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property firstBetTotal (base name: "firstBetTotal")', function() {
      // uncomment below and update the code to test the property firstBetTotal
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property secondBetCard1 (base name: "secondBetCard1")', function() {
      // uncomment below and update the code to test the property secondBetCard1
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property secondBetCard2 (base name: "secondBetCard2")', function() {
      // uncomment below and update the code to test the property secondBetCard2
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property secondBetTotal (base name: "secondBetTotal")', function() {
      // uncomment below and update the code to test the property secondBetTotal
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property followActions (base name: "followActions")', function() {
      // uncomment below and update the code to test the property followActions
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property secondBetFollowAction (base name: "secondBetFollowAction")', function() {
      // uncomment below and update the code to test the property secondBetFollowAction
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

    it('should have the property secondBetId (base name: "secondBetId")', function() {
      // uncomment below and update the code to test the property secondBetId
      //var instance = new BlackjackGamingService.SplitResponse();
      //expect(instance).to.be();
    });

  });

}));