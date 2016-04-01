'''
Copyright (c) 2014-2016 Kartik Kumar, Dinamica Srl (me@kartikkumar.com)
Copyright (c) 2014-2016 Abhishek Agrawal, Delft University of Technology
                                          (abhishek.agrawal@protonmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

# Set up modules and packages
# I/O
import commentjson
import json
from pprint import pprint

# SQL database
import sqlite3

# Numerical
import numpy as np
import pandas as pd

# System
import sys
import time

print ""
print "---------------------------------------------------------------------------------"
print "                                 D2D                                             "
print "                                                                                 "
print "         Copyright (c) 2016, K. Kumar (me@kartikkumar.com)                       "
print "         Copyright (c) 2016, A. Agrawal (abhishek.agrawal@protonmail.com)        "
print "---------------------------------------------------------------------------------"
print ""

# Start timer.
start_time = time.time( )

print ""
print "******************************************************************"
print "                          Input parameters                        "
print "******************************************************************"
print ""

# # Parse JSON configuration file
# # Raise exception if wrong number of inputs are provided to script
# if len(sys.argv) != 2:
#     raise Exception("Only provide a JSON config file as input!")

# json_data = open(sys.argv[1])
# config = commentjson.load(json_data)
# json_data.close()
# pprint(config)

# Get plotting packages
# import matplotlib

print ""
print "******************************************************************"
print "                            Operations                            "
print "******************************************************************"
print ""

# Connect to SQLite database.
try:
    database = sqlite3.connect("../data/HEO/heo_rocket_bodies.db")

except sqlite3.Error, e:
    print "Error %s:" % e.args[0]
    sys.exit(1)
















# Fetch scan data.
scan_data = pd.read_sql( "SELECT    arrival_position_error, arrival_velocity_error                \
                          FROM      sgp4_scanner_results                                          \
                          WHERE     lambert_transfer_id = 9032484                                \
                          OR        lambert_transfer_id = 12219982                                \
                          OR        lambert_transfer_id = 12460000                                \
                          OR        lambert_transfer_id = 8292495                                \
                          OR        lambert_transfer_id = 6615000                                \
                          OR        lambert_transfer_id = 5245041                                \
                          OR        lambert_transfer_id = 4752531                                \
                          OR        lambert_transfer_id = 12442847  \
                          OR        lambert_transfer_id = 6806288    \
                          OR        lambert_transfer_id = 6917500;",
                          database )

scan_data.columns = [ 'arrival_position_error', 'arrival_velocity_error' ]

print scan_data['arrival_position_error']
print ""
print scan_data['arrival_velocity_error']


# Close SQLite database if it's still open.
if database:
    database.close( )

# Stop timer
end_time = time.time( )

# Print elapsed time
print "Script time: " + str("{:,g}".format(end_time - start_time)) + "s"

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""
