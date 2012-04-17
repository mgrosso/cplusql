# cplusql: a relational dataflow language. 

## You define the dataflows
You define relations, expressions, joins, pipes, filters, synchronization points, and outputs, and the language moves rows through those dataflows you've defined.  Whenever a sync point is reached, all flows defined previous to that point are executed in parallel.

## Integration with files, commands, SQL

You can run commands as the source or destination of a stream, or as part of an
expression evaluated for each row in a stream.  Similarly, you can use SQL
expressions as the source or destination of a stream, or inside an expression
executed per row. Importantly, you can use an arbitrarily complex expression to
determine a hash bucket that in turn determines the destination file or
database.

## Grammar 

The grammar is similar to SQL, and is not sensitive to white space or 
case.  For example:

    create stream passwd 
        using files ( "test_passwd" ) delimiter ":"
        columns ( uname,pass,uid,gid,fullname,home_dir,shell ) ;
    create stream passwdjf 
        using passwd 
        where passwd.uid < 20- 4+2*2+3%2- 5/(2+3)+1/2- 1/2 ;
    create dest 
        from passwdjf 
        filename "etc_passwdjf" ;

## Expressions and datatypes

Cplusql has a full featured set of expressions with syntax similar to SQL; the basic math operations are there, but there are also a number of other expressions for string operations, text processing, regex, hash lookups, hashes, sql, shell commands, and more.

## Data structures

Streams can be loaded into memory, optionally indexed in various ways, and accessed repeatedly in subsequent phases of execution.  

### Maps  

The most common data structure is a key/value map.  A typical simple dataware
house data flow might first load one or more dimensions using files,  sql, or
static configurations, creating in memory maps from alternate keys to primary
keys, declare a sync point, and then access fact table data, replacing long
alternate key strings from the raw data with shorter primary key strings from
the map.   

Here is an example program to shrink a file with large strings, perhaps a 
fact table load file:

    create map tiny_dim using configuration ( foo="bar", a="b" );
    create stream bigger using files ( "bigger.txt" ) delimiter "|"
        columns ( id, long_string ) ;
    create map bigger_dim using bigger 
        from bigger.id to bigger.long_string ;
    sync;
    create stream bigdata using files ( "bigdata.txt" ) delimiter ":"
        columns( tiny, big, other );
    create stream bigdata2 using bigdata 
        usercolumns 
            tiny_id     map_value( tiny_dim, bigdata2.tiny, "this could be an abort expression" ),
            bigger_id   map_value( bigger_dim, bigdata2.big, "or this could generate a new key")
        #dont include the big source columns in our output.
        (IGNORE_SOURCE_COLUMNS=1);
    create dest from bigdata2 filename "smallerdata.txt" ;

## In memory relations with or without indexes

An example of loading a file into memory, sorting and filtering it, and subsequently using it to create an ordered stream.

    create stream p using files ( "join.input.txt" ) delimiter ":"
        columns ( uname,pass,uid,gid,fullname,home_dir,shell ) ;
    persist pp from p ;
    sync;
    create index ppindex using pp 
        sort using gid 
        where ! shell ~= "/sbin/nologin" 
        ;
    sync;
    #note:
    #see test/join.cplusql and test/mergejoin.cplusql for 
    #more useful examples of how you can use an indexed in memory table.
    create stream ppordered using pp index ppindex ;
    create dest from ppordered filename "join.output.03.out" ;

## Status

Have you looked at PIG or hive?  ... because I haven't updated this project
except for the README in a while.

In its heyday, cplusql was used regularly in production for about 4 years
across 2 companies, and has been used in shorter term projects since then.
Its robust and full featured. However...

At this point, it hasn't been used in production for years, and there is no
documentation.  I have only minimal time for support, therefore, while it may
be a useful for some projects, I recommend you do not use it unless you are
comfortable enough with c++ source code to make do with minimal support and
still meet your project timelines.

Since each new version of g++ defines some aspect of c++ more tightly than
before, it usually needs a half day of cleaning up every year or two.

Only postgresql is supported currently as I haven't used Oracle or Mysql in a
while.  In practice, this isn't much of an obstacle for the dump and load
phases because for large datasets, the tightly integrated utilities
provided by your database vendor are always far and away the fastest way to
extract and load large flat files. Use cplusql or another ETL tool for
transforming large data sets, and for extracting and loading small to medium
size datasets.

## TODO

Good updates would be native access to HDFS, s3 buckets, json expressions for
access to RESTful apis and mysql.

A syntax supporting transparent job allocation via PBS or hadoop would make
sense as well.  Currently you can execute commands to do this, but thats 
just not a joyful way to do distributed programming.  Have you looked at PIG?


