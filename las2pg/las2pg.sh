#!/bin/sh

DATABASE="dataset"

dropdb $DATABASE
createdb $DATABASE

psql -d $DATABASE -f schema.sql

pdal pipeline -i pipe.json
