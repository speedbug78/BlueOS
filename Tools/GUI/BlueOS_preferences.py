import os
import json


pref_file_name = "BlueOS_prefs.bos"

def load_preferences ():
    global preferences
    if( not os.path.exists( pref_file_name )):
        print( "no preference file" )
        preferences = {}
    else:
        pref_file = open( pref_file_name, "r" )
        preferences = json.loads( pref_file.read() )
        pref_file.close()

def save_preferences ():
    pref_file = open( pref_file_name, "w" )
    pref_file.write( json.dumps( preferences ))
    pref_file.close()
