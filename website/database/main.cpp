#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sqlite3.h> 
using namespace std;

struct tmElements_t { 
  uint8_t Second; 
  uint8_t Minute; 
  uint8_t Hour; 
  uint8_t Wday;   // day of week, sunday is day 1
  uint8_t Day;
  uint8_t Month; 
  uint8_t Year;   // offset from 1970; 
};

struct payload_data_t
{
  float p; // 4 bytes
  float t; // 4 bytes
  float h; // 4 bytes
  short int vcc; // 2 bytes
  short int lum; // 2 bytes
  bool rain; // 1 byte
  tmElements_t tm;
}; // 17 bytes < 32 bytes (max payload size for NRF24L01+)

payload_data_t data;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

char* getTimestamp(const tmElements_t d) {
   char* timestamp;
   sprintf(timestamp, "%u-%u-%u %u:%u:%u.000", d.Year, d.Month, d.Day, d.Hour, d.Minute, d.Second);
   return timestamp;
}

int saveNodeData(const payload_data_t data) {

   // Open database
   char *zErrMsg = 0;
   int rc;
   char *sql;
   sqlite3 *db;
   rc = sqlite3_open("test.db", &db);
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return -1;
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   // char* timestamp = getTimestamp(data.tm);
   printf("%u", data.tm.Year);
   // printf("%u-%u-%u %u:%u:%u.000", data.tm.Year, data.tm.Month, data.tm.Day, data.tm.Hour, data.tm.Minute, data.tm.Second);
   // Insert one record    
   sprintf(sql, "INSERT INTO data (NODE_ID, TEMP, PRESSURE, HUMIDITY, RAIN, LUM, V_BAT, DATETIME)" \
   "VALUES(%d, %f, %f, %f, %d, %d, %f, %s)", 1, data.t, data.p, data.h, data.rain ? 1 : 0, data.lum, data.vcc, getTimestamp(data.tm));
   
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);
   return 0;
}

int createDataTable () {
      
   char *zErrMsg = 0;
   int rc;
   char *sql;

   sqlite3 *db;
   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   // Create table for storing sensor data of one node
   sql = "CREATE TABLE DATA("  \
      "ID INT PRIMARY KEY NOT NULL," \
      "NODE_ID  INT   NOT NULL," \
      "TEMP     REAL  NOT NULL," \
      "PRESSURE REAL  NOT NULL," \
      "HUMIDITY REAL  NOT NULL," \
      "RAIN     BOOL  NOT NULL," \
      "LUM      INT   NOT NULL," \
      "V_BAT    REAL  NOT NULL," \
      "DATETIME TEXT  NOT NULL)";  // stored as  YYYY-MM-DD HH:MM:SS.SSS
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);
}


int main(int argc, char** argv)
{

   // Mock arduino data 
   data.t = 25;
   data.p = 1023.12;
   data.h = 45.30;
   data.rain = true;
   data.lum = 930;
   data.vcc = 5100;
   data.tm.Hour = 3;
   data.tm.Minute = 39;
   data.tm.Second = 0;
   data.tm.Day = 17;
   data.tm.Month = 5;
   data.tm.Year = 2020;
   
   // Once
   // createDataTable();

   // Store sensor data
   saveNodeData(data);
}