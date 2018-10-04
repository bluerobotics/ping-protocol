bold=$(tput bold)
normal=$(tput sgr0)

echob() {
    echo "${bold}${1}${normal}"
}

printfb() {
    printf "${bold}${1}${normal}"
}

error() {
    echo -e "ERROR: $*" >&2
    exit 1
}

usage() {
    echo "USAGE: $scriptname --no-deploy, --debug, --help"
}

checktool() {
    name=""
    errormsg=""
    if (( $# > 1 )); then
        name=$2
        if (( $# >= 3 )); then
            errormsg=$3
        fi
    else
        name=( $1 )
    fi
    printfb "$name: "
    eval "$1" >/dev/null 2>&1

    [ $? == 0 ] || {
        echob "✖"
        error "$name is not available." $errormsg
    }
    echob "✔"
}

runstep() {
    name=$1
    okmessage=$2
    errormsg=$3
    printfb "$okmessage: "
    eval "$1" >/dev/null 2>&1
    [ $? == 0 ] || {
        echob "✖"
        error "$name failed." $errormsg
    }
    echob "✔"
}
