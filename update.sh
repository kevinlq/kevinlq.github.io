echo "project status and branch information:"
echo "git status && git branch -a"
git status && git branch -a

echo "project file upload......."
echo "git add. && git commit -m && git push "
git add . && git commit -m "更新" && git push
echo "upload finished!"

read -n 1