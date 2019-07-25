import os
import sys

try:
    import Tkinter as tk
except ImportError:
    import tkinter as tk

try:
    import ttk
    py3 = False
except ImportError:
    import tkinter.ttk as ttk
    py3 = True

try:
    import tkFileDialog as filedialog
except ImportError:
    from tkinter import filedialog

try:
    import tkMessageBox as messagebox
except ImportError:
    from tkinter import messagebox

def parse_CubeMx_File( cubemx_file_path ):
    proj_data = {}
    proj_file = open( cubemx_file_path, "rt" )
    try:
        for line in proj_file:
            #Escaped "#" sign or no "#" sign (data line)
            if(( "\#" in line ) or ( "#" not in line )):
                line_lst = line.split( "=" )
                line_lst.reverse()
                line_lst[0] = line_lst[0].strip()
                data_lst = line_lst[1].split( "." )
                data_lst.reverse()
                line_data = {data_lst[0]:line_lst[0]}
                for item in data_lst[1:]:
                    line_data = {item: line_data}
                merge_dict( proj_data, line_data )
    except:
        proj_file.close()
        messagebox.showerror("Error", "Unable to parse CubeMx project file.")
        return

    return proj_data
    proj_file.close()

def parse_makefile( makefile_path ):
    makefile_data = {}
    makefile_lst = []
    #try:
    #Open makefile
    makefile_file = open( makefile_path, "rt" )

    #Remove comments
    for line in makefile_file:
        line_lst = line.split( "#" )
        line = line_lst[0]
        if( line.strip() != "" ):
            makefile_lst.append( line.strip() )

    #Close makefile
    makefile_file.close()

    #Combine long lines
    for index, line in enumerate( makefile_lst ):
        if( line != "" ):
            while( makefile_lst[index][len( makefile_lst[index] ) - 1] == "\\" ):
                #Remove "\" from current line and add next line to this one
                makefile_lst[index] = makefile_lst[index].strip( "\\" ).strip() + " " + makefile_lst[index + 1].strip()
                #Remove next line
                del makefile_lst[index + 1]

    #Parse conditionals
    for index, line in enumerate( makefile_lst ):
        if( "ifdef" in line ):
            check_val = line.strip( "ifdef" ).strip()
            print( "Check value: " + check_val )
            #Should this be checked in other places too (i.e. could be defined in the file itself)?
            if( check_val in os.environ ):
                while(( makefile_lst[index] != "else" ) and ( makefile_lst[index] != "endif" )):
                    index = index + 1
                if( makefile_lst[index] == "else" ):
                    del makefile_lst[index]
                    while( makefile_lst[index] != "endif" ):
                        del makefile_lst[index]
            else:
                while(( makefile_lst[index].strip() != "else" ) and ( makefile_lst[index] != "endif" )):
                    print( makefile_lst[index] )
                    del makefile_lst[index]

        #elif( "ifndef" in line ):
        #elif( "ifeq" in line ):
        #elif( "ifneq" in line ):

    #Create variables
    for line in makefile_lst:
        if( "+=" in line ):
            line_lst = line.split( "+=" )
            makefile_data[line_lst[0].strip()] += ( " " + line_lst[1].strip())
        else:
            if( ":=" in line ):
                line_lst = line.split( ":=" )
            else:
                line_lst = line.split( "=" )
            if( len( line_lst ) > 1 ):
                merge_dict( makefile_data, {line_lst[0].strip(): line_lst[1].strip()} )

    #Do replacements

    #except Exception as error:
    #    makefile_file.close()
    #    messagebox.showerror("Error", "Unable to parse Makefile.")
    #    print( error )
    #    sys.stdout.flush()
    #    return

    return makefile_data

def parse_linkfile( linkfile_path ):
    linkfile_data = {"MEMORY":{}, "SECTIONS":{}}
    linkfile_lst = []
    #try:
    #Open linkfile
    linkfile_file = open( linkfile_path, "rt" )

    #Remove comments (modify to handle nested comments)
    in_comment = 0
    for line in linkfile_file:
        comment_start = line.find( "/*" )
        comment_end = line.find( "*/" )

        # A comment begins in this line
        if( comment_start != -1 and comment_end == -1 and in_comment == 0 ):
            linkfile_lst.append(( line[:comment_start - 1] ).strip())
            in_comment = 1

        # A comment ends in this line
        elif( comment_end != -1 and comment_start == -1 and in_comment == 1 ):
            linkfile_lst.append(( line[comment_end + 2:] ).strip())
            in_comment = 0

        # A comment begins and ends in this line
        elif( comment_start != -1 and comment_end != -1 and in_comment == 0 ):
            linkfile_lst.append(( line[:comment_start - 1] + line[comment_end + 2:] ).strip())

        # No comment on this line
        elif( comment_start == -1 and comment_end == -1 and in_comment == 0 ):
            if( line.strip() != "" ):
                linkfile_lst.append( line.strip() )

        # In the middle of a comment
        elif( comment_start == -1 and comment_end == -1 and in_comment == 1 ):
            #Throw away the line
            in_comment = 1

        else:
            messagebox.showerror("Error", "Unable to parse Linkfile.")
            return

    linkfile_file.close ()

    # Parse statements
    in_memory = False
    in_sections = False
    subsection = ""
    depth = 0
    section_started = False
    for index, line in enumerate( linkfile_lst ):

        # Parse MEMORY
        if( "MEMORY" in line or in_memory == True ):

            in_memory = True

            depth += line.count( "{" )
            depth -= line.count( "}" )

            if( depth > 0 ):
                section_started = True
            elif( depth <= 0 and section_started == True ):
                in_memory = False
                section_started = False

            if( ":" in line and section_started == True ):
                line_lst = line.split( ":" )
                mem_key_lst = line_lst[0].split( "(" )
                mem_access = mem_key_lst[1].strip().strip( ")" ).strip()
                mem_type = mem_key_lst[0].strip()
                mem_dat_lst = line_lst[1].strip().split( "," )
                mem_dat = {}
                for item in mem_dat_lst:
                    if "=" in item:
                        item_lst = item.split( "=" )
                        mem_dat[item_lst[0].strip()] = item_lst[1].strip()

                linkfile_data["MEMORY"][mem_type] = mem_dat
                linkfile_data["MEMORY"][mem_type].update( {"ACCESS": mem_access} )

        # Parse SECTIONS
        elif( "SECTIONS" in line or in_sections == True ):
            in_sections = True

            depth += line.count( "{" )
            depth -= line.count( "}" )

            if( depth > 0 ):
                section_started = True
            elif( depth <= 0 and section_started == True ):
                in_sections = False
                section_started = False

            if( depth == 1 and section_started == True and ":" in line ):
                line_lst = line.split( ":" )
                subsection = line_lst[0].strip()
                linkfile_data["SECTIONS"] = merge_dict( linkfile_data["SECTIONS"], {subsection: []} )
            elif( depth == 2 and subsection != "" ):
                linkfile_data["SECTIONS"][subsection].append( line.strip())
            elif( depth == 1 and subsection != "" ):
                subsection = ""


        # Parse statements
        elif( "=" in line ):
            print( line.strip() )
            line_lst = line.split( "=" )
            merge_dict( linkfile_data, {line_lst[0].strip(): line_lst[1].strip().strip( ";" ).strip()} )

    return linkfile_data

def merge_dict( dict1, dict2, value = None ):
    "merges dict2 into dict1"
    if value is None: value = []
    for key in dict2:
        if key in dict1:
            if isinstance(dict1[key], dict) and isinstance( dict2[key], dict ):
                merge_dict( dict1[key], dict2[key], value + [str( key )])
            elif dict1[key] == dict2[key]:
                pass # same leaf value
            else:   #different values for the same leaf
                raise Exception( 'Conflict at %s' % '.'.join( value + [str( key )] ))
        else:
            dict1[key] = dict2[key]
    return dict1
