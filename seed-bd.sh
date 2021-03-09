#!/bin/bash
mysql -u"root" -p"root" -e "DROP DATABASE IF EXISTS TELCOMANAGER;"
mysql -u"root" -p"root" -e "CREATE DATABASE IF NOT EXISTS TELCOMANAGER;"
mysql -u"root" -p"root" TELCOMANAGER -e "CREATE TABLE USER (
  USER_ID VARCHAR(255) NOT NULL,
  NAME_USER VARCHAR(255) NOT NULL,
  PRIMARY KEY (USER_ID));
  CREATE TABLE FILE (
    FILE_ID INT NOT NULL AUTO_INCREMENT,
    NAME_FILE VARCHAR(255) NOT NULL,
    SIZE VARCHAR(255) NOT NULL,    
    USER_ID VARCHAR(255) NOT NULL,
    PRIMARY KEY (FILE_ID),
    FOREIGN KEY (USER_ID) REFERENCES USER(USER_ID)
    );"
mysql -u"root" -p"root" TELCOMANAGER -e " INSERT INTO USER(USER_ID, NAME_USER)
	VALUES ('Cliente1', 'Anderson');
	INSERT INTO USER(USER_ID, NAME_USER)
	VALUES ('Cliente2', 'Soraya');
	INSERT INTO USER(USER_ID, NAME_USER)
	VALUES ('Cliente3', 'Ana Paula');"
mysql -u"root" -p"root" TELCOMANAGER -e " INSERT INTO FILE(NAME_FILE, SIZE, USER_ID)
	VALUES ('Arq1.txt', '6','Cliente1');
	INSERT INTO FILE(NAME_FILE,SIZE, USER_ID)
	VALUES ('Arq2.txt', '6','Cliente2');
	INSERT INTO FILE(NAME_FILE,SIZE, USER_ID)
	VALUES ('Arq3.txt', '6','Cliente3');
	INSERT INTO FILE(NAME_FILE,SIZE, USER_ID)
	VALUES ('Arq4.txt', '6', 'Cliente3');"
