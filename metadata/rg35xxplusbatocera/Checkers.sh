export PORTS_FOLDER=$(realpath $(dirname ${BASH_SOURCE[0]}))
export GAMEDIR="${PORTS_FOLDER}/checkers"
export HOME="/userdata/system"
export SDL_ASSERT=always_ignore
export SDL_GAMECONTROLLERCONFIG=$(grep "Deeplay" "${HOME}/.config/gamecontrollerdb.txt")

chmod +x $GAMEDIR/game

cd $GAMEDIR
./game -f
