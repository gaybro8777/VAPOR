#!/bin/sh

ROOT_DIR=$(git rev-parse --show-toplevel)
HOOK_DIR=$ROOT_DIR/.git/hooks
HOOK=$HOOK_DIR/pre-commit
ln -sf $ROOT_DIR/share/gitHooks/pre-commit $HOOK

if [ -e "$HOOK" ]
then
    echo "pre-commit hook installed in $HOOK_DIR"
else
    echo "Failure: Unable to create sym-link in $HOOK_DIR"
fi
