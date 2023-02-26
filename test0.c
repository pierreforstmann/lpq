/*
 * Portions Copyright (c) 1996-2023, PostgreSQL Global Development Group
 * 
 * Portions Copyright (c) 1994, The Regents of the University of California
 *
 * Portions Copyright(c) 2023 Pierre Forstmann
 *
 * test0.c
 *
 *      Test prepared statements.
 *
 * Before running this, populate a database with the following commands
 *
 * CREATE SCHEMA test0;
 * SET search_path = test0;
 * CREATE TABLE test0 (i int4, t text);
 * INSERT INTO test0 values (1, 'Hello');
 * INSERT INTO test0 values (2, 'Goodbye');
 *
 */

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "libpq-fe.h"

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

int
main(int argc, char **argv)
{
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;
    const char *paramValues[1];
    int         paramLengths[1];
    int         paramFormats[1];
    uint32_t    binaryIntVal;
    int		nFields;
    int		i;
    int		j;

    /*
     * If the user supplies a parameter on the command line, use it as the
     * conninfo string; otherwise default to setting dbname=postgres and using
     * environment variables or defaults for all other connection parameters.
     */
    if (argc > 1)
        conninfo = argv[1];
    else
        conninfo = "dbname = postgres";

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "%s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    /* Set always-secure search path, so malicious users can't take control. */
    res = PQexec(conn, "SET search_path = test0");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "SET failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);

    /* Here is our out-of-line parameter value */
    paramValues[0] = "Hello";

    /*
     * PREPARE
     */

    res = PQprepare(conn,
		    "test0", /* statement name */
                    "SELECT * FROM test0 WHERE t = $1",
                     1,      /* one param */
                     NULL);  /* parameter types */
   
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PQprepare failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    /*
     * EXECUTE
     */

    res= PQexecPrepared(conn,
                        "test0",
                        1,     /* one param */
                        paramValues,
                        NULL,  /* parameter lengths */
    		        NULL,  /* parameter formats */
			0);    /* result format */

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "PQexecPrepared failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    /*
     * retrieve results
     */

    /* first, print out the attribute names */
    nFields = PQnfields(res);
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(res, i));
    printf("\n\n");

    /* next, print out the rows */
    for (i = 0; i < PQntuples(res); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(res, i, j));
        printf("\n");
    }


    /*
     * free resources before exit
     */
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);

    return 0;
}


