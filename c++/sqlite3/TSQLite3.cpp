#include "TSQLite3.h"




int TSQLite3::test_OpenDB()
{
	sqlite3* db = 0;

	// The database is opened for reading and writing, and is created if it does not already exist. 
	// ������ݿ����ֱ�Ӵ򿪣�����������򴴽�֮���ٴ򿪡��򿪷�ʽΪ�ɶ���д��
	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		int error_code = sqlite3_errcode(db);
		const char* error_msg = sqlite3_errmsg(db);

		std::cout << "Open failed!" << std::endl;
		std::cout << "Error code: " << error_code << std::endl;
		std::cout << "Error message: " << error_msg << std::endl;
		return -1;
	}

	std::cout << "Open successfully!" << std::endl;

	sqlite3_close(db);
	return 0;
}

int TSQLite3::test_CreateTable()
{
	sqlite3* db = NULL;

	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		std::cout << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	const char* kCreateTableSql = "CREATE TABLE COMPANY( \
						ID INT PRIMARY KEY NOT NULL, \
						NAME TEXT NOT NULL, \
						AGE INT NOT NULL, \
						ADDRESS CHAR(50), \
						SALARY REAL);";
	char* error_msg = NULL;
	// ���ʧ�ܻᶯ̬����һ��error_msg,ע��Ҫ�ֶ�free��
	rc = sqlite3_exec(db, kCreateTableSql, NULL, NULL, &error_msg);
	if (rc != SQLITE_OK) {
		std::cout << "Create Table COMPANY failed!" << std::endl;
		std::cout << "Error msg: " << error_msg << std::endl;
		sqlite3_free(error_msg);
		sqlite3_close(db);
		return -1;
	}
	std::cout << "Create Table COMPANY successfully!" << std::endl;
	sqlite3_close(db);
	return 0;
}

static std::string InitInsertSql(const std::string& id,
	const std::string& name,
	const std::string& age,
	const std::string& address,
	const std::string& salary) {
	std::string sql = "INSERT INTO COMPANY(ID, NAME, AGE, ADDRESS, SALARY) VALUES(";
	sql += id + ", '" + name + "', " + age + ", '" + address + "', " + salary + ");";
	return sql.c_str();
}

int TSQLite3::test_InsertData()
{
	sqlite3* db = NULL;

	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		std::cout << "Open " << m_testDbName << " failed! Error msg: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	std::string insert_sql = InitInsertSql("1", "Zhe", "18", "BeiJing", "1000");
	insert_sql += InitInsertSql("2", "Ma", "19", "ShangHai", "2000");
	insert_sql += InitInsertSql("3", "Miki", "20", "GuangZhou", "3000");
	insert_sql += InitInsertSql("4", "Li", "21", "ShenZhen", "4000");
	insert_sql += InitInsertSql("5", "Zhang", "22", "Wuhan", "5000");

	char* error_msg = NULL;
	rc = sqlite3_exec(db, insert_sql.c_str(), NULL, NULL, &error_msg);
	if (rc != SQLITE_OK) {
		std::cout << "Insert failed! Error msg: " << error_msg << std::endl;
		sqlite3_free(error_msg);
		sqlite3_close(db);
		return -1;
		
	}
	std::cout << "Insert successfully!" << std::endl;
	sqlite3_close(db);
	return 0;
}


static int SelectCallback(void* data, int col_count, char** col_values, char** col_names) {
	for (int i = 0; i < col_count; ++i) {
		if (col_values[i] == NULL) {
			continue;
		}

		std::cout << col_names[i] << " = " << col_values[i] << std::endl;
	}

	const char* seperator = reinterpret_cast<const char*>(data);
	std::cout << seperator << std::endl;

	return 0;
}

int TSQLite3::test_SelectData()
{
	sqlite3* db = NULL;

	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		std::cout << "Open " << m_testDbName << " failed! Error msg: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	const char* kSelectSql = "SELECT * FROM COMPANY;";
	const char* kSeperator = "================================";
	char* error_msg = NULL;

	rc = sqlite3_exec(db, kSelectSql, SelectCallback, (void*)kSeperator, &error_msg);
	if (rc != SQLITE_OK) {
		std::cout << "Select failed! Error msg: " << error_msg << std::endl;
		sqlite3_free(error_msg);
		sqlite3_close(db);
		return -1;
	}

	std::cout << "Select successfully!" << std::endl;
	sqlite3_close(db);
	return 0;
}

int TSQLite3::test_UpdateData()
{
	sqlite3* db = NULL;

	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		std::cout << "Open " << m_testDbName << " failed! Error msg: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	const char* kSelectSql = "UPDATE COMPANY SET SALARY = 9999 WHERE ID = 1; SELECT * FROM COMPANY;";
	const char* kSeperator = "================================";
	char* error_msg = NULL;

	rc = sqlite3_exec(db, kSelectSql, 0, (void*)0, &error_msg);
	if (rc != SQLITE_OK) {
		std::cout << "Update failed! Error msg: " << error_msg << std::endl;
		sqlite3_free(error_msg);
		sqlite3_close(db);
		return -1;
		
	}
	std::cout << "Update successfully!" << std::endl;
	sqlite3_close(db);
	return 0;
}


int TSQLite3::test_DeleteData()
{
	sqlite3* db = NULL;

	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		std::cout << "Open " << m_testDbName << " failed! Error msg: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	const char* kSelectSql = "DELETE FROM COMPANY WHERE ID = 4; SELECT * FROM COMPANY;";
	const char* kSeperator = "================================";
	char* error_msg = NULL;

	rc = sqlite3_exec(db, kSelectSql, 0, (void*)0, &error_msg);
	if (rc != SQLITE_OK) {
		std::cout << "Delete failed! Error msg: " << error_msg << std::endl;
		sqlite3_free(error_msg);
		sqlite3_close(db);
		return -1;
	}
	std::cout << "Delete successfully!" << std::endl;

	sqlite3_close(db);
	return 0;
}


int TSQLite3::test_StepExecute(){
	sqlite3* db = NULL;

	int rc = sqlite3_open(m_testDbName, &db);
	if (rc != SQLITE_OK) {
		std::cout << "Open " << m_testDbName << " failed! Error msg: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	const char* kSelectSql = "SELECT * FROM COMPANY WHERE ID = ? OR ID = ?;";

	sqlite3_stmt* stmt = NULL;
	// sqlite3_prepare2 �ӿڰ�һ��SQL��������ֽ������������ִ�к�����
	rc = sqlite3_prepare_v2(db, kSelectSql, strlen(kSelectSql), &stmt, NULL);
	if (rc != SQLITE_OK) {
		std::cout << "Failed to prepare sql!" << std::endl;
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return -1;
	}

	// sqlite3_bind ��������ȫ���ӿڣ�������������SQL�����е�ͨ�����ֵ�ġ�
	sqlite3_bind_int(stmt, 1, 2);
	sqlite3_bind_int(stmt, 2, 3);

	do {
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ERROR) {
			std::cout << "Failed to step execute!" << std::endl;
			break;
		}

		if (rc == SQLITE_DONE) {
			std::cout << "Finished to step execute!" << std::endl;
			break;
		}

		int column_count = sqlite3_column_count(stmt);
		for (int i = 0; i < column_count; ++i) {
			const unsigned char* column_value = sqlite3_column_text(stmt, i);
			if (column_value == NULL) {
				continue;
			}

			std::cout << sqlite3_column_name(stmt, i) << " = " << column_value << std::endl;
		}

		std::cout << "=====================================" << std::endl;
	} while (rc == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}