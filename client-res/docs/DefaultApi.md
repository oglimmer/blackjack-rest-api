# BlackjackGamingService.DefaultApi

All URIs are relative to *https://bj.oglimmer.de*

Method | HTTP request | Description
------------- | ------------- | -------------
[**createDrawDeck**](DefaultApi.md#createDrawDeck) | **POST** /v2/deck | Creates a new Deck object
[**createGame**](DefaultApi.md#createGame) | **POST** /v2/game | Creates a new Game.
[**createPlayer**](DefaultApi.md#createPlayer) | **POST** /v2/player | Creates a new Player object
[**doubleBet**](DefaultApi.md#doubleBet) | **POST** /v2/game/{gameId}/bet/{betId}/double | Doubles the bet and takes another card. This option is only available when the first two cards are 9,10,11 in total.
[**getBet**](DefaultApi.md#getBet) | **GET** /v2/game/{gameId}/bet/{betId} | Returns information about the dealer&#39;s cards, the dealer&#39;s total Value and who won the game.
[**getPlayer**](DefaultApi.md#getPlayer) | **GET** /v2/player/{playerId} | Returns the cash a player owns
[**highscore**](DefaultApi.md#highscore) | **GET** /v2/highscore | Returns the highscore.
[**hit**](DefaultApi.md#hit) | **POST** /v2/game/{gameId}/bet/{betId}/hit | Hit - takes another card.
[**insurance**](DefaultApi.md#insurance) | **POST** /v2/game/{gameId}/bet/{betId}/insurance | Answers the dealer&#39;s question for an insurance. This option is only available when the dealer&#39;s first card as an Ace.
[**placeBet**](DefaultApi.md#placeBet) | **POST** /v2/game/{gameId}/bet | Places the initial bet on a game. Minimum 1, maximum 1000.
[**split**](DefaultApi.md#split) | **POST** /v2/game/{gameId}/bet/{betId}/split | Split your hand into two hands. Also draws 2 additional cards for each hand. This option is only available when the first two cards are of the same rank.
[**stand**](DefaultApi.md#stand) | **POST** /v2/game/{gameId}/bet/{betId}/stand | Stand - Finishes the game, thus let&#39;s the dealer take cards and finally pays the winner.



## createDrawDeck

> CreateDeckResponse createDrawDeck()

Creates a new Deck object

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
apiInstance.createDrawDeck((error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters

This endpoint does not need any parameter.

### Return type

[**CreateDeckResponse**](CreateDeckResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## createGame

> CreateGameResponse createGame(createGameRequest)

Creates a new Game.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let createGameRequest = new BlackjackGamingService.CreateGameRequest(); // CreateGameRequest | 
apiInstance.createGame(createGameRequest, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **createGameRequest** | [**CreateGameRequest**](CreateGameRequest.md)|  | 

### Return type

[**CreateGameResponse**](CreateGameResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json


## createPlayer

> CreatePlayerResponse createPlayer(createPlayerRequest)

Creates a new Player object

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let createPlayerRequest = new BlackjackGamingService.CreatePlayerRequest(); // CreatePlayerRequest | 
apiInstance.createPlayer(createPlayerRequest, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **createPlayerRequest** | [**CreatePlayerRequest**](CreatePlayerRequest.md)|  | 

### Return type

[**CreatePlayerResponse**](CreatePlayerResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json


## doubleBet

> HitResponse doubleBet(gameId, betId)

Doubles the bet and takes another card. This option is only available when the first two cards are 9,10,11 in total.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betId = 56; // Number | Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
apiInstance.doubleBet(gameId, betId, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betId** | **Number**| Bet ID, retrieved by POST /v2/game/&lt;GAMEID&gt;/bet | 

### Return type

[**HitResponse**](HitResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## getBet

> BetGetResponse getBet(gameId, betId)

Returns information about the dealer&#39;s cards, the dealer&#39;s total Value and who won the game.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betId = 56; // Number | Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
apiInstance.getBet(gameId, betId, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betId** | **Number**| Bet ID, retrieved by POST /v2/game/&lt;GAMEID&gt;/bet | 

### Return type

[**BetGetResponse**](BetGetResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## getPlayer

> GetPlayerResponse getPlayer(playerId)

Returns the cash a player owns

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let playerId = 56; // Number | Player ID, retrieved by POST /v2/player
apiInstance.getPlayer(playerId, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **playerId** | **Number**| Player ID, retrieved by POST /v2/player | 

### Return type

[**GetPlayerResponse**](GetPlayerResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## highscore

> HighscoreResponse highscore()

Returns the highscore.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
apiInstance.highscore((error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters

This endpoint does not need any parameter.

### Return type

[**HighscoreResponse**](HighscoreResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## hit

> HitResponse hit(gameId, betId)

Hit - takes another card.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betId = 56; // Number | Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
apiInstance.hit(gameId, betId, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betId** | **Number**| Bet ID, retrieved by POST /v2/game/&lt;GAMEID&gt;/bet | 

### Return type

[**HitResponse**](HitResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## insurance

> StandResponse insurance(gameId, betId, insuranceRequest)

Answers the dealer&#39;s question for an insurance. This option is only available when the dealer&#39;s first card as an Ace.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betId = 56; // Number | Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
let insuranceRequest = new BlackjackGamingService.InsuranceRequest(); // InsuranceRequest | 
apiInstance.insurance(gameId, betId, insuranceRequest, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betId** | **Number**| Bet ID, retrieved by POST /v2/game/&lt;GAMEID&gt;/bet | 
 **insuranceRequest** | [**InsuranceRequest**](InsuranceRequest.md)|  | 

### Return type

[**StandResponse**](StandResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json


## placeBet

> BetResponse placeBet(gameId, betRequest)

Places the initial bet on a game. Minimum 1, maximum 1000.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betRequest = new BlackjackGamingService.BetRequest(); // BetRequest | 
apiInstance.placeBet(gameId, betRequest, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betRequest** | [**BetRequest**](BetRequest.md)|  | 

### Return type

[**BetResponse**](BetResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: application/json
- **Accept**: application/json


## split

> SplitResponse split(gameId, betId)

Split your hand into two hands. Also draws 2 additional cards for each hand. This option is only available when the first two cards are of the same rank.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betId = 56; // Number | Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
apiInstance.split(gameId, betId, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betId** | **Number**| Bet ID, retrieved by POST /v2/game/&lt;GAMEID&gt;/bet | 

### Return type

[**SplitResponse**](SplitResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json


## stand

> StandResponse stand(gameId, betId)

Stand - Finishes the game, thus let&#39;s the dealer take cards and finally pays the winner.

### Example

```javascript
import BlackjackGamingService from 'blackjack_gaming_service';

let apiInstance = new BlackjackGamingService.DefaultApi();
let gameId = 56; // Number | Game ID, retrieved by POST /v2/game
let betId = 56; // Number | Bet ID, retrieved by POST /v2/game/<GAMEID>/bet
apiInstance.stand(gameId, betId, (error, data, response) => {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
});
```

### Parameters


Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **gameId** | **Number**| Game ID, retrieved by POST /v2/game | 
 **betId** | **Number**| Bet ID, retrieved by POST /v2/game/&lt;GAMEID&gt;/bet | 

### Return type

[**StandResponse**](StandResponse.md)

### Authorization

No authorization required

### HTTP request headers

- **Content-Type**: Not defined
- **Accept**: application/json

