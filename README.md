# Run via Docker

```
docker build --tag blackjack .
docker run --rm -p 8000:8000 blackjack
```

Now use `./play.sh`.

# Build

**You need to have oat++ installed, see https://oatpp.io/docs/installation/unix-linux/**

```bash
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
./play.sh
```

# How to play via curl

## Create your player (do once)
curl https://bj.oglimmer.de/player -X POST

## Create a stack of cards (called deck, do this once):
curl https://bj.oglimmer.de/deck -X POST

## Check your cash:
curl https://bj.oglimmer.de/player/${PLAYER_ID}

## Start a single game (re-use player and deck)
curl https://bj.oglimmer.de/game -d '{"playerId": '${PLAYER_ID}', "deckId": '${DECK_ID}'}'

## Put your bet on a newly started game (once per game)
curl https://bj.oglimmer.de/game/${GAME_ID}/bet/${BET} -X POST

## Either Hit (take a card, do this as long as you want more cards)
curl https://bj.oglimmer.de/game/${GAME_ID}/hit -X POST
## or Stand (finish and let the dealer draw cards):
curl https://bj.oglimmer.de/game/${GAME_ID}/stand -X POST

      