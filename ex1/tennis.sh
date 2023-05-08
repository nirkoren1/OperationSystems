#!/bin/bash
#Nir Koren 316443902

# This is a tennis paper game that is played by two players.

PLAYER1SCORE=50
PLAYER2SCORE=50
PLAYER1PICK=0
PLAYER2PICK=0
STATE=0
RUNNING=1


function print_board() {
  echo " --------------------------------- "
  echo " |       |       #       |       | "
  echo " |       |       #       |       | "
  # switch case to print the board according to the state of the game.
  case $STATE in
    3)
      echo " |       |       #       |       |O"
      shift 1
      ;;
    2)
      echo " |       |       #       |   O   | "
      shift 1
      ;;
    1)
      echo " |       |       #   O   |       | "
      shift 1
      ;;
    0)
      echo " |       |       O       |       | "
      shift 1
      ;;
    -1)
      echo " |       |   O   #       |       | "
      shift 1
      ;;
    -2)
      echo " |   O   |       #       |       | "
      shift 1
      ;;
    -3)
      echo "O|       |       #       |       | "
      shift 1
      ;;
  esac
  echo " |       |       #       |       | "
  echo " |       |       #       |       | "
  echo " --------------------------------- "
}

function print_score {
  # format the score to be printed.
  echo " Player 1: ${PLAYER1SCORE}         Player 2: ${PLAYER2SCORE} "
}

function is_runnig() {
  if [[ $PLAYER1SCORE -le 0 || $PLAYER2SCORE -le 0 ]]; then
    RUNNING=0
    return 0
  else
    RUNNING=1
  fi
  if [[ $STATE -eq 3 || $STATE -eq -3 ]]; then
    RUNNING=0
    return 0
  else
    RUNNING=1
  fi
}

function who_won() {
    # if state is 3 the player 1 won. if state is -3 the player 2 won.
    if [[ $STATE -eq 3 ]]; then
      return 1
    elif [[ $STATE -eq -3 ]]; then
      return 2
    fi
    balance=$((PLAYER1SCORE - PLAYER2SCORE))
    if [[ $balance -gt 0 ]]; then
      return 1
    elif [[ $balance -lt 0 ]]; then
      return 2
    elif [[ $balance -eq 0 ]]; then
      if [[ $STATE -gt 0 ]]; then
        return 1
      elif [[ $STATE -lt 0 ]]; then
        return 2
      fi
    fi
    return 0
}

function player_one_pick_number() {
  player_score=$PLAYER1SCORE
  not_valid=1
  while [[ $not_valid -eq 1 ]]; do
    echo "PLAYER 1 PICK A NUMBER: "
    read player_pick
    # check if the number is a number
    if [[ $player_pick == ^[0-9]+$ ]]; then
      echo "NOT A VALID MOVE !"
    elif [[ $player_pick -ge 1 && $player_pick -le player_score ]]; then
      not_valid=0
    else
      echo "NOT A VALID MOVE !"
    fi
  done
  PLAYER1PICK=$player_pick
}

function player_two_pick_number() {
  player_score=$PLAYER2SCORE
  not_valid=1
  while [[ $not_valid -eq 1 ]]; do
    echo "PLAYER 2 PICK A NUMBER: "
    read player_pick
    # check if the number is a number
    if [[ $player_pick == ^[0-9]+$ ]]; then
      echo "NOT A VALID MOVE !"
    elif [[ $player_pick -ge 1 && $player_pick -le player_score ]]; then
      not_valid=0
    else
      echo "NOT A VALID MOVE !"
    fi
  done
  PLAYER2PICK=$player_pick
}

function update_state_and_scores() {
  balance=$((PLAYER1PICK - PLAYER2PICK))
  if [[ $balance -gt 0 ]]; then
    if [[ $STATE -eq -1 ]]; then
      STATE=$((STATE + 2))
    else
      STATE=$((STATE + 1))
    fi
  elif [[ $balance -lt 0 ]]; then
    if [[ $STATE -eq 1 ]]; then
      STATE=$((STATE - 2))
    else
      STATE=$((STATE - 1))
    fi
  fi
  PLAYER1SCORE=$((PLAYER1SCORE - PLAYER1PICK))
  PLAYER2SCORE=$((PLAYER2SCORE - PLAYER2PICK))
}

is_runnig

while [ $RUNNING -eq 1 ]; do
  print_score
  print_board
  if [[ PLAYER1SCORE -ne 50 || PLAYER2SCORE -ne 50 ]]; then
    echo -e "       Player 1 played: ${PLAYER1PICK}\n       Player 2 played: ${PLAYER2PICK}\n\n"
  fi
  player_one_pick_number
  player_two_pick_number
  update_state_and_scores
  is_runnig
done

print_score
print_board
echo -e "       Player 1 played: ${PLAYER1PICK}\n       Player 2 played: ${PLAYER2PICK}\n\n"
who_won
winner=$?
if [[ $winner -eq 1 ]]; then
  echo "PLAYER 1 WINS !"
elif [[ $winner -eq 2 ]]; then
  echo "PLAYER 2 WINS !"
else
  echo "IT'S A DRAW !"
fi