def draw_mem():
    c_width = int( w.Canvas2.cget( "width" ))
    c_height = int( w.Canvas2.cget( "height" ))
    print( c_width )
    box_start = c_width * 0.05
    box_end = c_width * 0.95
    mem_title = w.Canvas2.create_text(( c_width / 2 ), 10, fill = "black", font = "Times 10", text = "Flash" )
    mem1 = w.Canvas2.create_rectangle( box_start, ( c_height * 0.1 ), box_end, ( c_height * 0.2 ), fill="blue" )

def resize():



    c_width = int( w.Canvas2.cget( "width" ))
    c_height = int( w.Canvas2.cget( "height" ))
    w.Canvas2.coords( mem1, ( c_width * 0.05 ), ( c_height * 0.1 ), ( c_width * 0.95 ), ( c_height * 0.2 ))


    c_width = int( w.Canvas2.cget( "width" ))
    c_height = int( w.Canvas2.cget( "height" ))
    print( c_width )
    box_start = c_width * 0.05
    box_end = c_width * 0.95
    mem_title = w.Canvas2.create_text(( c_width / 2 ), 10, fill = "black", font = "Times 10", text = "Flash" )
    mem1 = w.Canvas2.create_rectangle( box_start, ( c_height * 0.1 ), box_end, ( c_height * 0.2 ), fill="blue" )
