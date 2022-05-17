#!/usr/bin/env bash

set -eu

PLAYER_ID=$(curl -s http://localhost:8000/player -X POST|jq -r '.playerId')
DECK_ID=$(curl -s http://localhost:8000/deck -X POST|jq -r '.deckId')

CASH=$(curl -s http://localhost:8000/player/${PLAYER_ID}|jq -r '.cash')
echo "Your cash: $CASH"

while (( CASH > 0 )); do

  GAME_ID=$(curl -s http://localhost:8000/game -X POST -d '{"playerId": '${PLAYER_ID}', "deckId": '${DECK_ID}'}' -H 'Content-Type: application/json'|jq -r '.gameId')

  BET_CODE=null
  while [ "$BET_CODE" != "200" ]; do
    read -p "Your bet : " BET
    BET_RESP=$(curl --write-out '\n%{http_code}' -s --output - http://localhost:8000/game/${GAME_ID}/bet/${BET} -X POST)
    BET_CODE=$(echo "$BET_RESP"|tail -1)
  done

  # macOS compatible "tail -n -1"
  echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq

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
