#!/usr/bin/env bash

set -eu

SERVER_BASE=${SERVER_BASE:-http://localhost:8000}

PLAYER_ID=$(curl -s $SERVER_BASE/player -X POST|jq -r '.playerId')
DECK_ID=$(curl -s $SERVER_BASE/deck -X POST|jq -r '.deckId')

CASH=$(curl -s $SERVER_BASE/player/${PLAYER_ID}|jq -r '.cash')
echo "Your cash: $CASH"

while (( CASH > 0 )); do

  GAME_ID=$(curl -s $SERVER_BASE/game -d '{"deckId": '${DECK_ID}'}' |jq -r '.gameId')

  BET_CODE=null
  while [ "$BET_CODE" != "200" ]; do
    read -p "Your bet : " BET
    BET_RESP=$(curl --write-out '\n%{http_code}' -s --output - $SERVER_BASE/game/${GAME_ID}/bet -d '{"playerId": '${PLAYER_ID}', "bet": '${BET}'}')
    BET_CODE=$(echo "$BET_RESP"|tail -1)
  done

  # macOS compatible "tail -n -1"
  echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq

  BET_ID=$(echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq -r '.betId')
  YOUR_TOTAL=$(echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq -r '.yourTotal')
  RESULT=$(echo "$BET_RESP" | tail -r | tail -n +2 | tail -r | jq -r '.result')
  while [ "$RESULT" = "null" ]; do

    if (( YOUR_TOTAL >= 9 && YOUR_TOTAL <= 11 )); then
      read -p "[h]it, [s]tand or [d]ouble on 9,10,11 : " CMD
    else
      read -p "[h]it, [s]tand : " CMD
    fi

    if [ "$CMD" = "h" ]; then
      CMD=hit
    elif [ "$CMD" = "s" ]; then
      CMD=stand
    elif [ "$CMD" = "d" ]; then
      CMD=double
    else
      CMD=
    fi

    if [ -n "$CMD" ]; then
      RESP=$(curl -s $SERVER_BASE/game/${GAME_ID}/bet/${BET_ID}/$CMD -X POST)

      echo "$RESP"|jq

      RESULT=$(echo "$RESP"|jq -r '.result')
    fi

  done

  CASH=$(curl -s $SERVER_BASE/player/${PLAYER_ID}|jq -r '.cash')
  echo "Your cash: $CASH"
done
