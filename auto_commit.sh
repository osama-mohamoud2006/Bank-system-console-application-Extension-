#!/bin/bash

# ==============================
# ⚙️ إعداد المتغيرات
# ==============================
project_dir="E:/projects/c++ course/08 - Algorithms & Problem Solving Level 4/Project 1"
snapshot_dir="E:/projects/_snapshots_level_four"
repo_url="https://github.com/osama-mohamoud2006/Bank-system-console-application-Extension-"
branch="main"

# 🔁 الدخول على مجلد المشروع
cd "$project_dir" || exit

# 🗓️ إنشاء أو قراءة تاريخ الإنشاء
created_file=".created_at"
if [ ! -f "$created_file" ]; then
    created_date=$(date '+%Y-%m-%d_%H-%M-%S')
    echo "$created_date" > "$created_file"
    echo "🆕 Created project date: $created_date"
else
    created_date=$(cat "$created_file")
    echo "📅 Project originally created at: $created_date"
fi

# 🧷 إنشاء نسخة Snapshot لو مش موجودة
snapshot_path="$snapshot_dir/$created_date"
if [ ! -d "$snapshot_path" ]; then
    mkdir -p "$snapshot_path"
    cp -r "$project_dir" "$snapshot_path"
    echo "📁 Snapshot created at: $snapshot_path"
else
    echo "📦 Snapshot already exists at: $snapshot_path"
fi

# ⛔ التأكد إن .gitignore موجود وفيه .vs/
gitignore_file=".gitignore"
if [ ! -f "$gitignore_file" ]; then
    echo ".vs/" > "$gitignore_file"
else
    grep -qxF ".vs/" "$gitignore_file" || echo ".vs/" >> "$gitignore_file"
fi

# 🔥 إزالة .vs/ من Git index لو متتبع
if git ls-files --error-unmatch ".vs/" > /dev/null 2>&1; then
    git rm -r --cached .vs/
fi

# 🧠 Git operations
git remote set-url origin "$repo_url"
git pull origin "$branch"

git add -u
git add .

commit_msg="Auto commit: $(date '+%Y-%m-%d %H:%M:%S')"
git commit -m "$commit_msg" --quiet

git push origin "$branch" --quiet
