#!/usr/bin/env bash

set -eu

SERVER_BASE=${SERVER_BASE:-http://localhost:8000/v2}

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
    BET_RESP=$(echo "$BET_RESP" | tail -r | tail -n +2 | tail -r)
  done

  # macOS compatible "tail -n -1"
  echo "$BET_RESP" | jq

  BET_2ND_ID=null
  ACTIONS_2ND=null
  BET_ID=$(echo "$BET_RESP" | jq -r '.betId')
  YOUR_TOTAL=$(echo "$BET_RESP" | jq -r '.yourTotal')
  ACTIONS=$(echo "$BET_RESP" | jq -r '.followActions')
  while [ "$ACTIONS" != "null" ]; do

    ACTIONS=$(echo "$ACTIONS" | jq -r 'join(" or ")')
    read -p "Do you want to ${ACTIONS}? : " CMD

    if [ "$CMD" = "h" ]; then
      CMD=hit
    elif [ "$CMD" = "s" ]; then
      CMD=stand
    elif [ "$CMD" = "d" ]; then
      CMD=double
    fi

    RESP=$(curl -s $SERVER_BASE/game/${GAME_ID}/bet/${BET_ID}/$CMD -X POST)
    ACTIONS=$(echo "$RESP" | jq -r '.followActions')

    if [ "$ACTIONS" != "null" ] || [ "$(echo "$RESP" | jq 'length')" != "1" ]; then
      echo "$RESP"|jq
    fi


    if [ "$BET_2ND_ID" = "null" ] && [ "$(echo "$RESP"|jq -r '.secondBetId')" != "null" ]; then
      BET_2ND_ID=$(echo "$RESP"|jq -r '.secondBetId')
      ACTIONS_2ND=$(echo "$RESP"|jq -r '.secondBetFollowAction')
    fi

  done

  #echo "BET_2ND_ID = $BET_2ND_ID // ACTIONS_2ND = $ACTIONS_2ND"
  if [ "$BET_2ND_ID" != "null" ]; then
    while [ "$ACTIONS_2ND" != "null" ]; do

      ACTIONS_2ND=$(echo "$ACTIONS_2ND" | jq -r 'join(" or ")')
      read -p "For your 2nd bet, do you want to ${ACTIONS_2ND}? : " CMD

      if [ "$CMD" = "h" ]; then
        CMD=hit
      elif [ "$CMD" = "s" ]; then
        CMD=stand
      elif [ "$CMD" = "d" ]; then
        CMD=double
      fi

      RESP=$(curl -s $SERVER_BASE/game/${GAME_ID}/bet/${BET_2ND_ID}/$CMD -X POST)

      ACTIONS_2ND=$(echo "$RESP"|jq -r '.followActions')
      if [ "$ACTIONS_2ND" != "null" ] || [ "$(echo "$RESP" | jq 'length')" != "1" ]; then
        echo "$RESP"|jq
      fi
      
    done
  fi

  END_RESP=$(curl -s $SERVER_BASE/game/${GAME_ID}/bet/${BET_ID})
  echo "$END_RESP"|jq

  if [ "$BET_2ND_ID" != "null" ]; then
    END_RESP=$(curl -s $SERVER_BASE/game/${GAME_ID}/bet/${BET_2ND_ID})
    echo "$END_RESP"|jq
  fi

  CASH=$(curl -s $SERVER_BASE/player/${PLAYER_ID}|jq -r '.cash')
  echo "Your cash: $CASH"
done
