#!/bin/bash

pull_request=$(echo "$GITHUB_CONTEXT" | jq -r '.event.pull_request')
if [ "$pull_request" != null ]; then
  # Not in a pull request, so compare against parent commit
  #base_commit="HEAD^"
  #base_commit=`git rev-parse --verify HEAD`
  #origin_commit=`git rev-parse --verify origin/HEAD`

  #origin_commit=`git rev-parse --verify HEAD^`
  echo "this is pull request, run git-clang-format"
  origin_commit=$(echo "$GITHUB_CONTEXT" | jq -r '.event.pull_request.base.sha')
  current_commit=$(echo "$GITHUB_CONTEXT" | jq -r '.event.pull_request.head.sha')
  echo $origin_commit
  echo $current_commit
  #echo "$GITHUB_SHA"
  echo "Running clang-format against parent commit $origin_commit, and $current_commit"
else
  #base_commit="$(git merge-base "${TRAVIS_BRANCH}" HEAD)"
  echo "this is not pull request"
  exit 0
fi
#exclude_regex="(.*thirdparty/|.*redismodule.h|.*.java|.*.jsx?|.*.tsx?)"
output="$(./git-clang-format --binary clang-format --commit "$current_commit" "$origin_commit" --diff)"
if [ "$output" = "no modified files to format" ] || [ "$output" = "clang-format did not modify any files" ] ; then
  echo "clang-format passed."
  exit 0
else
  echo "clang-format failed:"
  echo "$output"
  exit 1
fi
