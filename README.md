# Bank Management System (Extension 2)

![Bank System](https://img.shields.io/badge/Status-Active-brightgreen)
![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Version](https://img.shields.io/badge/Version-2.0-orange)
![License](https://img.shields.io/badge/License-MIT-green)

This is an **extension 2** of the original Bank Management System project. The core functionality for managing client accounts remains, but this version introduces a **secure login system** and **advanced admin management** with granular permission controls. Now, you can create multiple admins, assign specific permissions (e.g., view clients but not delete them), and ensure only authorized users access sensitive operations. Data is persisted in text files, and the system uses bitwise permissions for efficient access control.

The project is built in C++ and relies on a custom library (`AllStuff.h`) for utility functions like input handling and screen coloring. It's designed for educational purposes, demonstrating file I/O, structs, enums, vectors, and bitwise operations.

---

## Table of Contents

- [New Enhancements](#new-enhancements)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
  - [Login and Admin Permissions](#login-and-admin-permissions)
  - [Client Management](#client-management)
  - [Transactions](#transactions)
  - [Admin Management](#admin-management)
- [Data Storage](#data-storage)
- [Custom Library Dependencies](#custom-library-dependencies)
- [Known Issues and Limitations](#known-issues-and-limitations)
- [Contributing](#contributing)
- [License](#license)

---

## New Enhancements

This extension builds on the previous bank project by adding:

- **Secure Login System**: Admins must log in with a username and password stored in `Admins.text`. Only authenticated users can access the main menu.
- **Admin Management Module**: 
  - Add, delete, update, search, and list admins.
  - Assign **permissions** to admins using bitwise flags (e.g., allow viewing client lists but restrict deletions).
  - Full access (-1) or custom permissions for specific actions like client transactions or managing other admins.
- **Permission Checks**: Every operation verifies the logged-in admin's permissions, preventing unauthorized access (e.g., a limited admin can see clients but can't remove them).
- **Improved Security and Usability**: Prevents self-deletion of the root admin ("admin"), handles invalid inputs gracefully, and uses color-coded feedback for errors/success.

---

## Features

### Core Features (from Original Project & Extension1)
- **Client Operations**:
  - List all clients with details (account number, PIN, name, phone, balance).
  - Add new clients (validates unique account numbers).
  - Delete clients (marks for deletion and rewrites file).
  - Update client details.
  - Search for clients by account number.
- **Transaction Operations(Extenstion 1)**:
  - Deposit funds (updates balance).
  - Withdraw funds (checks for sufficient balance).
  - View total balances across all clients.

### New Features (in this Extension)
- **Admin Operations**:
  - List all admins with usernames, PINs, and permission levels.
  - Add new admins with full or custom permissions.
  - Delete admins (except the root "admin").
  - Update admin details and permissions.
  - Search for admins by username.
- **Permission System**:
  - Bitwise enums for permissions (e.g., `PListClient = 1`, `PDeleteClient = 4`).
  - Check permissions before executing actions (e.g., `CheckPermission(admin, PDeleteClient)`).
  - Custom assignment during add/update: Prompt for specific access (e.g., "Show client list? Y/N").
- **Login and Session Management**:
  - Loads admins from file and verifies credentials.
  - Tracks the current logged-in admin (`AdminThatWillAcess`) for permission checks.
  - Logout returns to login screen.

The system uses vectors of structs for in-memory data handling and rewrites files for persistence.

---

## Project Structure

- **Main Components**:
  - `stdata` struct: Holds client data (account number as primary key).
  - `stadmins` struct: Holds admin data (username as primary key, permissions as int).
  - Enums: `enOption` for main menu, `enAdminPermissions` for access control, `entranscations` for transactions, `enadminsStuff` for admin menu.
  - Global Variables: File paths (`path` for clients, `AdminsPer` for admins), delimiter (`delmi`).

- **Key Functions**:
  - **Data Handling**: `Vector_have_all_data()`, `convert_line_into_record()`, `split_string()`, `edit_file()`.
  - **Client Ops**: `add_client()`, `delete_client()`, `update_client()`, `find_client()`, `show_client_list()`.
  - **Admin Ops**: `add_Admin()`, `delete_Admin()`, `update_admin()`, `find_admin()`, `show_admin_list()`.
  - **Permissions**: `CheckPermission()`, `GiveTheAdminFullAccess()`, `CustomAdminPer()`.
  - **Transactions**: `Deposit()`, `Withdraw()`, `Print_total_balances()`.
  - **Menus**: `main_menu()`, `transcationsScreenMenu()`, `main_admins_menu()`.
  - **Login**: `login()`, `CheckUsernameAndPassword()`.

The code is modular, with separate sections for clients, admins, and transactions.

---

## Installation

1. **Prerequisites**:
   - C++ compiler (e.g., g++ via MinGW or Visual Studio).
   - Custom library: Ensure `AllStuff.h` is in the path `E:\projects\my library\AllStuff.h` (or adjust the include path). This library provides functions like `read_string()`, `read_full_line()`, `enter_postive_number()`, `screen_color()`.

2. **Setup**:
   - Clone the repository:
   - Create empty data files in the project directory OR just use the text files that included in repo :)
     - `local db.text` (for clients).
     - `Admins.text` (for admins). Add a default admin: `admin#//#admin#//#-1` (username: admin, PIN: admin, full permissions).

3. **Compile and Run**:
   - Compile: `g++ main.cpp -o bank_system`
   - Run: `./bank_system`

Note: The project uses absolute paths for the library; update if needed.

---

## Usage

### Login and Admin Permissions
- Start the program: It opens the login screen.
- Enter username and password (e.g., default: admin/admin).
- Permissions control access:
  - Full access (`per = -1`): All operations.
  - Custom: Bitwise OR of enums (e.g., `PListClient | PSearchClient` allows viewing/searching clients but not editing).
- Error Message: "you don't have permission to access this section" if denied.

### Client Management
- From main menu (option 1-5):
  - Add: Prompts for details, checks uniqueness.
  - Delete/Update/Find: Searches by account number, confirms actions.
- Data Format in File: `account#//#pin#//#name#//#phone#//#balance`

### Transactions
- Menu option 6: Sub-menu for deposit/withdraw/total balances.
- Deposit/Withdraw: Validates account, amount, and balance.
- Total Balances: Lists names and balances, sums total.

### Admin Management
- Main menu option 7: Opens admin sub-menu.
- Add: Prompts for username/PIN, then permissions (full or custom).
- Delete: Can't delete "admin".
- Update: Allows changing PIN and permissions.
- Data Format in File: `username#//#pin#//#permissions`

Press any key to return to menus after operations.

---

## Data Storage

- **Clients**: `local db.text` – One line per client, delimited by `#//#`.
- **Admins**: `Admins.text` – One line per admin, including permission integer.
- Files are read into vectors on load, modified in memory, and rewritten on changes.

---

## Custom Library Dependencies

Relies on `AllStuff.h` for:
- Input: `read_string()`, `read_full_line()`, `enter_postive_number()`.
- UI: `screen_color()` (e.g., red for errors, green for success).
- If missing, implement these or adjust the code.


