#!/bin/bash

#if [ -z "${TRAVIS_PULL_REQUEST-}" ] || [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
if git rev-parse --verify HEAD >/dev/null 2>&1; then
  # Not in a pull request, so compare against parent commit
  #base_commit="HEAD^"
  base_commit=`git rev-parse --verify HEAD`
  echo "Running clang-format against parent commit $base_commit"
else
  #base_commit="$(git merge-base "${TRAVIS_BRANCH}" HEAD)"
  base_commit=4b825dc642cb6eb9a060e54bf8d69288fbee4904
  echo "Running clang-format against branch $base_commit, with hash $(git rev-parse "$base_commit")"
fi
#exclude_regex="(.*thirdparty/|.*redismodule.h|.*.java|.*.jsx?|.*.tsx?)"
output="$(./git-clang-format --binary /usr/bin/clang-format --commit "$base_commit" --diff)"
if [ "$output" = "no modified files to format" ] || [ "$output" = "clang-format did not modify any files" ] ; then
  echo "clang-format passed."
  exit 0
else
  echo "clang-format failed:"
  echo "$output"
  exit 1
fi
