# Conventional Commits

This document defines our team's commit message convention based on the Conventional Commits specification.

## Commit message format

Format:

```
type(optional-scope): short description
```

## Components

- `type`: one of the allowed types (see below).
- `optional-scope`: short area or module (no spaces), e.g. `auth`, `ui`, `api`.
- `short description` (subject): imperative, about 72 characters or fewer, no trailing period.

## Allowed types and descriptions

- `feat`: a new feature for the user
- `fix`: a bug fix
- `docs`: documentation only changes
- `style`: formatting (whitespace, semicolons, etc.) — no code logic changes
- `refactor`: code change that neither fixes a bug nor adds a feature
- `perf`: a code change that improves performance
- `build`: changes that affect the build system or external dependencies (CMakeLists, etc.)
- `chore`: other changes that don’t modify src or test files (e.g., tooling updates)
- `revert`: reverts a previous commit (include the reverted commit hash in the body)

## Issue referencing

When a commit completes an issue, use one of these keywords in the commit message body or footer:

```
closes #X
fixes #X
resolves #X
```

If the commit only references an issue but does not complete it, put the reference in parentheses after the subject:

```
feat(auth): add login form (#3)
```

## Examples

Valid commit message examples:

```
feat(auth): add login form, closes #3
fix(api): handle null response, fixes #12
feat(ui): add dark mode toggle, closes #17
fix(parser): avoid crash on empty input, fixes #2
docs: add API usage examples
perf: optimize image decoding
refactor(auth): extract token handler
```
