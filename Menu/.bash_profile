# Script executado no login, define o menu de jogos

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022

# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi

# set PATH so it includes user's private bin if it exists
#if [ -d "$HOME/bin" ] ; then
#    PATH="$HOME/bin:$PATH"
#fi

# apresentacao inicial
clear
echo "R E T R O S E R V E R"
echo
echo "Viva a experiencia dos jogos antigos."
echo
echo "Por favor, use o sistema de forma responsavel"
echo
echo

# pede um nome de usuario para o diretorio de save
VALIDO="^[0-9a-zA-Z]+$"
while true; do
  echo -n "Informe o seu nome: "
  read nome
  if [[ "$nome" =~ $VALIDO ]]; then
    if [ -d "$nome" ]; then
      cd $nome
      break
    elif mkdir $nome; then
      cd $nome
      break
    fi
  else
    echo "Nome invalido!"
  fi
done

# Menu dos jogos
while true; do
  clear
  ADVENTURE=/usr/local/games/adventure.data
  export ADVENTURE
  PS3='Selecione uma opcao: '
  options=("Adventure" "Dragon Island" "Oregon Trail" "Pirate Adventure" \
    "Super Star Trek", "Logout")
  select opt in "${options[@]}"
  do
    clear
    case $REPLY in
        "1")
            adventure
            break
            ;;
        "2")
            exec yabasic /usr/local/games/dragon.bas
            break
            ;;
        "3")
            exec yabasic /usr/local/games/oregon.bas
            break
            ;;
        "4")
            exec yabasic /usr/local/games/piradv.bas
            break
            ;;
        "5")
            startrek
            break
            ;;
        "6")
            logout
            ;;
        *) echo "Opcao invalida!"
           ;;
    esac
  done 
done
