import BlueOS_schedule
import BlueOS_timeline
import BlueOS_support
import random

# Container class to hold task related information
class Tasks:
    def __init__( self, canvas ):
        self.task_info = {}
        self.task_schedule = []
        self.timeline_block = BlueOS_timeline.Timeline_Block( canvas )

    def update_schedule( self ):
        self.task_schedule = BlueOS_schedule.calc_schedule( self.task_info )
        self.timeline_block.draw_timeline( self.task_schedule, self.task_info )

    def add_task( self, name, info ):
        self.task_info[name] = info
        color = self.random_color()
        self.task_info[name]["color"] = color
        self.update_schedule()
        ram_size = self.task_info[name]["RAM"]

        flash_size = self.task_info[name]["FLASH"]

        BlueOS_support.flash_block.add_item ( name, 0, flash_size + "K", color )
        BlueOS_support.ram_block.add_item ( name, 0, ram_size + "K", color )
        BlueOS_support.flash_block.update()
        BlueOS_support.ram_block.update()

    def random_color( self ):
        rcolor =  ( random.randint( 0, 255 ), random.randint( 0, 255 ), random.randint( 0, 255 ))
        return "#%02x%02x%02x" % rcolor
