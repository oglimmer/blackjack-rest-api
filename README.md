# Run via Docker

```
docker build --tag blackjack .
docker run --rm -p 8000:8000 blackjack
```

Now use `SERVER_BASE=http://localhost:8000/v2 ./play.sh` to play via command line or use a browser at `http://localhost:8000/play.html` 

# Play against the official server bj.oglimmer.de

If you have Docker installed you can play via:

```bash
docker run -it --rm livepeerci/curl-jq bash -c "$(curl -sfLS https://raw.githubusercontent.com/oglimmer/blackjack-rest-api/master/play.sh)"
```

To play on your local terminal, you need to have curl and jq installed.

```bash
bash -c "$(curl -sfLS https://raw.githubusercontent.com/oglimmer/blackjack-rest-api/master/play.sh)"
```

# Build

## Prerequisites

You need to have oat++ and oat-swagger installed, see https://oatpp.io/docs/installation/unix-linux/ 

For Unix/macOS do this:

```
# oatpp
git clone https://github.com/oatpp/oatpp.git --depth=1
cd oatpp/
mkdir build
cd build
cmake ..
make install

# oatpp-swagger
git clone https://github.com/oglimmer/oatpp-swagger.git --depth=1
cd oatpp-swagger/
mkdir build
cd build
make install
```

For the UI you also need to have node and npm installed.

## Blackjack

This is how you build blackjack itself:

```bash
# get the Ace editor
git clone https://github.com/ajaxorg/ace-builds/ /tmp/ace
cp -r /tmp/ace/src-min client-res/static/ace
rm -rf /tmp/ace
# build UI
cd client-res
npm i
browserify src/main.js -o static/bundle.js -t [ babelify ]
cd ..
# build C++
mkdir build && cd build
cmake ..
make
```

# Run & Test

To run the server:

```bash
build/blackjack-exe
```

To play (and thus test the server)

```bash
SERVER_BASE=http://localhost:8000/v2 ./play.sh
```

or open a browser at http://localhost:8000

# How to play via curl

## Create your player (do once)
curl https://bj.oglimmer.de/v2/player -X POST

## Create a stack of cards (called deck, do this once):
curl https://bj.oglimmer.de/v2/deck -X POST

## Check your cash:
curl https://bj.oglimmer.de/v2/player/${PLAYER_ID}

## Start a single game (re-use player and deck)
curl https://bj.oglimmer.de/v2/game -d '{"deckId": '${DECK_ID}'}'

## Put your bet on a newly started game (once per game)
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet -d '{"playerId": '${PLAYER_ID}', "bet": '${BET}'}'

## Either Hit (take a card, do this as long as you want more cards)
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/hit -X POST
## or Stand (finish and let the dealer draw cards):
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/stand -X POST
## or Double (if your initial cards are 9,10,11 in total)
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/double -X POST
## or Split (if your initial cards are of the same rank)
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/split -X POST
## when the dealer has an Ace on the first card, you are asked for an insurance:
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/insurance -d '{"insurance": "${yes|no}"}'

## When the 'followActions' are [] use this to get the end result
curl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}

Use: https://raw.githubusercontent.com/oglimmer/blackjack-rest-api/master/play.sh to play via bash! (needs curl and jq installed)
