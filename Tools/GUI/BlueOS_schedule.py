#   TODO - fix this scheduler!
#   This scheduler only works when every task's period is an interval of the smallest period
#   It also needs to have every period be evenly divisible inside the longest period


import math

tick_time = 0.100  # in ms

#Schedule Calculator
def calc_schedule( task_info ):

    scheduled_tasks = []
    schedule = []
    task_table = {}

    #Find minimum frame size (frame can't be shorter than the longest task's process time)
    min_fsize = -1
    ltask = ""
    for task in task_info:
        print( "Task: " + str( task ))
        task_ticks = ms_to_ticks( task_info[task]["Duration"] )
        if( min_fsize == -1 ):
            min_fsize = task_ticks
            ltask = task
        elif( min_fsize < task_ticks ):
            min_fsize = task_ticks
            ltask = task

    #Find longest running task
    ltaskp = -1
    lptask = ""
    for task in task_info:
        task_period = ms_to_ticks( task_info[task]["Period"] )
        if( ltaskp == -1 ):
            ltaskp = task_period
            lptask = task
        elif( ltaskp < task_period ):
            ltaskp = task_period
            lptask = task

    #The minimum hyperperiod is the period of the longest task
    min_hsize = ltaskp

    #Find the smallest maximum inter-task interval
    itaski = -1
    ftask = ""
    for task in task_info:
        task_period = ms_to_ticks( task_info[task]["Period"] )
        max_interval = task_period
        if( itaski == -1 ):
            itaski = max_interval
            ftask = task
        elif( itaski > max_interval ):
            itaski = max_interval
            ftask = task

    #The maximum frame size has to fit between the start of two instances of the same task.
    max_fsize = itaski

    #Check that tasks will fit
    #The longest running task has to execute in the period between the fastest running task + its allowed jitter.
    if( min_fsize > itaski ):
        print( "Task " + ltask + " takes longer to execute than permitted between instances of " + ftask + "." )
    else:
        print( "Worst case task " + ltask + " fits between instances of " + ftask + "." )

    print( "Minimum Frame: " + str( min_fsize ) + " = " + ltask )
    print( "Maximum Frame: " + str( max_fsize ) + " = " + ftask )
    print( "Longest Period: " + str( ltaskp ) + " = " + lptask )
    print( "Smallest Maximum Inter-Task Interval: " + str( itaski ) + " = " + ftask )
    print( "Number of Frames: " + str( ltaskp / min_fsize ))

    #Check for schedulability

    #Start with simpler Utilization Bound test
    utilization_array = []
    num_tasks = 0

    for task in task_info:
        task_ticks = ms_to_ticks( task_info[task]["Duration"] )    #Ci in the equations
        task_period = ms_to_ticks( task_info[task]["Period"] )  #Ti in the equations
        task_utilization = float ( task_ticks ) / task_period
        utilization_array.append( task_utilization )

        #Calculate bound for this number of tasks
        num_tasks += 1
        utilization_bound = num_tasks * (( 2.0 ** ( 1.0 / num_tasks )) - 1.0 )

        #If the sum of task utilization to this point is lower than the bound we are schedulable
        if( sum( utilization_array ) < utilization_bound ):
            print( "Task " + task + " can be scheduled.  " + "Processor utilization = " + str( sum( utilization_array )))

        #If the tasks utilize more than 100% of the processor we can't schedule :P
        elif( sum( utilization_array ) > 1 ):
            print( "Task " + task + " puts processor usage over 100%." )

        else:
            print( "Finishing check with Precise test." )
            break

    #Check remaining tasks with Precise test
    #TODO: finish this section
    response = 0
    last_response = 0
    task_ticks_list = []
    for task in task_info:
        task_ticks = ms_to_ticks( task_info[task]["Duration"] )    #Ci in the equations
        task_period = ms_to_ticks( task_info[task]["Period"] )  #Ti in the equations
        task_iteration_response = sum( task_ticks_list ) + task_ticks
        task_next_iteration_response = 0
        #while iteration_response !=

    #Create schedule starting with the lowest period task
    tick = 0
    schedule_array = []
    task_fits = True

    #Blank out schedule and fill to it's minimum length (every task is scheduled at least once)
    for tick in range( math.ceil( ltaskp )):
        schedule.append( "Fill" )

    #Start inserting tasks where they can fit
    #I think this should be re-written as a frame based scheduler to make it more flexable/reliable
    for task in task_info:
        #Initialize variables
        tick            = 0
        task_period     = ms_to_ticks( task_info[task]["Period"] )
        task_ticks      = ms_to_ticks( task_info[task]["Duration"] )
        open_tick       = "Fill"
        schedule_length = len( schedule )
        period_skip     = False

        #Iterate through the length of the schedule
        while tick < schedule_length:
            #If this time tick is open for a task
            if( schedule[tick] == open_tick ):

                #Check if this task fits in this time slot
                task_fits = does_task_fit( task, tick, schedule, task_ticks )

                #If the task fits put it in the schedule
                if( task_fits ):
                    task_start = tick
                    while( tick < task_start + task_ticks and tick < schedule_length ):
                        schedule[tick] = task
                        tick += 1
                    #Skip ahead 1 task period
                    tick = task_start + task_period - 1
                    period_skip = True

                #If the task doesn't fit and we just skipped a period, report error
                elif( period_skip ):
                    print( "Task: " + task + " doesn't fit after skipping a period." )

            tick += 1

    return schedule

def ms_to_ticks( ms ):
    return math.ceil( float( ms ) / tick_time )

#Check if this task fits in this time slot
def does_task_fit( task_name, this_tick, schedule, task_ticks ):
    #Initialize variables
    future_tick     = this_tick
    task_fits       = True
    open_tick       = "Fill"

    print( "Schedule Length: " + str( len( schedule )))
    print( "This tick: " + str( this_tick ))
    print( "Task ticks: " + str ( task_ticks ))
    #Check the future ticks to see if there is room for this task
    while( future_tick < ( this_tick + task_ticks )):

        if( schedule[future_tick] != open_tick ):
            task_fits = False

        future_tick += 1

    return task_fits;
