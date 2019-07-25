try:
    import Tkinter as tk
    import Tkinter.font
except ImportError:
    import tkinter as tk
    import tkinter.font

import BlueOS_support

global window_buffer
window_buffer = 8

class Mem_Block:
    def __init__( self, canvas, name = "", mem_start = 0, mem_end = 0, permissions = "" ):
        self.name = name
        self.mem_start = mem_start
        self.mem_end = mem_end
        self.permissions = permissions
        self.canvas = canvas
        self.window_height = self.canvas.winfo_height()
        self.window_width = self.canvas.winfo_width()
        self.allocation_lst = []
        self.scroll_limit = 0
        self.scroll_position = 0

        self.font = "Times 10"
        self.txt_color = "black"
        self.txt_height = tk.font.Font( font=self.font ).metrics( 'linespace' )

        self.canvas.bind('<MouseWheel>',lambda e:self.scroll(e))

        self.show_title()
        self.show_end()

    def scroll( self, p1 ):
        if(( p1.delta > 0 ) and ( self.scroll_position < 0 )):
            self.scroll_position += 1
        elif(( p1.delta < 0 ) and ( self.scroll_position > -self.scroll_limit )):
            self.scroll_position -= 1
        self.canvas.scan_dragto( 0, self.scroll_position )

    def add_item( self, name, position, size, color = "white" ):
        # Convert size to a number if necessary
        if( isinstance( size, str )):
            size = mem_size_str_to_num( size )

        # Find a place for this memory block if necessary
        if( position == 0 ):
            position = self.find_position( size )

        print( "Mem Block Position: " + str( position ))

        new_item = Mem_Rect( self.canvas, name, position, size, color )
        self.allocation_lst.append( new_item )

    def find_position( self, size ):
        for block in self.allocation_lst:
            if(( block.name == "Free" ) and block.mem_size >=  size ):
                return block.mem_position
        return -1

    def order_mem( self ):
        # Remove free space
        for index, item in enumerate( self.allocation_lst ):
            if( item.name == "Free" ):
                self.allocation_lst[index].destroy()
                del self.allocation_lst[index]

        # Sort a list (mem_list) of Mem_Rects by position
        new_mem_list = []
        for item in self.allocation_lst:
            item_placed = False
            for index, place in enumerate( new_mem_list ):
                if item.mem_position < place.mem_position:
                    new_mem_list.insert( index, item )
                    item_placed = True
                    break
            if( item_placed == False ):
                new_mem_list.append( item )

        self.allocation_lst = new_mem_list

        # Find and fill gaps
        for index, item in enumerate( self.allocation_lst ):

            item_end = item.mem_position + item.mem_size
            # If there is room between allocated memory
            if(( index + 1 ) < len( self.allocation_lst )):
                free_space_size = self.allocation_lst[index + 1].mem_position - item_end
                if( free_space_size > 1 ):
                    self.insert_free_space( index + 1, item_end, free_space_size )

            # If there is room at the end of memory
            elif( item_end < self.mem_end ):
                free_space_size = self.mem_end - item_end
                self.insert_free_space( index + 1, item_end, free_space_size )

            # If there is room at the beginning of memory
            elif( index == 0 and ( item.mem_position > self.mem_start )):
                free_space_size = item.mem_position - self.mem_start
                self.insert_free_space( index, self.mem_start, free_space_size )



    def insert_free_space( self, index, mem_position, mem_size ):
        free_space = Mem_Rect( self.canvas, "Free" )
        # free_space.position = ( window_buffer, window_position )
        free_space.mem_size = mem_size
        free_space.mem_position = mem_position
        self.calc_height( free_space )
        self.allocation_lst.insert( index, free_space )

    def show_title( self ):
        # Show title
        self.title = self.canvas.create_text(( self.window_width / 2 ), ( self.txt_height / 2 ) + window_buffer, fill = self.txt_color, font = self.font, text = self.name )

    def show_end( self ):
        # Show memory end
        self.end = self.canvas.create_text(( self.window_width / 2 ), self.window_height - (( self.txt_height / 2 ) + window_buffer ), fill = self.txt_color, font = self.font, text = " ")

    def calc_height( self, item ):
        calc_height = (( item.mem_size / ( self.mem_end - self.mem_start )) * ( self.window_height - ( window_buffer * 4 ) - ( self.txt_height * 2 ))) - window_buffer
        item.set_height( calc_height )

    def update( self ):
        # Update memory information
        self.canvas.itemconfig( self.title, text = self.name + " (" + mem_size_num_to_str( self.mem_end - self.mem_start ) + ") - " + hex( self.mem_start ))
        self.canvas.itemconfig( self.end, text = hex( self.mem_end ))

        # Order memory allocations
        self.order_mem()

        # Show memory allocations
        window_position = self.txt_height + ( window_buffer * 2 )
        for item in self.allocation_lst:
            self.calc_height( item )
            item.width = self.window_width - ( window_buffer * 2 )

            item.position = ( window_buffer, window_position )
            window_position += ( item.height + window_buffer )
            item.update()

        if(( window_position + self.txt_height + window_buffer ) > self.window_height ):
            self.scroll_limit = ( window_position + self.txt_height + window_buffer ) - self.window_height
        else:
            self.scroll_limit = 0

        # Update the "end" info position
        self.canvas.coords( self.end, ( self.window_width / 2 ), ( window_position + ( self.txt_height / 2 )))

class Mem_Rect:
    def __init__( self, canvas, name = "", position = 0, size = 0, color = "white" ):
        self.canvas = canvas
        self.width = 0
        self.height = 0
        self.position = ( 0, 0 )
        self.middle = ( 0, 0 )
        self.name = name
        self.txt_buffer = 2
        self.window_buffer = 8
        self.mem_size = size
        self.mem_position = position
        self.font = "Times 10"
        self.txt_color = "black"
        self.color = color
        self.txt_height = tk.font.Font( font=self.font ).metrics( 'linespace' )

        self.rect = canvas.create_rectangle( self.position[0], self.position[1], self.position[0] + self.width, self.position[1] + self.height, fill=self.color )
        self.text = canvas.create_text( self.middle[0], self.middle[1], fill = self.txt_color, font = self.font, text = self.name )

        self.canvas.tag_bind( self.rect, '<ButtonRelease-1>', self.click )
        self.canvas.tag_bind( self.text, '<ButtonRelease-1>', self.click )

    def click( self, event ):
        print( self.name + ": Clicked!" )

    def set_size( self, size ):
        self.width = size[0]
        self.height = size[1]

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
        self.canvas.itemconfig( self.text, text = self.name + " - " + mem_size_num_to_str( self.mem_size ))
        # Update text position
        self.canvas.coords( self.text, self.middle[0], self.middle[1] )
        # Update text color
        self.canvas.itemconfig( self.text, fill = self.txt_color )
        # Update text font
        self.canvas.itemconfig( self.text, font = self.font )

    def destroy( self ):
        self.canvas.delete( self.text )
        self.canvas.delete( self.rect )

# Convert the length of memory from a string to a number
def mem_size_str_to_num( mem_size_str ):
    if( mem_size_str[len( mem_size_str ) - 1] == "K" ):
        mem_size_num = int( mem_size_str[:len( mem_size_str ) - 1] )
        mem_size_num = mem_size_num * 1024
    elif( mem_size_str[len( mem_size_str ) - 1] == "M" ):
        mem_size_num = int( mem_size_str[:len( mem_size_str ) - 1] )
        mem_size_num = mem_size_num * 1024 * 1024
    elif( mem_size_str[len( mem_size_str ) - 1] == "G" ):
        mem_size_num = int( mem_size_str[:len( mem_size_str ) - 1] )
        mem_size_num = mem_size_num * 1024 * 1024 * 1024
    elif( mem_size_str[len( mem_size_str ) - 1] == "T" ):
        mem_size_num = int( mem_size_str[:len( mem_size_str ) - 1] )
        mem_size_num = mem_size_num * 1024 * 1024 * 1024 * 1024
    else:
        mem_size_num = int( mem_size_str )
    return mem_size_num

# Convert the length of memory from a number to a string
def mem_size_num_to_str( mem_size_num ):
    Terra = ( 1024 * 1024 * 1024 * 1024 )
    Giga = ( 1024 * 1024 * 1024 )
    Mega = ( 1024 * 1024 )
    Kilo = ( 1024 )
    if( mem_size_num >= Terra ):
        mem_size_str = str( mem_size_num / Terra ) + "T"
    elif( mem_size_num >= Giga ):
        mem_size_str = str( mem_size_num / Giga ) + "G"
    elif( mem_size_num >= Mega ):
        mem_size_str = str( mem_size_num / Mega ) + "M"
    elif( mem_size_num >= Kilo ):
        mem_size_str = str( mem_size_num / Kilo ) + "K"
    else:
        mem_size_str = str( mem_size_num )
    return mem_size_str
