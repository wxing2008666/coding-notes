#!/bin/bash

if git rev-parse --verify HEAD >/dev/null 2>&1; then
  # Not in a pull request, so compare against parent commit
  #base_commit="HEAD^"
  #base_commit=`git rev-parse --verify HEAD`
  #origin_commit=`git rev-parse --verify origin/HEAD`

  current_commit=`git rev-parse --verify HEAD`
  #origin_commit=$(echo "$GITHUB_CONTEXT" | jq -r '.event.pull_request.base.sha')
  #current_commit=$(echo "$GITHUB_CONTEXT" | jq -r '.sha')
  #echo $origin_commit
  echo $current_commit
  #echo "$GITHUB_SHA"
  echo "Running clang-format against parent commit $origin_commit, and $current_commit"
else
  #base_commit="$(git merge-base "${TRAVIS_BRANCH}" HEAD)"
  origin_commit=4b825dc642cb6eb9a060e54bf8d69288fbee4904
  current_commit=4b825dc642cb6eb9a060e54bf8d69288fbee4904
  echo "Running clang-format against branch $current_commit, with hash $(git rev-parse "$current_commit")"
fi
#exclude_regex="(.*thirdparty/|.*redismodule.h|.*.java|.*.jsx?|.*.tsx?)"
output="$(./git-clang-format --binary clang-format --commit "$current_commit" --diff)"
if [ "$output" = "no modified files to format" ] || [ "$output" = "clang-format did not modify any files" ] ; then
  echo "clang-format passed."
  exit 0
else
  echo "clang-format failed:"
  echo "$output"
  exit 1
fi
