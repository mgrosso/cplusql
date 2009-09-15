## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      Makefile.am
## Author:    mgrosso Matthew Grosso
## Created:   
## Project:   
## Purpose:   
## 
## Copyright (c) 2003 Matt Grosso. All Rights Reserved.
## 
## $Id: Makefile.am,v 1.37 2005/10/19 08:41:19 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

FRAME_CPP = \
	ExecWrap.cpp \
	Thread.cpp \
	BaseException.cpp \
	DateUtils.cpp \
	Exclusion.cpp \
	Event.cpp \
	Semaphore.cpp \
	ProxyEventHandler.cpp \
	ptr_strdup.cpp \
	fwrap.cpp \
	twrap.cpp \
	netwrap.cpp \
	Config.cpp \
	AppConfig.cpp \
	ConfigDump.cpp \
	Convert.cpp \
	Log.cpp \
	AppLog.cpp \
	Parse.cpp \
	Random.cpp \
	dayTime.cpp \
	Listener.cpp \
	DelimitedColumn.cpp \
	RegexColumns.cpp \
	MultiRegexColumns.cpp \
	ParsedColumns.cpp \
	AbstractNotifier.cpp \
	Notifier.cpp \
	AbstractHashAlg.cpp \
	HashAlgFactory.cpp \
	BasicHashAlg.cpp \
	Expression.cpp \
	AbortExpression.cpp \
	ProxyExpression.cpp \
	BooleanExpression.cpp \
	LogicalExpression.cpp \
	CharExpression.cpp \
	LiteralExpression.cpp \
	ColumnExpression.cpp \
	ColumnElementExpression.cpp \
	HashExpression.cpp \
	md5.cpp \
	MD5Expression.cpp \
	CRC48Expression.cpp \
	CRC48HashAlg.cpp \
	CeilExpression.cpp \
	CeilHashAlg.cpp \
	FloorExpression.cpp \
	FloorHashAlg.cpp \
	JoinExpression.cpp \
	ArithmeticExpression.cpp \
	AdditionExpression.cpp \
	SubtractionExpression.cpp \
	MultiplicationExpression.cpp \
	DivisionExpression.cpp \
	ModuloExpression.cpp \
	BitwiseAndExpression.cpp \
	BitwiseOrExpression.cpp \
	PowExpression.cpp \
	LogicalAndExpression.cpp \
	LogicalOrExpression.cpp \
	IfThenExpression.cpp \
	LessThanExpression.cpp \
	GreaterThanExpression.cpp \
	StrLessThanExpression.cpp \
	StrGreaterThanExpression.cpp \
	EqualsExpression.cpp \
	StrEqualsExpression.cpp \
	MapValueExpression.cpp \
	MapExistsExpression.cpp \
	NegationExpression.cpp \
	IntegerExpression.cpp \
	LineNumberExpression.cpp \
	RandomExpression.cpp \
	RangedUnaryArithmeticExpression.cpp \
	LongDoubleFormatExpression.cpp \
	LnExpression.cpp \
	Log10Expression.cpp \
	Log2Expression.cpp \
	AbstractProjection.cpp \
	NumericProjection.cpp \
	DoubleProjection.cpp \
	StringProjection.cpp \
	SumProjection.cpp \
	AvgProjection.cpp \
	MinProjection.cpp \
	MaxProjection.cpp \
	StrMinProjection.cpp \
	StrMaxProjection.cpp \
	CountDistinctProjection.cpp \
	CountChangesProjection.cpp \
	CountProjection.cpp \
	FirstProjection.cpp \
	LastProjection.cpp \
	Dest.cpp \
	DestBuffered.cpp \
	DestThreadedBuffer.cpp \
	DestFile.cpp \
	DestHashFiles.cpp \
	DestPgCopy.cpp \
	PgCopyListener.cpp \
	DestNotifier.cpp \
	BytePusher.cpp \
	CopyByteSource.cpp \
	ThreadedByteSource.cpp \
	HashHelperShim.cpp \
	DestFactory.cpp \
	Slurp.cpp \
	SlurpedSource.cpp \
	SlurpedLineSource.cpp \
	RFC822Source.cpp \
	AbstractDelimitedSource.cpp \
	AbstractDelimitedSourceNext.cpp \
	NoCopyDelimitedSource.cpp \
	ByteSource.cpp \
	FileByteSource.cpp \
	MultiByteSource.cpp \
	PgCopyByteSource.cpp \
	Source.cpp \
	AbstractSimpleMeta.cpp \
	AbstractRelationMeta.cpp \
	RelationMeta.cpp \
	DestJoint.cpp \
	MapJoint.cpp \
	AbstractJoint.cpp \
	NextNotifyJoint.cpp \
	ExpressionStringMap.cpp \
	RelationFactory.cpp \
	AbstractRelation.cpp \
	ProxyRelation.cpp \
	UnaryExpression.cpp \
	FileExistsExpression.cpp \
	PreSortedProjection.cpp \
	SimpleRelation.cpp \
	DirTree.cpp \
	DirStack.cpp \
	Auth.cpp \
	Connection.cpp \
	HashConnections.cpp \
	DBConnectionFactory.cpp \
	DBException.cpp \
	DBConnection.cpp \
	DBStatement.cpp \
	DBPreparedStatement.cpp \
	DBResultSet.cpp \
	DBDelimitedSource.cpp \
	AbstractKeyValMap.cpp \
	AbstractKeyValSetMap.cpp \
	AbstractDelimitedSourceMap.cpp \
	ADSMapFactory.cpp \
	HashedDSMap.cpp \
	CopyCacheDSMap.cpp \
	ADSProxyMap.cpp \
	SingleRowDelimitedSource.cpp \
	DBMap.cpp \
	DBMapFactory.cpp \
	sequenceWrapper.cpp \
	OracleConnection.cpp \
	OracleStatement.cpp \
	OracleResultSet.cpp \
	PostgresConnection.cpp \
	PostgresStatement.cpp \
	PostgresPreparedStatement.cpp \
	PostgresResultSet.cpp \
	RawExpression.cpp \
	Runnable.cpp \
	RunnableHandler.cpp \
	ExceptionHandler.cpp \
	AppExceptionHandler.cpp \
	AsyncRunnable.cpp \
	ThreadRunnable.cpp \
	ForkExecRunnable.cpp \
	ProxyRunnable.cpp \
	SQLRunnable.cpp \
	PostgresMakeTable.cpp \
	EventQueueExitRunnable.cpp \
	Daemon.cpp \
	AsyncRunnableCollection.cpp \
	SyncRunnableCollection.cpp \
	LRUCache.cpp \
	TranslateExpression.cpp \
	ByteBuf.cpp \
	RegexMatchExpression.cpp \
	RegexWrap.cpp \
	DLList.cpp \
	ServiceInfo.cpp \
	AbstractDestFactory.cpp \
	StringArray.cpp \
	StringArrayListener.cpp \
	StringArrayRelation.cpp \
	Index.cpp \
	IndexBuilder.cpp \
	StringArrayOrderedRelation.cpp \
	StrStrExpression.cpp \
	StrLenExpression.cpp \
	StrPlusExpression.cpp \
	BinaryIntegerExpression.cpp \
	ascii_iso.cpp \
	NormalizeExpression.cpp \
	IconvExpression.cpp \
	MatchCountExpression.cpp \
	CommandExpression.cpp \
	ChompExpression.cpp \
	StubDelimitedSource.cpp \
	DelayStubDelimitedSource.cpp \
	LongLongFormatExpression.cpp \
	WarnExpression.cpp \
	LogInfoExpression.cpp \
	MemoryJoinRelation.cpp \
	DMLExpression.cpp \
	SprintfExpression.cpp \
	SQLEscapeExpression.cpp \
	NanosleepExpression.cpp \
	nanosleep_wrap.cpp \
	ExpressionRelation.cpp \
	PgListenExpression.cpp \
	SQLValueExpression.cpp \
	RepeatDBRelation.cpp \
	CoalesceExpression.cpp \
	CompareListExpression.cpp \
	StrGreatestExpression.cpp \
	StrLeastExpression.cpp \
	SubStrExpression.cpp \
	ByteStripExpression.cpp \
	PhraseMatchColumns.cpp \
	QueryList.cpp \
	XpathRelation.cpp \
	FactorialExpression.cpp \
	SequenceExpression.cpp \
	DaemonizeExpression.cpp


##temporary
##	MergeJoinRelation.cpp 

STATIC_LIB=libframe.a
SHARED_LIB_NO_VERSION=@SHARED_LIB_NO_VERSION@
SHARED_LIB_W_VERSION=@SHARED_LIB_W_VERSION@

libframe_a_SOURCES = $(FRAME_CPP)
lib_LIBRARIES = $(STATIC_LIB)
noinst_PROGRAMS =  dummy
DUMMY=dummy$(EXEEXT)
dummy_SOURCES = 

clean:
	$(RM) *.o $(STATIC_LIB) $(SHARED_LIB_NO_VERSION) $(SHARED_LIB_W_VERSION) $(DUMMY) .depend 

$(DUMMY): $(SHARED_LIB_W_VERSION)
	touch $@

$(SHARED_LIB_NO_VERSION):  $(STATIC_LIB)
	$(CC) $(LIBS) $(SO_OPTS) $(CPPFLAGS) $(libframe_a_OBJECTS) -o $@

$(SHARED_LIB_W_VERSION):  $(SHARED_LIB_NO_VERSION)
	ln -fs $< $@

makedbg: 
	echo SHARED_LIB_NO_VERSION $(SHARED_LIB_NO_VERSION) 
	echo SHARED_LIB_W_VERSION $(SHARED_LIB_W_VERSION) 


depend: .depend

.depend: 
	echo >.depend
	makedepend -Y../inc/ -f.depend $(FRAME_CPP) 2>/dev/null
