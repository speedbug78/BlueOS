try:
    import Tkinter as tk
    import Tkinter.font
except ImportError:
    import tkinter as tk
    import tkinter.font

import BlueOS_support

global window_buffer
window_buffer = 8

class Timeline_Block:
    def __init__( self, canvas ):
        self.canvas = canvas
        self.window_height = self.canvas.winfo_height()
        self.window_width = self.canvas.winfo_width()
        self.displayed_tasks = []
        self.task_color_info = {}
        self.scroll_limit = 0
        self.scroll_position = 0
        self.scale = 5 # Pix per tick
        self.window_buffer = 8

        self.font = "Times 10"
        self.txt_color = "black"
        self.txt_height = tk.font.Font( font=self.font ).metrics( 'linespace' )

        self.canvas.bind('<MouseWheel>',lambda e:self.scroll(e))

    def scroll( self, p1 ):
        if(( p1.delta > 0 ) and ( self.scroll_position < 0 )):
            self.scroll_position += 1
        elif(( p1.delta < 0 ) and ( self.scroll_position > -self.scroll_limit )):
            self.scroll_position -= 1
        self.canvas.scan_dragto( 0, self.scroll_position )

    def draw_timeline( self, schedule, task_info ):
        self.canvas.create_line( self.window_buffer, 100, self.window_width - self.window_buffer, 100 )

        position_index = self.window_buffer

        # Destroy existing timeline
        for box in self.displayed_tasks:
            box.destroy()
            del box

        index = 0
        while index < len( schedule ):
            task = schedule[index]

            if( task in task_info ):
                self.task_color_info[task] = task_info[task]["color"]
            else:
                self.task_color_info[task] = "white"

            # Count number of ticks this task runs
            start_tick = index
            name = task
            while(( name == task ) and ( index < len( schedule ))):
                name = schedule[index]
                index += 1

            width = (( index - start_tick ) * self.scale )  # In pix
            position = (( position_index, self.window_buffer ))
            self.add_item( task, width, position, self.task_color_info[task] )

            position_index += ( width + self.window_buffer )

    def add_item( self, name, width, position, color ):
        new_item = Task_Rect( self.canvas, name, width, position, color )
        self.displayed_tasks.append( new_item )

    def update( self ):
        # Update timeline information
        self.canvas.itemconfig( self.title, text = self.name + " (" + mem_size_num_to_str( self.mem_end - self.mem_start ) + ") - " + hex( self.mem_start ))
        self.canvas.itemconfig( self.end, text = hex( self.mem_end ))

class Task_Rect:
    def __init__( self, canvas, name = "", width = 0, position = ( 0, 0 ), color = "white"):
        self.canvas = canvas
        self.width = width
        self.height = 50
        self.position = position
        self.middle = ( self.position[0] + ( self.width / 2 ), self.position[1] + ( self.height / 2 ))
        self.name = name
        self.txt_buffer = 2

        self.font = "Times 10"
        self.txt_color = "black"
        self.color = color
        self.txt_height = tk.font.Font( font=self.font ).metrics( 'linespace' )

        self.rect = canvas.create_rectangle( self.position[0], self.position[1], self.position[0] + self.width, self.position[1] + self.height, fill = self.color )
        self.text = canvas.create_text( self.middle[0], self.middle[1], fill = self.txt_color, font = self.font, text = self.name )

        self.canvas.tag_bind( self.rect, '<ButtonRelease-1>', self.click )
        self.canvas.tag_bind( self.text, '<ButtonRelease-1>', self.click )

    def click( self, event ):
        print( self.name + ": Clicked!" )

    def set_size( self, size ):
        self.width = size[0]
        self.set_height( size[1] )

    def set_height( self, height ):
        min_height = self.txt_height + ( self.txt_buffer * 2 )
        if( height < min_height ):
            self.height = min_height
        else:
            self.height = height

    def update( self ):
        self.middle = ( self.position[0] + ( self.width / 2 ), self.position[1] + ( self.height / 2 ))
        # Update rectange size & position
        self.canvas.coords( self.rect, self.position[0], self.position[1], self.position[0] + self.width, self.position[1] + self.height )
        # Update rectange color
        self.canvas.itemconfig( self.rect, fill = self.color)
        # Update text
        self.canvas.itemconfig( self.text, text = self.name )
        # Update text position
        self.canvas.coords( self.text, self.middle[0], self.middle[1] )
        # Update text color
        self.canvas.itemconfig( self.text, fill = self.txt_color )
        # Update text font
        self.canvas.itemconfig( self.text, font = self.font )

    def destroy( self ):
        self.canvas.delete( self.text )
        self.canvas.delete( self.rect )

