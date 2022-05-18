#!/usr/bin/env bash

set -eu

SERVER_BASE=${SERVER_BASE:-http://localhost:8000}

PLAYER_ID=$(curl -s $SERVER_BASE/player -X POST|jq -r '.playerId')
DECK_ID=$(curl -s $SERVER_BASE/deck -X POST|jq -r '.deckId')

CASH=$(curl -s $SERVER_BASE/player/${PLAYER_ID}|jq -r '.cash')
echo "Your cash: $CASH"

while (( CASH > 0 )); do

  GAME_ID=$(curl -s $SERVER_BASE/game -X POST -d '{"playerId": '${PLAYER_ID}', "deckId": '${DECK_ID}'}' -H 'Content-Type: application/json'|jq -r '.gameId')

  BET_CODE=null
  while [ "$BET_CODE" != "200" ]; do
    read -p "Your bet : " BET
    BET_RESP=$(curl --write-out '\n%{http_code}' -s --output - $SERVER_BASE/game/${GAME_ID}/bet/${BET} -X POST)
    BET_CODE=$(echo "$BET_RESP"|tail -1)
  done

  # macOS compatible "tail -n -1"
  echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq

  RESULT=$(echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq -r '.result')
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
      RESP=$(curl -s $SERVER_BASE/game/${GAME_ID}/$CMD -X POST|jq)

      echo "$RESP"|jq

      RESULT=$(echo "$RESP"|jq -r '.result')
    fi

  done

  CASH=$(curl -s $SERVER_BASE/player/${PLAYER_ID}|jq -r '.cash')
  echo "Your cash: $CASH"
done
