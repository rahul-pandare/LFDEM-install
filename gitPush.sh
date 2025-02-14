#!/bin/bash

#git pull origin master --rebase  # Pulls the latest changes from GitHub to avoid conflicts
git add .                        # Stages all new and modified files
git commit -m "Updated files"    # Commits the changes with a fixed message
git push origin master           # Pushes changes to the master branch

echo "✅ Changes successfully pushed to GitHub!"  # Displays a success message