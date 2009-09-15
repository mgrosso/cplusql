#include "db/DBManager.h"
#include "db/DBConnection.h"
#include "db/DBStatement.h"
#include "db/DBPreparedStatement.h"

void selectAll(ConnectionPtr & conn);
void insertVal(ConnectionPtr &, int, const char*);
void updateVal(ConnectionPtr &, int, const char*);
void deleteVal(ConnectionPtr &, int);
void deleteVal(ConnectionPtr &, const char*);
void commit(ConnectionPtr &);
void begin(ConnectionPtr &);
void rollback(ConnectionPtr &);
void executePreparedStatement(PreparedStatementPtr &);

/* The test requires a db named "test" on localhost and a user "lksm" in
 * "test" database.  The tests are run against a table called "dpak" 
 * which has the following ddl:
 *	create table dpak
 *	(col1 int not null
 *	 col2 char(20) null);
 *	create unique index index1 on dpak(col1);
 */
int main(int argc, char** argv)
{
	if(argc < 3)
	{
		cerr << "usage: pgdbtest mode col1_val col2_val" << endl;
	}
	string mode(argv[1]);
	int col1_val = atoi(argv[2]);
	string col2_val(argv[3]);
	ConnectionPtr conn = DBManager::getConnection("lksm", "", "localhost/test", DBConnection::POSTGRES_CONNECTION);
	try
	{
		if(strcmp(mode.c_str(), "0")==0)
		{
			string query = col2_val;
			PreparedStatementPtr stmt = conn->prepareStatement(query.c_str());
			cerr<<"PSTMT created successfully." << endl;
			executePreparedStatement(stmt);
		}
		else if(strcmp(mode.c_str(), "1")==0)
		{
			begin(conn);
			sleep(5);
			updateVal(conn, col1_val, col2_val.c_str());
			sleep(10);
			commit(conn);
		}
		else if(strcmp(mode.c_str(), "2")==0)
		{
			begin(conn);
			sleep(5);
			updateVal(conn, col1_val, col2_val.c_str());
			sleep(10);
			commit(conn);
		}
		else if(strcmp(mode.c_str(), "3")==0)
		{
			begin(conn);
			sleep(2);
			updateVal(conn, col1_val, col2_val.c_str());
			sleep(5);
			rollback(conn);
		}
	}catch(DBException &e){
		cerr << "DBException: " << e.getMessage() << endl;
	}catch(BaseException &e){
		cerr << "BaseException: " << e.getMessage() << endl;
	}
	return 0;
}

void insertVal(ConnectionPtr & conn, int a, const char* b)
{
	char col1_val[10];
	memset(col1_val, 0, 10);
	snprintf(col1_val, 10, "%li", a);

	string insert("insert into dpak values(");
	insert += col1_val;
	insert += string(",");
	insert += string("'") + string(b) + string("'") + string(")");
	try
	{
		StatementPtr stmt = conn->createStatement();
		int rows = stmt->executeUpdate(insert.c_str());
		cerr << "Number of rows inserted: " << rows << endl;
	}catch(BaseException & e){
		cerr << "BaseException: " << e.getMessage() << endl;
	}
}

void updateVal(ConnectionPtr & conn, int a, const char* b)
{
	char col1_val[10];
	memset(col1_val, 0, 10);
	snprintf(col1_val, 10, "%li", a);

	string insert("update dpak set col2=");
	insert += string("'") + string(b) + string("'");
	insert += string("where col1=");
	insert += col1_val;
	try
	{
		StatementPtr stmt = conn->createStatement();
		int rows = stmt->executeUpdate(insert.c_str());
		cerr << "Number of rows updated: " << rows << endl;
	}catch(BaseException & e){
		cerr << "BaseException: " << e.getMessage() << endl;
	}
}

void deleteVal(ConnectionPtr & conn, int a)
{
	char col1_val[10];
	memset(col1_val, 0, 10);
	snprintf(col1_val, 10, "%li", a);

	string insert("delete from dpak where col1=");
	insert += col1_val;
	try
	{
		StatementPtr stmt = conn->createStatement();
		int rows = stmt->executeUpdate(insert.c_str());
		cerr << "Number of rows delete: " << rows << endl;
	}catch(BaseException & e){
		cerr << "BaseException: " << e.getMessage() << endl;
	}
}

void deleteVal(ConnectionPtr & conn, const char * b)
{
	string insert("delete from dpak where col2=");
	insert += string("'") + string(b) + string("'");
	try
	{
		StatementPtr stmt = conn->createStatement();
		int rows = stmt->executeUpdate(insert.c_str());
		cerr << "Number of rows delete: " << rows << endl;
	}catch(BaseException & e){
		cerr << "BaseException: " << e.getMessage() << endl;
	}
}

void commit(ConnectionPtr & conn)
{
	StatementPtr stmt = conn->createStatement();
	stmt->executeUpdate("commit");
	cerr<<"Commit..." << endl;
}

void begin(ConnectionPtr & conn)
{
	StatementPtr stmt = conn->createStatement();
	stmt->executeUpdate("begin transaction");
	cerr<<"Begin..." << endl;
}

void rollback(ConnectionPtr & conn)
{
	StatementPtr stmt = conn->createStatement();
	stmt->executeUpdate("rollback");
	cerr<<"Rollback..." << endl;
}

void selectAll(ConnectionPtr & conn)
{
	StatementPtr stmt = conn->createStatement();
	ResultSetPtr res = stmt->executeQuery("select * from dpak");
	while(res->next())
	{
		int i;
		for(i=0; i< res->numColumns(); i++)
		{
			cerr << res->getString(i) << "|";
		}
		cerr << endl;
	}
}

void executePreparedStatement(PreparedStatementPtr & stmt)
{
	long long l = 20000000000;
	stmt->setNumeric(1, l);
	stmt->setString(2, "c");
	stmt->setString(3, NULL);
	int num= stmt->executeUpdate();
	cerr << "NUM ROWS INSERTED: " << num << endl;
	/*ResultSetPtr res = stmt->executeQuery();
	while(res->next())
	{
		int i;
		for(i=0; i< res->numColumns(); i++)
		{
			cerr << res->getString(i) << "|";
		}
		cerr << endl;
	}*/
}
