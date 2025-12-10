# 🤝 دليل المساهمة - Contributing Guidelines

[English](#english) | [العربية](#arabic)

---

<a name="english"></a>
## English

Thank you for considering contributing to the Fish Game project! This document provides guidelines and instructions for contributing.

### 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How to Contribute](#how-to-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Commit Messages](#commit-messages)
- [Pull Request Process](#pull-request-process)

---

### 📜 Code of Conduct

- Be respectful and inclusive
- Welcome newcomers and help them learn
- Provide constructive feedback
- Focus on what is best for the project

---

### 🚀 How to Contribute

#### Reporting Bugs

1. Check if the bug has already been reported in [Issues](../../issues)
2. Create a new issue with:
   - Clear title and description
   - Steps to reproduce
   - Expected vs actual behavior
   - System information (OS, compiler, etc.)

#### Suggesting Features

1. Check existing feature requests in [Issues](../../issues)
2. Create a new issue with:
   - Clear description of the feature
   - Use case and benefits
   - Possible implementation approach

#### Contributing Code

1. **Fork the repository**
2. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/your-bug-fix
   ```
3. **Make your changes**
4. **Test your changes**
5. **Commit your changes** (follow commit message guidelines)
6. **Push to your fork:**
   ```bash
   git push origin feature/your-feature-name
   ```
7. **Create a Pull Request**

---

### 🛠️ Development Setup

1. Clone your fork:
   ```bash
   git clone https://github.com/your-username/projectt.git
   cd projectt
   ```

2. Add upstream remote:
   ```bash
   git remote add upstream https://github.com/original-owner/projectt.git
   ```

3. Install dependencies (see README.md)

4. Build and test:
   ```bash
   # Using Code::Blocks or your preferred IDE
   # Or using command line (see README.md)
   ```

---

### 📝 Coding Standards

#### General Guidelines

- Follow existing code style and structure
- Write clear, readable code
- Comment complex logic
- Keep functions focused and small
- Use meaningful variable and function names

#### C++ Style

- Use camelCase for functions and variables
- Use UPPER_CASE for constants
- Add comments for non-obvious code
- Keep lines under 100 characters when possible
- Use consistent indentation (spaces or tabs, but be consistent)

#### Example:

```cpp
// Good
const int MAX_FISH_COUNT = 50;
void drawFish(const Fish& fish) {
    // Clear implementation
}

// Avoid
int mfc = 50;  // Unclear abbreviation
void df(Fish f) { }  // Unclear names
```

---

### 💬 Commit Messages

Write clear, descriptive commit messages:

#### Format:

```
<type>: <short description>

<optional longer description>

<optional references>
```

#### Types:

- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation changes
- `style:` - Code style changes (formatting, etc.)
- `refactor:` - Code refactoring
- `test:` - Adding or updating tests
- `chore:` - Maintenance tasks

#### Examples:

```
feat: Add power-up fish types

Adds three new power-up fish types: speed boost, 
slow motion, and extra time. Includes collision 
detection and visual effects.

fix: Resolve collision detection edge case

Fixes issue where fast-moving fish could pass 
through each other. Improves collision algorithm 
with better time-step handling.

docs: Update README with build instructions

Adds detailed build instructions for Windows, 
Linux, and macOS platforms.
```

---

### 🔄 Pull Request Process

1. **Update your branch:**
   ```bash
   git checkout main
   git pull upstream main
   git checkout your-feature-branch
   git merge main
   ```

2. **Ensure your code:**
   - Compiles without errors
   - Follows coding standards
   - Includes necessary comments
   - Has been tested

3. **Create Pull Request:**
   - Clear title and description
   - Reference related issues
   - Describe changes made
   - Include screenshots if UI changes

4. **Respond to feedback:**
   - Be open to suggestions
   - Make requested changes
   - Keep discussion focused and respectful

---

### ✅ Checklist Before Submitting

- [ ] Code compiles without warnings
- [ ] Code follows project style guidelines
- [ ] Comments added for complex logic
- [ ] Changes tested on your system
- [ ] Commit messages are clear and descriptive
- [ ] Branch is up to date with main
- [ ] No unnecessary files included

---

### 📚 Resources

- [Git Documentation](https://git-scm.com/doc)
- [GitHub Flow Guide](https://guides.github.com/introduction/flow/)
- [C++ Best Practices](https://github.com/cpp-best-practices/cppbestpractices)

---

<a name="arabic"></a>
## العربية

شكراً لك على التفكير في المساهمة في مشروع لعبة السمك! يوفر هذا المستند إرشادات وتعليمات للمساهمة.

### 📋 المحتويات

- [قواعد السلوك](#قواعد-السلوك)
- [كيفية المساهمة](#كيفية-المساهمة)
- [إعداد التطوير](#إعداد-التطوير)
- [معايير البرمجة](#معايير-البرمجة)
- [رسائل الالتزام](#رسائل-الالتزام)
- [عملية طلب السحب](#عملية-طلب-السحب)

---

### 📜 قواعد السلوك

- كن محترماً وشاملاً
- رحّب بالوافدين الجدد وساعدهم على التعلم
- قدم ملاحظات بناءة
- ركّز على ما هو أفضل للمشروع

---

### 🚀 كيفية المساهمة

#### الإبلاغ عن الأخطاء

1. تحقق مما إذا تم الإبلاغ عن الخطأ بالفعل في [المشاكل](../../issues)
2. أنشئ مشكلة جديدة تتضمن:
   - عنوان ووصف واضحان
   - خطوات إعادة الإنتاج
   - السلوك المتوقع مقابل الفعلي
   - معلومات النظام (OS، المترجم، إلخ)

#### اقتراح الميزات

1. تحقق من طلبات الميزات الموجودة في [المشاكل](../../issues)
2. أنشئ مشكلة جديدة تتضمن:
   - وصف واضح للميزة
   - حالة الاستخدام والفوائد
   - نهج التطبيق المحتمل

#### المساهمة بالكود

1. **قم بعمل Fork للمستودع**
2. **أنشئ فرع ميزة:**
   ```bash
   git checkout -b feature/اسم-الميزة-الخاصة-بك
   # أو
   git checkout -b fix/إصلاح-الخطأ-الخاص-بك
   ```
3. **قم بإجراء تغييراتك**
4. **اختبر تغييراتك**
5. **ألزم تغييراتك** (اتبع إرشادات رسائل الالتزام)
6. **ارفع إلى Fork الخاص بك:**
   ```bash
   git push origin feature/اسم-الميزة-الخاصة-بك
   ```
7. **أنشئ طلب سحب (Pull Request)**

---

### 🛠️ إعداد التطوير

1. استنسخ Fork الخاص بك:
   ```bash
   git clone https://github.com/اسم-المستخدم-الخاص-بك/projectt.git
   cd projectt
   ```

2. أضف المستودع الأصلي:
   ```bash
   git remote add upstream https://github.com/المالك-الأصلي/projectt.git
   ```

3. ثبّت التبعيات (انظر README.md)

4. البناء والاختبار:
   ```bash
   # باستخدام Code::Blocks أو بيئة التطوير المفضلة لديك
   # أو باستخدام سطر الأوامر (انظر README.md)
   ```

---

### 📝 معايير البرمجة

#### الإرشادات العامة

- اتبع نمط وهيكل الكود الموجود
- اكتب كود واضح وقابل للقراءة
- علّق على المنطق المعقد
- حافظ على التركيز في الدوال وصغر حجمها
- استخدم أسماء متغيرات ودوال ذات معنى

#### نمط C++

- استخدم camelCase للدوال والمتغيرات
- استخدم UPPER_CASE للثوابت
- أضف تعليقات للكود غير الواضح
- حافظ على السطور تحت 100 حرف عند الإمكان
- استخدم مسافة بادئة متناسقة (مسافات أو علامات تبويب)

#### مثال:

```cpp
// جيد
const int MAX_FISH_COUNT = 50;
void drawFish(const Fish& fish) {
    // تنفيذ واضح
}

// تجنب
int mfc = 50;  // اختصار غير واضح
void df(Fish f) { }  // أسماء غير واضحة
```

---

### 💬 رسائل الالتزام

اكتب رسائل التزام واضحة ووصفية:

#### التنسيق:

```
<النوع>: <وصف قصير>

<وصف أطول اختياري>

<مراجع اختيارية>
```

#### الأنواع:

- `feat:` - ميزة جديدة
- `fix:` - إصلاح خطأ
- `docs:` - تغييرات التوثيق
- `style:` - تغييرات نمط الكود (تنسيق، إلخ)
- `refactor:` - إعادة هيكلة الكود
- `test:` - إضافة أو تحديث الاختبارات
- `chore:` - مهام الصيانة

#### أمثلة:

```
feat: إضافة أنواع أسماك معززة

يضيف ثلاثة أنواع أسماك معززة جديدة: زيادة السرعة، 
حركة بطيئة، ووقت إضافي. يتضمن كشف التصادم 
والمؤثرات البصرية.

fix: حل مشكلة حالة حافة كشف التصادم

يحل مشكلة حيث يمكن للأسماك سريعة الحركة أن تمر 
خلال بعضها البعض. يحسّن خوارزمية التصادم 
مع معالجة أفضل للخطوات الزمنية.

docs: تحديث README مع تعليمات البناء

يضيف تعليمات بناء مفصلة لمنصات Windows، 
Linux، و macOS.
```

---

### 🔄 عملية طلب السحب

1. **حدّث فرعك:**
   ```bash
   git checkout main
   git pull upstream main
   git checkout فرع-الميزة-الخاص-بك
   git merge main
   ```

2. **تأكد من أن كودك:**
   - يترجم بدون أخطاء
   - يتبع معايير البرمجة
   - يتضمن التعليقات اللازمة
   - تم اختباره

3. **أنشئ طلب سحب:**
   - عنوان ووصف واضحان
   - راجع المشاكل ذات الصلة
   - صف التغييرات المُجراة
   - أدرج لقطات الشاشة إذا كانت هناك تغييرات في الواجهة

4. **رد على الملاحظات:**
   - كن منفتحاً على الاقتراحات
   - قم بإجراء التغييرات المطلوبة
   - حافظ على التركيز والاحترام في المناقشة

---

### ✅ قائمة التحقق قبل الإرسال

- [ ] الكود يترجم بدون تحذيرات
- [ ] الكود يتبع إرشادات نمط المشروع
- [ ] تمت إضافة تعليقات للمنطق المعقد
- [ ] تم اختبار التغييرات على نظامك
- [ ] رسائل الالتزام واضحة ووصفية
- [ ] الفرع محدث مع main
- [ ] لا توجد ملفات غير ضرورية مضمّنة

---

### 📚 الموارد

- [توثيق Git](https://git-scm.com/doc)
- [دليل GitHub Flow](https://guides.github.com/introduction/flow/)
- [أفضل ممارسات C++](https://github.com/cpp-best-practices/cppbestpractices)

---

شكراً لمساهمتك! 🎉

