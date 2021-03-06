# blackjack_gaming_service

BlackjackGamingService - JavaScript client for blackjack_gaming_service
A REST API to play Blackjack. Written in C++ with oat++.
This SDK is automatically generated by the [OpenAPI Generator](https://openapi-generator.tech) project:

- API version: 1.0
- Package version: 1.0
- Build package: org.openapitools.codegen.languages.JavascriptClientCodegen
For more information, please visit [https://bj.oglimmer.de](https://bj.oglimmer.de)

## Installation

### For [Node.js](https://nodejs.org/)

#### npm

To publish the library as a [npm](https://www.npmjs.com/), please follow the procedure in ["Publishing npm packages"](https://docs.npmjs.com/getting-started/publishing-npm-packages).

Then install it via:

```shell
npm install blackjack_gaming_service --save
```

Finally, you need to build the module:

```shell
npm run build
```

##### Local development

To use the library locally without publishing to a remote npm registry, first install the dependencies by changing into the directory containing `package.json` (and this README). Let's call this `JAVASCRIPT_CLIENT_DIR`. Then run:

```shell
npm install
```

Next, [link](https://docs.npmjs.com/cli/link) it globally in npm with the following, also from `JAVASCRIPT_CLIENT_DIR`:

```shell
npm link
```

To use the link you just defined in your project, switch to the directory you want to use your blackjack_gaming_service from, and run:

```shell
npm link /path/to/<JAVASCRIPT_CLIENT_DIR>
```

Finally, you need to build the module:

```shell
npm run build
```

#### git

If the library is hosted at a git repository, e.g.https://github.com/GIT_USER_ID/GIT_REPO_ID
then install it via:

```shell
    npm install GIT_USER_ID/GIT_REPO_ID --save
```

### For browser

The library also works in the browser environment via npm and [browserify](http://browserify.org/). After following
the above steps with Node.js and installing browserify with `npm install -g browserify`,
perform the following (assuming *main.js* is your entry file):

```shell
browserify main.js > bundle.js
```

Then include *bundle.js* in the HTML pages.

### Webpack Configuration

Using Webpack you may encounter the following error: "Module not found: Error:
Cannot resolve module", most certainly you should disable AMD loader. Add/merge
the following section to your webpack config:

```javascript
module: {
  rules: [
    {
      parser: {
        amd: false
      }
    }
  ]
}
```

## Getting Started

Please follow the [installation](#installation) instruction and execute the following JS code:

```javascript
var BlackjackGamingService = require('blackjack_gaming_service');


var api = new BlackjackGamingService.DefaultApi()
var callback = function(error, data, response) {
  if (error) {
    console.error(error);
  } else {
    console.log('API called successfully. Returned data: ' + data);
  }
};
api.createDrawDeck(callback);

```

## Documentation for API Endpoints

All URIs are relative to *https://bj.oglimmer.de*

Class | Method | HTTP request | Description
------------ | ------------- | ------------- | -------------
*BlackjackGamingService.DefaultApi* | [**createDrawDeck**](docs/DefaultApi.md#createDrawDeck) | **POST** /v2/deck | Creates a new Deck object
*BlackjackGamingService.DefaultApi* | [**createGame**](docs/DefaultApi.md#createGame) | **POST** /v2/game | Creates a new Game.
*BlackjackGamingService.DefaultApi* | [**createPlayer**](docs/DefaultApi.md#createPlayer) | **POST** /v2/player | Creates a new Player object
*BlackjackGamingService.DefaultApi* | [**doubleBet**](docs/DefaultApi.md#doubleBet) | **POST** /v2/game/{gameId}/bet/{betId}/double | Doubles the bet and takes another card. This option is only available when the first two cards are 9,10,11 in total.
*BlackjackGamingService.DefaultApi* | [**getBet**](docs/DefaultApi.md#getBet) | **GET** /v2/game/{gameId}/bet/{betId} | Returns information about the dealer&#39;s cards, the dealer&#39;s total Value and who won the game.
*BlackjackGamingService.DefaultApi* | [**getPlayer**](docs/DefaultApi.md#getPlayer) | **GET** /v2/player/{playerId} | Returns the cash a player owns
*BlackjackGamingService.DefaultApi* | [**highscore**](docs/DefaultApi.md#highscore) | **GET** /v2/highscore | Returns the highscore.
*BlackjackGamingService.DefaultApi* | [**hit**](docs/DefaultApi.md#hit) | **POST** /v2/game/{gameId}/bet/{betId}/hit | Hit - takes another card.
*BlackjackGamingService.DefaultApi* | [**insurance**](docs/DefaultApi.md#insurance) | **POST** /v2/game/{gameId}/bet/{betId}/insurance | Answers the dealer&#39;s question for an insurance. This option is only available when the dealer&#39;s first card as an Ace.
*BlackjackGamingService.DefaultApi* | [**placeBet**](docs/DefaultApi.md#placeBet) | **POST** /v2/game/{gameId}/bet | Places the initial bet on a game. Minimum 1, maximum 1000.
*BlackjackGamingService.DefaultApi* | [**split**](docs/DefaultApi.md#split) | **POST** /v2/game/{gameId}/bet/{betId}/split | Split your hand into two hands. Also draws 2 additional cards for each hand. This option is only available when the first two cards are of the same rank.
*BlackjackGamingService.DefaultApi* | [**stand**](docs/DefaultApi.md#stand) | **POST** /v2/game/{gameId}/bet/{betId}/stand | Stand - Finishes the game, thus let&#39;s the dealer take cards and finally pays the winner.


## Documentation for Models

 - [BlackjackGamingService.BetGetResponse](docs/BetGetResponse.md)
 - [BlackjackGamingService.BetRequest](docs/BetRequest.md)
 - [BlackjackGamingService.BetResponse](docs/BetResponse.md)
 - [BlackjackGamingService.CreateDeckResponse](docs/CreateDeckResponse.md)
 - [BlackjackGamingService.CreateGameRequest](docs/CreateGameRequest.md)
 - [BlackjackGamingService.CreateGameResponse](docs/CreateGameResponse.md)
 - [BlackjackGamingService.CreatePlayerRequest](docs/CreatePlayerRequest.md)
 - [BlackjackGamingService.CreatePlayerResponse](docs/CreatePlayerResponse.md)
 - [BlackjackGamingService.GetPlayerResponse](docs/GetPlayerResponse.md)
 - [BlackjackGamingService.HighscoreElementResponse](docs/HighscoreElementResponse.md)
 - [BlackjackGamingService.HighscoreResponse](docs/HighscoreResponse.md)
 - [BlackjackGamingService.HitResponse](docs/HitResponse.md)
 - [BlackjackGamingService.InsuranceRequest](docs/InsuranceRequest.md)
 - [BlackjackGamingService.SplitResponse](docs/SplitResponse.md)
 - [BlackjackGamingService.StandResponse](docs/StandResponse.md)


## Documentation for Authorization

All endpoints do not require authorization.
