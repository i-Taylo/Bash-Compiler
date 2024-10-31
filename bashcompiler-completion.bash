#!/bin/bash

_bashcompiler_completion() {
    local cur opts
    cur="${COMP_WORDS[COMP_CWORD]}"
    opts="--optim --help --version"

    COMPREPLY=($(compgen -W "${opts}" -- ${cur}))
    return 0
}

complete -F _bashcompiler_completion bashcompiler
