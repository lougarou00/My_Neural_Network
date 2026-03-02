#!/usr/bin/env sh

# abort on errors
set -e

# build
npm run docs:build

# navigate into the build output directory
cd docs/.vitepress/dist

# si tu utilises un custom domain, crée un fichier CNAME ici

git init
git add -A
git commit -m 'deploy'
git branch -M main

# push to gh-pages branch
git push -f git@github.com:EpitechPGE3-2025/G-CPP-500-COT-5-1-rtype-22.git main:gh-pages

cd -
