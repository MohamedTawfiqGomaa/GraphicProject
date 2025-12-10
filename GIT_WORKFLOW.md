# 🔄 دليل سير عمل Git - Git Workflow Guide

[English](#english) | [العربية](#arabic)

---

<a name="english"></a>
## English

### 📚 Table of Contents

1. [Initial Setup](#initial-setup)
2. [Daily Workflow](#daily-workflow)
3. [Working with Branches](#working-with-branches)
4. [Handling Conflicts](#handling-conflicts)
5. [Best Practices](#best-practices)

---

### 🚀 Initial Setup

#### 1. Clone the Repository

```bash
git clone https://github.com/your-username/projectt.git
cd projectt
```

#### 2. Configure Git (if not already done)

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

#### 3. Add Upstream Remote (if you forked)

```bash
git remote add upstream https://github.com/original-owner/projectt.git
```

#### 4. Verify Remotes

```bash
git remote -v
```

---

### 📝 Daily Workflow

#### Starting Work

1. **Update your local main branch:**
   ```bash
   git checkout main
   git pull origin main
   ```

2. **Create a new branch for your work:**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/bug-description
   ```

#### Making Changes

1. **Edit files**
2. **Check status:**
   ```bash
   git status
   ```

3. **Stage changes:**
   ```bash
   git add <file>
   # or to add all changes
   git add .
   ```

4. **Commit changes:**
   ```bash
   git commit -m "feat: Add new feature description"
   ```

5. **Push to your branch:**
   ```bash
   git push origin feature/your-feature-name
   ```

#### Finishing Work

1. **Update your branch with latest main:**
   ```bash
   git checkout main
   git pull origin main
   git checkout feature/your-feature-name
   git merge main
   # or
   git rebase main
   ```

2. **Push final changes:**
   ```bash
   git push origin feature/your-feature-name
   ```

3. **Create Pull Request on GitHub**

4. **After PR is merged, clean up:**
   ```bash
   git checkout main
   git pull origin main
   git branch -d feature/your-feature-name
   ```

---

### 🌿 Working with Branches

#### Branch Naming Convention

- `feature/description` - New features
- `fix/description` - Bug fixes
- `docs/description` - Documentation updates
- `refactor/description` - Code refactoring
- `test/description` - Adding tests

#### Common Branch Commands

```bash
# List all branches
git branch

# List all branches (including remote)
git branch -a

# Create new branch
git checkout -b branch-name

# Switch to existing branch
git checkout branch-name

# Delete local branch
git branch -d branch-name

# Delete remote branch
git push origin --delete branch-name
```

---

### ⚠️ Handling Conflicts

#### When Conflicts Occur

1. **During merge:**
   ```bash
   git checkout main
   git pull origin main
   git checkout your-branch
   git merge main
   # Conflicts will be shown
   ```

2. **Edit conflicted files:**
   - Look for conflict markers: `<<<<<<<`, `=======`, `>>>>>>>`
   - Resolve conflicts manually
   - Remove conflict markers

3. **After resolving:**
   ```bash
   git add <resolved-files>
   git commit -m "fix: Resolve merge conflicts"
   git push origin your-branch
   ```

#### Conflict Resolution Tips

- Communicate with team members
- Test after resolving conflicts
- Keep both changes when appropriate
- Ask for help if unsure

---

### ✅ Best Practices

#### Commits

- ✅ Make small, focused commits
- ✅ Write clear commit messages
- ✅ One logical change per commit
- ❌ Don't commit broken code
- ❌ Don't commit unnecessary files

#### Branching

- ✅ Create branches for each feature/fix
- ✅ Keep branches up to date
- ✅ Delete branches after merging
- ❌ Don't work directly on main/master

#### Pull Requests

- ✅ Keep PRs small and focused
- ✅ Write clear descriptions
- ✅ Request reviews from teammates
- ✅ Respond to feedback promptly
- ❌ Don't merge your own PRs (let teammate review)

#### Communication

- ✅ Update team on your progress
- ✅ Ask questions when stuck
- ✅ Share knowledge with team
- ✅ Code review constructively

---

### 🔍 Useful Git Commands

```bash
# View commit history
git log
git log --oneline --graph --all

# View changes
git diff
git diff --staged

# Undo changes
git restore <file>           # Discard working directory changes
git restore --staged <file>  # Unstage file

# View remote branches
git branch -r

# Fetch latest changes without merging
git fetch origin

# See what changed
git show <commit-hash>
git diff HEAD~1 HEAD

# Stash changes temporarily
git stash
git stash pop
git stash list
```

---

### 📋 Common Workflows

#### Scenario 1: Starting New Feature

```bash
git checkout main
git pull origin main
git checkout -b feature/add-sound-effects
# Make changes
git add .
git commit -m "feat: Add background music"
git push origin feature/add-sound-effects
# Create PR on GitHub
```

#### Scenario 2: Fixing a Bug

```bash
git checkout main
git pull origin main
git checkout -b fix/collision-detection-bug
# Fix bug
git add .
git commit -m "fix: Correct collision detection algorithm"
git push origin fix/collision-detection-bug
# Create PR on GitHub
```

#### Scenario 3: Updating Your Branch

```bash
git checkout your-branch
git fetch origin
git merge origin/main
# Resolve conflicts if any
git push origin your-branch
```

---

<a name="arabic"></a>
## العربية

### 📚 المحتويات

1. [الإعداد الأولي](#الإعداد-الأولي)
2. [سير العمل اليومي](#سير-العمل-اليومي)
3. [العمل مع الفروع](#العمل-مع-الفروع)
4. [التعامل مع التعارضات](#التعامل-مع-التعارضات)
5. [أفضل الممارسات](#أفضل-الممارسات)

---

### 🚀 الإعداد الأولي

#### 1. استنساخ المستودع

```bash
git clone https://github.com/اسم-المستخدم-الخاص-بك/projectt.git
cd projectt
```

#### 2. تكوين Git (إذا لم يتم ذلك بالفعل)

```bash
git config --global user.name "اسمك"
git config --global user.email "بريدك@example.com"
```

#### 3. إضافة المستودع الأصلي (إذا قمت بعمل Fork)

```bash
git remote add upstream https://github.com/المالك-الأصلي/projectt.git
```

#### 4. التحقق من المستودعات البعيدة

```bash
git remote -v
```

---

### 📝 سير العمل اليومي

#### بدء العمل

1. **حدّث فرع main المحلي:**
   ```bash
   git checkout main
   git pull origin main
   ```

2. **أنشئ فرعاً جديداً لعملك:**
   ```bash
   git checkout -b feature/اسم-الميزة-الخاصة-بك
   # أو
   git checkout -b fix/وصف-الخطأ
   ```

#### إجراء التغييرات

1. **عدّل الملفات**
2. **تحقق من الحالة:**
   ```bash
   git status
   ```

3. **أضف التغييرات للتحضير:**
   ```bash
   git add <الملف>
   # أو لإضافة كل التغييرات
   git add .
   ```

4. **ألزم التغييرات:**
   ```bash
   git commit -m "feat: إضافة وصف الميزة الجديدة"
   ```

5. **ارفع إلى فرعك:**
   ```bash
   git push origin feature/اسم-الميزة-الخاصة-بك
   ```

#### إنهاء العمل

1. **حدّث فرعك بأحدث main:**
   ```bash
   git checkout main
   git pull origin main
   git checkout feature/اسم-الميزة-الخاصة-بك
   git merge main
   # أو
   git rebase main
   ```

2. **ارفع التغييرات النهائية:**
   ```bash
   git push origin feature/اسم-الميزة-الخاصة-بك
   ```

3. **أنشئ طلب سحب على GitHub**

4. **بعد دمج PR، نظّف:**
   ```bash
   git checkout main
   git pull origin main
   git branch -d feature/اسم-الميزة-الخاصة-بك
   ```

---

### 🌿 العمل مع الفروع

#### اصطلاح تسمية الفروع

- `feature/الوصف` - ميزات جديدة
- `fix/الوصف` - إصلاحات الأخطاء
- `docs/الوصف` - تحديثات التوثيق
- `refactor/الوصف` - إعادة هيكلة الكود
- `test/الوصف` - إضافة اختبارات

#### أوامر الفروع الشائعة

```bash
# عرض جميع الفروع
git branch

# عرض جميع الفروع (بما في ذلك البعيدة)
git branch -a

# إنشاء فرع جديد
git checkout -b اسم-الفرع

# التبديل إلى فرع موجود
git checkout اسم-الفرع

# حذف فرع محلي
git branch -d اسم-الفرع

# حذف فرع بعيد
git push origin --delete اسم-الفرع
```

---

### ⚠️ التعامل مع التعارضات

#### عند حدوث التعارضات

1. **أثناء الدمج:**
   ```bash
   git checkout main
   git pull origin main
   git checkout فرعك
   git merge main
   # سيتم عرض التعارضات
   ```

2. **عدّل الملفات المتعارضة:**
   - ابحث عن علامات التعارض: `<<<<<<<`, `=======`, `>>>>>>>`
   - حل التعارضات يدوياً
   - احذف علامات التعارض

3. **بعد الحل:**
   ```bash
   git add <الملفات-المحلولة>
   git commit -m "fix: حل تعارضات الدمج"
   git push origin فرعك
   ```

#### نصائح حل التعارضات

- تواصل مع أعضاء الفريق
- اختبر بعد حل التعارضات
- احتفظ بكلا التغييرين عند الحاجة
- اطلب المساعدة إذا كنت غير متأكد

---

### ✅ أفضل الممارسات

#### الالتزامات (Commits)

- ✅ قم بعمل التزامات صغيرة ومركزة
- ✅ اكتب رسائل التزام واضحة
- ✅ تغيير منطقي واحد لكل التزام
- ❌ لا تلتزم بكود معطل
- ❌ لا تلتزم بملفات غير ضرورية

#### الفروع

- ✅ أنشئ فروع لكل ميزة/إصلاح
- ✅ حافظ على الفروع محدثة
- ✅ احذف الفروع بعد الدمج
- ❌ لا تعمل مباشرة على main/master

#### طلبات السحب (Pull Requests)

- ✅ حافظ على PRs صغيرة ومركزة
- ✅ اكتب أوصاف واضحة
- ✅ اطلب مراجعات من زملائك
- ✅ رد على الملاحظات بسرعة
- ❌ لا تدمج PRs الخاصة بك (دع زميلك يراجع)

#### التواصل

- ✅ حدّث الفريق عن تقدمك
- ✅ اسأل الأسئلة عند الحيرة
- ✅ شارك المعرفة مع الفريق
- ✅ راجع الكود بشكل بناء

---

### 🔍 أوامر Git مفيدة

```bash
# عرض تاريخ الالتزامات
git log
git log --oneline --graph --all

# عرض التغييرات
git diff
git diff --staged

# التراجع عن التغييرات
git restore <الملف>           # تجاهل تغييرات دليل العمل
git restore --staged <الملف>  # إلغاء تحضير الملف

# عرض الفروع البعيدة
git branch -r

# جلب أحدث التغييرات دون الدمج
git fetch origin

# رؤية ما تغير
git show <رمز-الالتزام>
git diff HEAD~1 HEAD

# تخزين التغييرات مؤقتاً
git stash
git stash pop
git stash list
```

---

### 📋 سيناريوهات شائعة

#### السيناريو 1: بدء ميزة جديدة

```bash
git checkout main
git pull origin main
git checkout -b feature/إضافة-المؤثرات-الصوتية
# قم بإجراء التغييرات
git add .
git commit -m "feat: إضافة موسيقى خلفية"
git push origin feature/إضافة-المؤثرات-الصوتية
# أنشئ PR على GitHub
```

#### السيناريو 2: إصلاح خطأ

```bash
git checkout main
git pull origin main
git checkout -b fix/خطأ-كشف-التصادم
# أصلح الخطأ
git add .
git commit -m "fix: تصحيح خوارزمية كشف التصادم"
git push origin fix/خطأ-كشف-التصادم
# أنشئ PR على GitHub
```

#### السيناريو 3: تحديث فرعك

```bash
git checkout فرعك
git fetch origin
git merge origin/main
# حل التعارضات إن وجدت
git push origin فرعك
```

---

## 🎯 نصائح إضافية

### للمبتدئين

- ابدأ بميزات صغيرة
- اطلب المساعدة عند الحاجة
- تعلم من مراجعات الكود
- تدرب على حل التعارضات

### للفريق

- تواصل بانتظام
- راجع PRs بسرعة
- شارك المعرفة
- احتفظ بالتوثيق محدثاً

---

**ملاحظة:** إذا واجهت أي مشكلة، لا تتردد في السؤال! الفريق موجود للمساعدة. 🚀

