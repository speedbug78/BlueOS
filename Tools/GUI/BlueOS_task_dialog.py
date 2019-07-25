# Need to add task memory requirements
# Add task resources
# Add task communications (task/task, task/os)

import tkinter

class Mbox(object):

    root = None

    def __init__( self, msg, callback = None ):
        """
        msg = <str> the message to be displayed
        dict_key = <sequence> (dictionary, key) to associate with user input
        (providing a sequence for dict_key creates an entry for user input)
        """
        tki = tkinter
        self.top = tki.Toplevel( Mbox.root )
        self.top.title(msg)
        if( callback != None ):
            self.callback = callback
        else:
            self.callback = None

        frm = tki.Frame( self.top, borderwidth=4, relief='ridge' )
        frm.pack( fill='both', expand=True )

        label2 = tki.Label( frm, text="Task Name" )
        label2.pack( padx=4, pady=4 )

        self.entry1 = tki.Entry(frm)
        self.entry1.pack( pady=4 )
        self.entry1.insert( "end", "Task" )

        label3 = tki.Label( frm, text="Task Duration (ms)" )
        label3.pack( padx=4, pady=4 )

        self.entry2 = tki.Entry(frm)
        self.entry2.pack( pady=4 )
        self.entry2.insert( "end", "2" )

        label4 = tki.Label( frm, text="Task Period (ms)" )
        label4.pack( padx=4, pady=4 )

        self.entry3 = tki.Entry( frm )
        self.entry3.pack( pady=4 )
        self.entry3.insert( "end", "10" )

        label5 = tki.Label( frm, text="RAM Allocation (K)" )
        label5.pack( padx=4, pady=4 )

        self.entry4 = tki.Entry( frm )
        self.entry4.pack( pady=4 )
        self.entry4.insert( "end", "10" )

        label6 = tki.Label( frm, text="Flash Allocation (K)" )
        label6.pack( padx=4, pady=4 )

        self.entry5 = tki.Entry( frm )
        self.entry5.pack( pady=4 )
        self.entry5.insert( "end", "10" )

        b_submit = tki.Button(frm, text='Submit')
        b_submit['command'] = lambda: self.entry_to_dict()
        b_submit.pack()

        b_cancel = tki.Button(frm, text='Cancel')
        b_cancel['command'] = self.top.destroy
        b_cancel.pack(padx=4, pady=4)

    def entry_to_dict( self ):
        d = {}
        d[self.entry1.get()] = {"Duration" : self.entry2.get(), "Period" : self.entry3.get(), "RAM" : self.entry4.get(), "FLASH" : self.entry5.get()}
        if( self.callback != None ):
            self.callback( d )
        self.top.destroy()
