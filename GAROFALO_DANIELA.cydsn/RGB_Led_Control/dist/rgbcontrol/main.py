from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.properties import ObjectProperty  # pylint: disable=E0611
from kivy.clock import Clock
from kivy.graphics import Color, Rectangle
from serialport import Board
from kivy.clock import Clock

class BottomLabel(BoxLayout):
    bottom_label = ObjectProperty(None)

    def update_text(self, instance, value):
        self.bottom_label.text = value

class ColorLabel(Label):
    def __init__(self, **kwargs):
        super(ColorLabel, self).__init__(**kwargs)
        self.r = 0
        self.g = 0
        self.b = 0

    def on_size(self, *args):
        self.canvas.before.clear()
        with self.canvas.before:
            Color(self.r, self.g, self.b, 1)
            Rectangle(pos=self.pos, size=self.size)

    def update_background_color(self, r, g, b):
        self.canvas.before.clear()
        with self.canvas.before:
            Color(r, g, b, 1)
            Rectangle(pos=self.pos, size=self.size)
        self.r = r
        self.g = g
        self.b = b

class ColorSlider(BoxLayout):
    color_label = ObjectProperty(None)
    value_slider = ObjectProperty(None)
    value_label = ObjectProperty(None)

    def update_color_label(self, text):
        self.color_label.text = text
    
    def update_color_label_color(self, color):
        self.color_label.color = color

    def on_value_slider(self, instance, value):
        self.value_slider.bind(value=self.update_value_label)

    def update_value_label(self, instance, value):
        self.value_label.text = f'{int(value)}'

    def get_current_value(self):
        return self.value_slider.value


class Container(BoxLayout):
    bottom_label = ObjectProperty(None)
    red_slider = ObjectProperty(None)
    green_slider = ObjectProperty(None)
    blue_slider = ObjectProperty(None)
    color_label = ObjectProperty(None)
    send_button = ObjectProperty(None)
    connection_bar = ObjectProperty(None)

    def __init__(self, **kwargs):
        super(Container, self).__init__(**kwargs)
        self.board = Board()
        self.board.bind(message_string=self.bottom_label.update_text)
        self.board.bind(connected=self.on_connect)
        self.check_connection_event = Clock.schedule_interval(self.pb_update, .05)

    def pb_update(self, dt):
        self.connection_bar.value += 1
        if (self.connection_bar. value >= self.connection_bar.max):
            self.connection_bar.value = 0

    def on_red_slider(self, instance, value):
        self.red_slider.update_color_label("Red")
        self.red_slider.update_color_label_color((1.0,0.0,0.0,1.0))

    def on_green_slider(self, instance, value):
        self.green_slider.update_color_label("Green")
        self.green_slider.update_color_label_color((0.0,1.0,0.0,1.0))

    def on_blue_slider(self, instance, value):
        self.blue_slider.update_color_label("Blue")
        self.blue_slider.update_color_label_color((0.0,0.0,1.0,1.0))

    def on_connect(self, instance, value):
        if (True):
            self.red_slider.disabled = False
            self.green_slider.disabled = False
            self.blue_slider.disabled = False
            self.send_button.disabled = False
        
            self.red_slider.value_slider.bind(value=self.update_bg_color)
            self.green_slider.value_slider.bind(value=self.update_bg_color)
            self.blue_slider.value_slider.bind(value=self.update_bg_color)

            self.check_connection_event.cancel()
            self.connection_bar.value = 100
    
    def update_bg_color(self, instance, value):
        r = self.red_slider.get_current_value()
        g = self.green_slider.get_current_value()
        b = self.blue_slider.get_current_value()
        self.color_label.update_background_color(r/255,g/255,b/255)

    def update_color(self):
        r = self.red_slider.get_current_value()
        g = self.green_slider.get_current_value()
        b = self.blue_slider.get_current_value()
        self.board.write_new_color(r, g, b)
        self.color_label.update_background_color(r/255,g/255,b/255)
        

class RGBLedApp(App):
    '''
    This is the main app class that returns the
    root widget, which is Container.
    '''

    def build(self):
        return Container()


# If we're calling main, start the Kivy App
if __name__ == "__main__":
    RGBLedApp().run()
