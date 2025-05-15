#include "mysql.h"

const char *user = "root";
const char *password = "";
const char *database = "comp";

sql::Driver *driver;
sql::Connection *conn;

NxMode mode;

void InitMySQL()
{
    driver = get_driver_instance();

    conn = driver->connect("tcp://localhost:3306", user, password);

    conn->setSchema(database);
}

void dialogLogin(IPlayer &player, int dialogId, DialogResponse response, int listItem, StringView inputText)
{
    NxPlayerData *data = queryExtension<NxPlayerData>(player);
    IPlayerDialogData *dialog = queryExtension<IPlayerDialogData>(player);

    if (dialogId == DIALOG_LOGIN)
    {
        if (response)
        {
            std::string decrypted_msg = decrypt(data->password, data->hash);

            if (inputText == decrypted_msg)
            {
                data->loggedin = TRUE;
                player.spawn();
            }
            else
            {
                dialog->_show(player, DIALOG_LOGIN, DialogStyle_PASSWORD, "เข้าสู่ระบบ", "กรุณากรอกรหัสผ่านของคุณ", "เข้าสู่ระบบ", "");
            }
        }
        else
        {
            player.kick();
        }
    }

    if (dialogId == DIALOG_REGISTOR)
    {
        if (response)
        {
            std::string pass(inputText);

            if (pass.length() < 8 || pass.length() > 63)
            {
                player._sendClientMessage(Colour::FromARGB(0xFF0000), "รหัสผ่านต้องมีความยาวอย่างน้อย 8 ตัวอักษรและไม่เกิน 63 ตัวอักษร");
                dialog->_show(player, DIALOG_REGISTOR, DialogStyle_INPUT, "ลงทะเบียน", "กรุณาสร้างรหัสผ่านที่คุณต้องการ", "สมัครเลย", "ยกเลิก");
                return;
            }

            try
            {
                std::string keyhash;

                for (int i = 0; i < 16; ++i)
                {
                    keyhash += static_cast<char>(rand() % 94 + 33);
                }

                std::string encrypted_msg = encrypt(pass, keyhash);

                std::unique_ptr<sql::PreparedStatement> stmt(
                    conn->prepareStatement("INSERT INTO players (name, hash, password, created_at) VALUES (?, ?, ?, NOW())"));
                stmt->setString(1, data->name);
                stmt->setString(2, keyhash);
                stmt->setString(3, encrypted_msg);
                stmt->executeUpdate();

                std::unique_ptr<sql::Statement> idStmt(conn->createStatement());
                std::unique_ptr<sql::ResultSet> idRes(idStmt->executeQuery("SELECT LAST_INSERT_ID()"));

                if (idRes->next())
                {
                    data->id = idRes->getInt(1);

                    std::unique_ptr<sql::PreparedStatement> stmt(
                        conn->prepareStatement("SELECT hash, password FROM players WHERE id = ?"));
                    stmt->setInt(1, data->id);

                    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

                    if (res->next())
                    {
                        data->hash = res->getString("hash");
                        data->password = res->getString("password");
                    }
                }

                player._sendClientMessage(Colour::FromARGB(0x00FF00), "ลงทะเบียนสำเร็จ! เข้าสู่เกม...");
                // player.spawn();
                dialog->_show(player, DIALOG_LOGIN, DialogStyle_PASSWORD, "เข้าสู่ระบบ", "กรุณากรอกรหัสผ่านของคุณ", "เข้าสู่ระบบ", "");
            }
            catch (const sql::SQLException &e)
            {
                player._sendClientMessage(Colour::FromARGB(0xFF0000), "เกิดข้อผิดพลาดขณะลงทะเบียน");
                std::cerr << "[MySQL Error] " << e.what() << std::endl;
                dialog->_show(player, DIALOG_REGISTOR, DialogStyle_INPUT, "ลงทะเบียน", "กรุณาสร้างรหัสผ่านที่คุณต้องการ", "สมัครเลย", "ยกเลิก");
            }
        }
        else
        {
            player.kick();
        }
    }
}

void showDialogLogin(IPlayer &player)
{
    IPlayerDialogData *dialog = queryExtension<IPlayerDialogData>(player);
    NxPlayerData *data = queryExtension<NxPlayerData>(player);

    if (!data || !dialog)
        return;

    data->name = std::string(player.getName());

    try
    {
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT * FROM players WHERE name = ?"));
        stmt->setString(1, data->name);

        sql::ResultSet *res = stmt->executeQuery();

        if (res->next())
        {
            data->id = res->getInt("id");
            data->hash = res->getString("hash");
            data->password = res->getString("password");

            dialog->_show(player, DIALOG_LOGIN, DialogStyle_PASSWORD, "เข้าสู่ระบบ", "กรุณากรอกรหัสผ่านของคุณ", "เข้าสู่ระบบ", "");
        }
        else
        {
            dialog->_show(player, DIALOG_REGISTOR, DialogStyle_INPUT, "ลงทะเบียน", "กรุณาสร้างรหัสผ่านที่คุณต้องการ", "สมัครเลย", "ยกเลิก");
        }
    }
    catch (const sql::SQLException &e)
    {
        dialog->_show(player, DIALOG_ERROR, DialogStyle_MSGBOX, "ข้อผิดพลาด", "เกิดข้อผิดพลาดขณะโหลดข้อมูล", "OK", "");

        std::cerr << "[MySQL] Error: " << e.what() << std::endl;
    }
}