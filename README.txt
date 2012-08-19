Remarks:

1. There are no "default" transactions, i.e. implicit transactions on every query as in ODBC driver for example. 
I didn't implement that because i guess a customer may do it himself if necessary using BEGIN TRAN, COMMIT TRAN, ROLLBACK
2. Stored procedures support with results as scalar int, double string etc) and as table.
3. Windows Integrated security and SQL Server security supported
4. Parameterized queries support with positional parameters. What is that?:

Parameterized query with positional parameters is the query like: SELECT c1, c2 FROM t WHERE c1 = ? AND c2 = ? 
Where the question marks are the links to parameters that contain data. 
The first sign is the link to the first parameter, second to the second etc. Parameters themself are passed as bind hash. Example:
$t[^table::sql{SELECT c1, c2 FROM t WHERE c1 = ? AND c2 = ?}[
		$.bind[ 
			$.1[Vasya Poupkine]
			$.2(30)
		]
	]
]

Hash keys names equal parameter's position in a query. If a name is impossible to parse as integer or it's value is 
out of range 1-<parameters count> the exception is thrown.

IMPORTANT: Parameter's names must begin with 1.

Why do it? It is useful to prevent so called SQL injections when user's data inserted into query 
without escaping and validation and may damage your DB. 
Moreover, it's possible to run into query size restriction by passing big string to WHERE clause.

Connection string format:

Windows Integrated: mssql://server=<server>^;database=<database>^;Trusted_Connection=Yes
SQL Server: mssql://Server=<server>^;Database=<database>^;Uid=<login>^;Pwd=<password>

limit and offset remark:

Microsoft SQL Server has no offset option like MySQL, so to implement effective limit and offset behavior using only parser's opportunities is 
impossible in common case (but for the simple ones there is the solution but it's not universal). So current implementation is 
ineffective as in ODBC driver also. I think over to deny this option by throwing the exception. Don's use this parser feature. 
It's much better to create SQL queries like this:

select top <limit>... from <table> where <pk> not in (select top <offset> <pk> from <table>  [order by <fields>]) [order by <fields>]

Driver connection:

mssql	<path>\parser3mssql.dll	

To work driver propely you must install Microsoft SQL Server 2005 Native Client http://www.microsoft.com/en-us/download/details.aspx?id=24793 
get sqlncli.msi or sqlncli_x64.msi in case of x64 version of Windows
