#   TODO - fix this scheduler!
#   This scheduler only works when every task's period is an interval of the smallest period
#   It also needs to have every period be evenly divisible inside the longest period


import math

schedule_file = "Schedule_Input.txt"
log_file = "Schedule_Log.txt"
h_file = "rts_prv_prj.h"

#Schedule Calculator
def main ():

    scheduled_tasks = []
    special_tasks = {}
    schedule = []
    task_table = {}

    #Clear log_file (optional)
    file = open( log_file, "w" )
    file.close ()

    #Read schedule input file
    file = open( schedule_file, "r" )
    file_lines = []

    #Parse schedule file removing comments
    #WARNING: The comment removal has bugs, some combination of comments will likely break it. ("\\" inside "/**/" or vice versa?)
    #TODO fix
    in_comment = False
    temp_line = ""
    for line in file:
        if not in_comment:
            commentc = line.find( "/*" )
            if( commentc > -1 ):
                endcommentc = line.find( "*/" )
                if( endcommentc > -1 ):
                    temp_line = line[0:commentc] + line[endcommentc + 2:( len(line) - 1 )]
                else:
                    temp_line = line[0:commentc]
                    in_comment = True

            commentcpp = line.find( "//" )
            if( commentcpp > -1 ):
                temp_line = line[0:commentcpp]

            if( commentcpp == -1 and commentc == -1 and line != "\n" and line != "" ):
                file_lines.append ( line )
            elif( temp_line != "\n" and temp_line != "" ):
                file_lines.append ( temp_line )
        else:
            endcommentc = line.find( "*/" )
            if( endcommentc > -1 ):
                in_comment = False
                temp_line += line[endcommentc + 2:( len(line) - 1 )]

    file.close ()

    #Parse lines into a list of task requirements
    #List contents for regularly scheduled tasks: [Task ID, Ticks, Period]
    #List contents for special tasks: [Task Description, Task ID]

    task_id = 0
    for line in file_lines:
        line = line.rstrip()
        colon = line.find( ":" )
        #Regular scheduled task
        if( colon == -1 ):
            line = line.split( "," )
            scheduled_tasks.append( [line[0].strip(),int( line[1].strip() ),int( line[2].strip() )])
            task_table[line[0].strip()] = task_id
            task_id += 1
        #Special task
        else:
            line = line.split( ":" )
            special_tasks[line[0].strip()] = line[1].strip()
            task_table[line[1].strip()] = task_id
            task_id += 1

    #Find minimum frame size (frame can't be shorter than the longest task's process time)
    min_fsize = -1
    ltask = ""
    for task in scheduled_tasks:
        task_ticks = task[1]
        if( min_fsize == -1 ):
            min_fsize = task_ticks
            ltask = task[0]
        elif( min_fsize < task_ticks ):
            min_fsize = task_ticks
            ltask = task[0]

    #Find longest running task
    ltaskp = -1
    lptask = ""
    for task in scheduled_tasks:
        task_period = task[2]
        if( ltaskp == -1 ):
            ltaskp = task_period
            lptask = task[0]
        elif( ltaskp < task_period ):
            ltaskp = task_period
            lptask = task[0]

    #The minimum hyperperiod is the period of the longest task
    min_hsize = ltaskp

    #Find the smallest maximum inter-task interval
    itaski = -1
    ftask = ""
    for task in scheduled_tasks:
        task_period = task[2]
        max_interval = task_period
        if( itaski == -1 ):
            itaski = max_interval
            ftask = task[0]
        elif( itaski > max_interval ):
            itaski = max_interval
            ftask = task[0]

    #The maximum frame size has to fit between the start of two instances of the same task.
    max_fsize = itaski

    #Check that tasks will fit
    #The longest running task has to execute in the period between the fastest running task + its allowed jitter.
    if( min_fsize > itaski ):
        print "Task " + ltask + " takes longer to execute than permitted between instances of " + ftask + "."
    else:
        print "Worst case task " + ltask + " fits between instances of " + ftask + "."

    print "Minimum Frame: " + str( min_fsize ) + " = " + ltask
    print "Maximum Frame: " + str( max_fsize ) + " = " + ftask
    print "Longest Period: " + str( ltaskp ) + " = " + lptask
    print "Smallest Maximum Inter-Task Interval: " + str( itaski ) + " = " + ftask
    print "Number of Frames: " + str( ltaskp / min_fsize )

    #Check for schedulability

    #Start with simpler Utilization Bound test
    utilization_array = []
    num_tasks = 0

    for task in scheduled_tasks:
        task_ticks = task[1]    #Ci in the equations
        task_period = task[2]   #Ti in the equations
        task_utilization = float ( task_ticks ) / task_period
        utilization_array.append( task_utilization )

        #Calculate bound for this number of tasks
        num_tasks += 1
        utilization_bound = num_tasks * (( 2.0 ** ( 1.0 / num_tasks )) - 1.0 )

        #If the sum of task utilization to this point is lower than the bound we are schedulable
        if( sum( utilization_array ) < utilization_bound ):
            print "Task " + task[0] + " can be scheduled.  " + "Processor utilization = " + str( sum( utilization_array ))

        #If the tasks utilize more than 100% of the processor we can't schedule :P
        elif( sum( utilization_array ) > 1 ):
            print "Task " + task[0] + " puts processor usage over 100%."

        else:
            print "Finishing check with Precise test."
            break

    #Check remaining tasks with Precise test
    #TODO: finish this section
    response = 0
    last_response = 0
    task_ticks_list = []
    for task in scheduled_tasks:
        task_ticks = task[1]    #Ci in the equations
        task_period = task[2]   #Ti in the equations
        task_iteration_response = sum( task_ticks_list ) + task_ticks
        task_next_iteration_response = 0
        #while iteration_response !=

    #Create schedule starting with the lowest period task
    tick = 0
    schedule_array = []
    task_fits = True

    #Blank out schedule and fill to it's minimum length (every task is scheduled at least once)
    for tick in range( ltaskp ):
        schedule.append( special_tasks["Fill Task"] )

    #Start inserting tasks where they can fit
    #I think this should be re-written as a frame based scheduler to make it more flexable/reliable
    for task_index in range( len( scheduled_tasks )):
        #Initialize variables
        tick            = 0
        task            = scheduled_tasks[task_index]
        task_period     = task[2]
        task_time       = task[1]
        task_name       = task[0]
        open_tick       = special_tasks["Fill Task"]
        schedule_length = len( schedule )
        period_skip     = False

        #Iterate through the length of the schedule
        while tick < schedule_length:
            #If this time tick is open for a task
            if( schedule[tick] == open_tick ):

                #Check if this task fits in this time slot
                task_fits = does_task_fit( task_name, tick, schedule, task_time, special_tasks  )

                #If the task fits put it in the schedule
                if( task_fits ):
                    task_start = tick
                    while( tick < task_start + task_time and tick < schedule_length ):
                        schedule[tick] = task_name
                        tick += 1
                    #Skip ahead 1 task period
                    tick = task_start + task_period - 1
                    period_skip = True

                #If the task doesn't fit and we just skipped a period, report error
                elif( period_skip ):
                    print "Task: " + task_name + " doesn't fit after skipping a period."

            tick += 1

    save_art_schedule( schedule, scheduled_tasks, special_tasks )
    save_schedule_h( schedule, scheduled_tasks, special_tasks, task_table )

#Check if this task fits in this time slot
def does_task_fit( task_name, this_tick, schedule, task_time, special_tasks ):
    #Initialize variables
    future_tick     = this_tick
    task_fits       = True
    open_tick       = special_tasks["Fill Task"]

    #Check the future ticks to see if there is room for this task
    while( future_tick < ( this_tick + task_time )):

        if( schedule[future_tick] != open_tick ):
            task_fits = False

        future_tick += 1

    return task_fits;

# Saves schedule as a txt file for debugging
def save_schedule( schedule ):
    file = open( log_file, "a" )
    for task in schedule:
        file.write( task + ", " )
    file.write( "\n" )
    file.close ()
    return;

# Saves schedule as ascii art to a txt file for debugging
def save_art_schedule( schedule, scheduled_tasks, special_tasks  ):
    file = open( log_file, "a" )
    schedule_art = make_schedule_art( schedule, scheduled_tasks, special_tasks )
    for line in schedule_art:
        file.write( line )
    file.close ()
    return;

# Generates the ascii art for a schedule
# Returns a list of strings
def make_schedule_art( schedule, scheduled_tasks, special_tasks ):
    open_tick       = special_tasks["Fill Task"]
    data            = []
    data_string     = ""
    schedule_length = len( schedule )
    tick            = 0
    page_break      = 100
    right_boarder   = page_break

    #Save index for schedule
    index = 0
    for task in scheduled_tasks:
        data.append( task[0] + " = " + str( index ) + "\n" )
        index += 1

    data.append( special_tasks["Fill Task"] + " = *\n" )
    data.append( "Task Switch = |\n" )
    data.append( "Tasks shown per tick in schedule.\n" )
    data.append( str( right_boarder ) + " ticks per row.\n\n" )



    while( tick < schedule_length ):
        task_name = schedule[tick]
        next_task_name = task_name

        data_string += ( "|" )

        #For the number of ticks that the task is the same
        while( next_task_name == task_name and tick < len( schedule )):

            if( task_name == open_tick ):
                data_string += ( "*" )
            else:
                data_string += ( str( task_index_get( task_name, scheduled_tasks ) ))

            tick += 1
            if( tick < schedule_length ):
                next_task_name = schedule[tick]

        if( tick >= page_break ):
            data_string += ( "\n" )
            page_break += right_boarder

        #data_string += ( "|" )


    data.append( data_string )
    data.append( "\n" )

    return data;

# Saves raw schedule array as a txt file for debugging
# Warning: this assumes a fixed depth array (and is currently broken?)
def save_array( array ):
    file = open( log_file, "w" )
    for line in array:
        for item in line:
            file.write( item + ", " )
        file.write( "\n" )
    file.close ()
    return;

# Returns the index of a task when given a task_name
def task_index_get( task_name, scheduled_tasks ):

    task_index = 0
    for this_task in scheduled_tasks:
        #print "Compare: " + str( this_task[0] ) + " and " + task_name + " is " + str( this_task[0] == task_name )
        if( this_task[0] == task_name ):
            #print "Returning: " + str( task_index )
            return task_index;
        task_index += 1
    print "Didn't find index: " + str( task_name )
    return -1;

# Writes the schedule into a .h file for use
def save_schedule_h( schedule, scheduled_tasks, special_tasks, task_table ):
    tick = 0
    schedule_length = len( schedule )

    #Beginning of file
    file_data = (                                                                                                 \
    "/*********************************************************************                                     \n\
    *                                                                                                           \n\
    *   HEADER NAME:                                                                                            \n\
    *       rts_prv_prj.h - Project specific include file for rts_main.c                                        \n\
    *                                                                                                           \n\
    *       Only include this file in rts_main.c.                                                               \n\
    *                                                                                                           \n\
    *   Copyright 2010-2018 by Garmin Ltd. or its subsidiaries.                                                 \n\
    *                                                                                                           \n\
    *********************************************************************/                                      \n\
                                                                                                                \n\
    #ifndef RTS_PRV_PRJ_H                                                                                       \n\
    #define RTS_PRV_PRJ_H                                                                                       \n\
                                                                                                                \n\
    /*--------------------------------------------------------------------                                      \n\
                            GENERAL INCLUDES                                                                    \n\
    --------------------------------------------------------------------*/                                      \n\
                                                                                                                \n\
    #include \"GRM_pub_lib.h\"                                                                                  \n\
                                                                                                                \n\
    #include \"CTL_pub.h\"                                                                                      \n\
    #include \"DBG_pub_cmd.h\"                                                                                  \n\
    #include \"HWM_pub_can.h\"                                                                                  \n\
    #include \"HWM_pub_led.h\"                                                                                  \n\
    #include \"HWM_pub_sense.h\"                                                                                \n\
    #include \"HWM_pub_tach.h\"                                                                                 \n\
    #include \"IOP_pub.h\"                                                                                      \n\
    #include \"SYC_pub_timer.h\"                                                                                \n\
                                                                                                                \n\
    #include \"hwm_prv_prj.h\"                                                                                  \n\
                                                                                                                \n\
    /*--------------------------------------------------------------------                                      \n\
                            LITERAL CONSTANTS                                                                   \n\
    --------------------------------------------------------------------*/                                      \n\
                                                                                                                \n\
    /*--------------------------------------------------------------------                                      \n\
                                    TYPES                                                                       \n\
    --------------------------------------------------------------------*/                                      \n\
                                                                                                                \n\
    /*--------------------------------------------------------------------                                      \n\
                            PROJECT INCLUDES                                                                    \n\
    --------------------------------------------------------------------*/                                      \n\
                                                                                                                \n\
    /*--------------------------------------------------------------------                                      \n\
                            MEMORY CONSTANTS                                                                    \n\
    --------------------------------------------------------------------*/                                      \n\
                                                                                                                \n\
    /*--------------------------------------------------------------------                                      \n\
                                VARIABLES                                                                       \n\
    --------------------------------------------------------------------*/                                      \n\
                                                                                                                \n\
    /*----------------------------------------------------------------                                          \n\
                                                                                                                 "\
    ).split( "\n" ) + make_schedule_art( schedule, scheduled_tasks, special_tasks ) + (                          "\
    ---------------------------------------------------------------*/                                           \n\
    #define TASK_DATA                                                                                       \\  \n\
    {                                                                                                       \\  \n\
    /*------------------------------------------------------------------------------------------------      \\  \n\
    task_id,    ticks left, number of overruns                                                              \\  \n\
    ------------------------------------------------------------------------------------------------*/      \\   "\
    ).split( "\n" )

    for task_name in task_table:
        file_data.append( "{ " + str( task_table[task_name] ) + ", 0, 0 },  \\" )

    file_data += (                                                                                                \
    "}                                                                                                         \n\
                                                                                                               \n\
                                                                                                               \n\
    #define TASK_SCHEDULE                                                                                   \\ \n\
    {                                                                                                       \\  \n\
    /*------------------------------------------------------------------------------------------------      \\  \n\
    task_id,    ticks,    task_function                                                                     \\  \n\
    ------------------------------------------------------------------------------------------------*/      \\   "\
    ).split( "\n" )

    #Re-write schedule into task_name/task_ticks pairs
    while( tick < schedule_length ):
        task_name = schedule[tick]
        next_task_name = task_name
        task_ticks = 0

        #For the number of ticks that the task is the same add to the schedule ticks
        while( next_task_name == task_name and tick < len( schedule )):
            tick += 1
            task_ticks += 1
            if( tick < len( schedule )):
                next_task_name = schedule[tick]

        file_data.append( "{  " + str( task_table[task_name] ) + ", " + str( task_ticks ) + ",  " + task_name + " },  \\" )

    #Write end of file
    file_data += (                                                                                    \
    "}                                                                                              \n\
                                                                                                    \n\
    /*--------------------------------------------------------------------                          \n\
                                  MACROS                                                            \n\
    --------------------------------------------------------------------*/                          \n\
                                                                                                    \n\
    /*--------------------------------------------------------------------                          \n\
                                PROCEDURES                                                          \n\
    --------------------------------------------------------------------*/                          \n\
                                                                                                    \n\
    #endif /* RTS_PRV_PRJ_H */                                                                      \n\
                                                                                                     "\
    ).split( "\n" )

    file = open( h_file, "w" )  #Clears the h_file

    for line in file_data:
        file.write( line.strip() + "\n" )

    file.close ()
    return;

main()
