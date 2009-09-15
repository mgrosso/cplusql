#ident  "file_id $Id: dbtest.cpp,v 1.5 2004/01/05 20:06:23 mgrosso Exp $"
#include <strstream>
#include "db/DBManager.h"
#include "db/DBConnection.h"
#include "db/sequenceWrapper.h"

int littleTest();
int bigTest();

int
main() {
    try{
        return littleTest();
    }catch ( const char *s ){
        cerr << "const char * exception: " << s << endl ;
    }catch ( BaseException &be ){
        cerr << "BaseException: " << be.getMessage() << endl;
    }catch ( ... ){
        cerr << "unknown exception." << endl;
    }
    return 0;
}

int 
littleTest()
{
    ConnectionPtr pt = DBManager::getConnection("pkgnu06","pkgntest06","dev1");
    StatementPtr  st = pt->createStatement();
    //cout << "Insert returns: " 
    //     << st->executeUpdate("INSERT INTO FOO VALUES ('foobar')")
    //     << endl;
    cerr << "0" << endl ;
    ResultSetPtr rs = st->executeQuery("select 'foo' as XVAL from dual");
    cerr << "1" << endl ;
    if (rs->isValid())
    {
        cerr << "Result set is valid" << endl;
        while(rs->next())
        {
            const string &xval=rs->getString("XVAL");
            const char *xvals = xval.c_str()  ;
            cerr << xvals << endl;
        }
        cerr << "Finished retrieval" << endl;
    }
    else 
    {
        cerr << "Invalid result set" << endl;
    }


    //rs = st->executeQuery("select client_cookie from person_session_etl");
    ResultSetPtr rs2 = st->executeQuery("SELECT DAY_DESC FROM DAY_DIM");
    //cerr << "1" << endl ;
    if (rs2->isValid())
    {
        cerr << "Result set is valid" << endl;
        while(rs2->next())
        {
            //cerr << "." ;
            //const string &ddesc=rs2->getString("DAY_DESC");
            //fprintf(stderr, "error=%s\n",pt->errorMessage());
            cerr << rs2->getString("DAY_DESC") << endl ;
            //cerr << ddescs << endl;
//            cerr << rs->colLength("FIRST_VISIT_FLAG") << endl;
        }
        cerr << "Finished retrieval" << endl;
    }
    else 
    {
        cerr << "Invalid result set" << endl;
    }
    return 0;
}


int
bigTest()
{
    strstream inserter, deleter;
    string input;

    inserter << "INSERT INTO FOO VALUES ('";
    deleter  << "delete from foo where value='";

    cout << "Value to insert into the table: ";
    cin >> input;
    inserter << input.c_str();
    inserter << "')";

    cout << "Value to delete from the table: ";
    cin >> input;
    deleter << input.c_str();
    deleter << "'";


    ConnectionPtr pt = DBManager::getConnection("mamau03","mamau03","pkdw");
    ConnectionPtr p2 = DBManager::getConnection("mamau03","mamau03","pkdw");

    StatementPtr  st = pt->createStatement();
    StatementPtr  s2 = p2->createStatement();

    cout << "Executing insertion queries." << endl;

    int  ret1 = st->executeUpdate(inserter.str());
    int  ret2 = s2->executeUpdate(deleter.str());


    cout << "st returns " << ret1 << endl;
    cout << "s2 returns " << ret2 << endl;

    cout << "Executing: " << "select url_pattern_1, url_pattern_2 from content_dim where url_pattern_1 like '%computer_club%'" << endl;
    ResultSetPtr rs = st->executeQuery("select url_pattern_1, url_pattern_2 from content_dim where url_pattern_1 like '%computer_club%'");
    if (rs->isValid()) {
        int numcols = rs->numColumns();
        cout << "COLUMNS: " << numcols << endl;
        for (int i=0; i<numcols; ++i) {
            cout << "\tLENGTH: " << rs->colLength(i) << "\t"
                 << "\tLENGTH: " << rs->colLength(rs->colName(i)) << "\t"
                 << "\tNAME: " << rs->colName(i) << endl;
        }
    } else {
        cout << "failure" << endl;
        return 0;
    }
    while(rs->next()) {
        DBRowPtr row = rs->getRow();
        DBRow::iterator beg = row->begin();
        DBRow::iterator end = row->end();

        while (beg != end) {
            cout << (*beg).get() << "|";
            ++beg;
        }
        cout << endl;
    }
    cout << "ROWS PROCESSED: " << rs->rowsProcessed() << endl;

    cout << "Executing: " << "select content_key, content_name, begin_day_key from content_dim where url_pattern_1 like '%devin%'" << endl;
    rs = st->executeQuery("select content_key, content_name, begin_day_key from content_dim where url_pattern_1 like '%devin%'");

    if (rs->isValid()) {
        int numcols = rs->numColumns();
        cout << "COLUMNS: " << numcols << endl;
        for (int i=0; i<numcols; ++i) {
            cout << "\tLENGTH: " << rs->colLength(i) << "\t"
                 << "\tNAME: " << rs->colName(i) << endl;
        }
    } else {
        cout << "failure" << endl;
        return 0;
    }
    while(rs->next()) {
        DBRowPtr row = rs->getRow();
        DBRow::iterator beg = row->begin();
        DBRow::iterator end = row->end();

        while (beg != end) {
            cout << (*beg).get() << "|";
            ++beg;
        }
        try {
            cout << " BY NAME: " << rs->getString("CONTENT_KEY");
        } catch (BaseException e) {
            cout << " EXCEPTION: " << e.getMessage() ;
        }
        cout << endl;
    }

    sequenceWrapper * seq = sequenceWrapper::Instance(pt, "person_key_seq", 10000);
    for (int i=0; i<100; ++i) {
	char seqnum[28];
	memset(seqnum, 0, 28);
	snprintf(seqnum, 28, "%Li", seq->nextval());
        cout << seqnum << " ";
    }
    cout << endl;
    cout << "FINISHED" << endl;
    seq->close();
    return 0;
}
