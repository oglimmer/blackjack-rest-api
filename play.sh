#!/usr/bin/env bash

set -eu

PLAYER_ID=$(curl -s http://localhost:8000/player -X POST|jq -r '.playerId')
DECK_ID=$(curl -s http://localhost:8000/deck -X POST|jq -r '.deckId')

CASH=$(curl -s http://localhost:8000/player/${PLAYER_ID}|jq -r '.cash')
echo "Your cash: $CASH"

while (( CASH > 0 )); do

  read -p "Your bet : " BET

  GAME_ID=$(curl -s http://localhost:8000/game -X POST -d '{"playerId": '${PLAYER_ID}', "deckId": '${DECK_ID}'}' -H 'Content-Type: application/json'|jq -r '.gameId')
  curl -s http://localhost:8000/game/${GAME_ID}/bet/${BET} -X POST|jq

  RESULT=null
  while [ "$RESULT" = "null" ]; do

    read -p "[h]it or [s]tand : " CMD

    if [ "$CMD" = "h" ]; then
      CMD=hit
    elif [ "$CMD" = "s" ]; then
      CMD=stand
    else
      CMD=
    fi

    if [ -n "$CMD" ]; then
      RESP=$(curl -s http://localhost:8000/game/${GAME_ID}/$CMD -X POST|jq)

      echo "$RESP"|jq

      RESULT=$(echo "$RESP"|jq -r '.result')
    fi

  done

  CASH=$(curl -s http://localhost:8000/player/${PLAYER_ID}|jq -r '.cash')
  echo "Your cash: $CASH"
done
