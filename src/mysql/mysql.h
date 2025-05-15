#pragma once
#pragma comment(lib, "mysqlcppconn.lib")

#include "encrypt.h"

#include "jdbc/mysql_connection.h"
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/prepared_statement.h>

#include <Server/Components/Dialogs/dialogs.hpp>

#include "../main.h"

// MYSQL SETTINGS //

extern sql::Driver *driver;
extern sql::Connection *conn;
extern const char *user;
extern const char *password;
extern const char *database;

// END MYSQL SETTINGS //

void InitMySQL();

void dialogLogin(IPlayer &player, int dialogId, DialogResponse response, int listItem, StringView inputText);
void showDialogLogin(IPlayer &player);