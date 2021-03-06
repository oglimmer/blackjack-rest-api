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

import ApiClient from '../ApiClient';

/**
 * The HitResponse model module.
 * @module model/HitResponse
 * @version 1.0
 */
class HitResponse {
    /**
     * Constructs a new <code>HitResponse</code>.
     * @alias module:model/HitResponse
     */
    constructor() { 
        
        HitResponse.initialize(this);
    }

    /**
     * Initializes the fields of this object.
     * This method is used by the constructors of any subclasses, in order to implement multiple inheritance (mix-ins).
     * Only for internal use.
     */
    static initialize(obj) { 
    }

    /**
     * Constructs a <code>HitResponse</code> from a plain JavaScript object, optionally creating a new instance.
     * Copies all relevant properties from <code>data</code> to <code>obj</code> if supplied or a new instance if not.
     * @param {Object} data The plain JavaScript object bearing properties of interest.
     * @param {module:model/HitResponse} obj Optional instance to populate.
     * @return {module:model/HitResponse} The populated <code>HitResponse</code> instance.
     */
    static constructFromObject(data, obj) {
        if (data) {
            obj = obj || new HitResponse();

            if (data.hasOwnProperty('drawnCard')) {
                obj['drawnCard'] = ApiClient.convertToType(data['drawnCard'], 'String');
            }
            if (data.hasOwnProperty('yourTotal')) {
                obj['yourTotal'] = ApiClient.convertToType(data['yourTotal'], 'Number');
            }
            if (data.hasOwnProperty('followActions')) {
                obj['followActions'] = ApiClient.convertToType(data['followActions'], ['String']);
            }
        }
        return obj;
    }


}

/**
 * @member {String} drawnCard
 */
HitResponse.prototype['drawnCard'] = undefined;

/**
 * @member {Number} yourTotal
 */
HitResponse.prototype['yourTotal'] = undefined;

/**
 * @member {Array.<String>} followActions
 */
HitResponse.prototype['followActions'] = undefined;






export default HitResponse;

