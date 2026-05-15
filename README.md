<div align="center">

# Certa.dev

</div>

<div align="center">
  <img src="./assets/logo/Certa.dev_clean.png" height="200" alt="logo"  />
</div>

<img height="20">

<div align="center">

![Status](https://img.shields.io/badge/status-active-brightgreen)
![Last Commit](https://img.shields.io/github/last-commit/codingburgas/Certa.dev)
![Issues](https://img.shields.io/github/issues-closed/codingburgas/Certa.dev)

</div>

<div align="center">
  <img src="https://skillicons.dev/icons?i=qt" height="40" alt="qt logo"  />
  <img width="5">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" height="40" alt="cplusplus logo"  />
  <img width="5">
  <img src="https://skillicons.dev/icons?i=cmake" height="40" alt="cmake logo"  />
  <img width="5">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/sqlite/sqlite-original.svg" height="40" alt="sqlite logo"  />
</div>

## Table of Contents

- [Overview](#overview)
  - [Goals](#goals)
  - [How it works](#how-it-works)
  - [Architecture](#architecture)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Building](#building)
- [Documentation](#documentation)
- [Contributors](#contributors)

## Overview

**Certa.dev is a desktop application that lets users build a personal movie catalogue, discover new films, and receive recommendations based on their genre preferences.**

Developed as a 9th-grade computer science assignment, the project demonstrates practical skills in desktop GUI development, object-oriented C++, and relational database management. The team followed the Scrum methodology across 4 sprints, tracking all work on a GitHub Kanban board.

### Goals

- Build a working multi-screen desktop application with a clean, consistent dark UI
- Separate concerns cleanly using a three-tier architecture - presentation never touches SQL, data access never touches layout
- Implement core algorithms (sorting, searching, recursion) without relying on standard library helpers
- Manage the full development process using Scrum, conventional commits, and GitHub pull requests

### How it works

1. **Register** - create an account and select your genre interests; the app stores a PBKDF2-SHA256 password hash
2. **Browse** - explore the full movie and actor catalogue; sort by rating, year, name, or number of films
3. **Search** - type any word to find matching movies and actors in real time via a debounced linear search
4. **Discover** - open Recommendations to see films suggested from your selected genres
5. **Collect** - mark movies as favourites and revisit them from the Favourites screen

### Architecture

Certa.dev uses a strict three-tier architecture. Each layer may only call the one directly below it - the presentation layer never executes SQL, and the data access layer never constructs UI elements.

| Layer            | Directory            | Contents                                                                                                                                  |
| ---------------- | -------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| **Presentation** | `presentationLayer/` | Qt Widgets screens (`auth`, `home`, `movies`, `actors`, `search`, `favorites`, `recommendations`, `profile`, `settings`) and shared cards |
| **Service**      | `serviceLayer/`      | Business logic (`authService`, `movieService`, `actorService`, `userService`), PBKDF2 password hashing, `UserSession` singleton           |
| **Data Access**  | `dataAccessLayer/`   | `Database` singleton + repositories (`authRepository`, `userRepository`, `movieRepository`, `actorRepository`); all SQL lives here        |
| **Models**       | `models/`            | Plain data-transfer structs (`user`, `auth`, `movie`, `actor`) - no logic, no SQL, no Qt signals                                          |

## Getting Started

### Prerequisites

- **Qt 6.5 or later** - install via the [Qt Online Installer](https://www.qt.io/development/download-qt-installer-oss); include the `Core`, `Widgets`, `SQL`, and `Network` modules plus the MinGW kit
- **CMake 3.20 or later** - add to PATH during installation
- **MinGW 12** or any C++17-capable compiler compatible with Qt 6 on Windows (bundled with the Qt MinGW kit)
- **Git**

### Installation

Clone the repository:

```bash
git clone https://github.com/codingburgas/Certa.dev.git
cd Certa.dev
```

The project database `assets/Certadev.db` is included - no separate setup needed.

### Building

```bash
cmake -B build -S .
cmake --build build
```

Qt's code-generation steps (`AUTOMOC`, `AUTOUIC`, `AUTORCC`) run automatically as part of the CMake build. Run the produced binary from the `build` directory - the binary reads `assets/Certadev.db` relative to its working directory, so do not move it without also moving the `assets/` folder.

## Documentation

- **Project Documentation** - [Certa.dev_Documentation.docx](https://codingburgas-my.sharepoint.com/:w:/g/personal/didimov24_codingburgas_bg/IQBdUnD24GwWToQrNqNmzk-4AXmtaLVgqlql6BEtWoqEzkU?e=w8jeU3)
- **Project Presentation** - [Certa.dev_Presentation.pptx](https://codingburgas-my.sharepoint.com/:p:/g/personal/didimov24_codingburgas_bg/IQCf_gZiHfk0SI-KZ0e0z7azAbVIVi0hpdDMbb3mkntC_-w?e=5om4lB)

## Contributors

- [**Denislav Dimov**](https://github.com/didimov24) - Scrum Trainer
- [**Vsevolod Bolotov**](https://github.com/vybolotov24) - Backend developer
- [**Georgi Georgiev**](https://github.com/gageorgiev24) - Backend developer
- [**Dimitar Rusev**](https://github.com/ddrusev24) - Frontend developer
