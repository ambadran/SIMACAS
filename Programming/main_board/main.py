from sensors import Sensors
from actuators import Actuators
from mpc import MPC
from server import Server
from time import sleep, sleep_ms
import _thread

sensors_dict = {
        'temperature': 0,
        'humidity': 0,
        'soilMoisture': 0,
        'lightIntensity': 0,
        'co2Concentration': 0
        }

actuators_dict = {
       'mechanism1': 0,
       'mechanism2': 0,
       'mechanism3': 0,
       'water1': 0,
       'water2': 0,
       'water3': 0,
       'fertilizer1': 0,
       'fertilizer2': 0,
       'fertilizer3': 0,
       'light1': 50,
       'light2': 50,
       'light3': 50
       }
mpc_dict = {
        'mpcEnabled': False,
        }

def ControllerCore():
    '''
    Abstraction for the First Core:
        - Reads Sensor Data
        - Controls Actuator States
        - Executes MPC control Algorithm
        - Saves Sensor Data Globally and executes Global Actuators States
    '''
    global actuators_dict
    global mpc_dict
    global sensors_dict

    sensors = Sensors()
    actuators = Actuators()
    mpc = MPC(sensors, actuators)

    while True:
        # print(actuators_dict, end=' \r')


        ################# Sensors Reading ##################
        sensors_dict['temperature'] = sensors.temperature
        sensors_dict['humidity'] = sensors.humidity
        sensors_dict['soilMoisture'] = sensors.soil_moisture
        sensors_dict['co2Concentration'] = sensors.CO2
        sensors_dict['lightIntensity'] = sensors.light_intensity
        ####################################################

        print(f"MPC state: {mpc_dict['mpcEnabled']}")
        if mpc_dict['mpcEnabled']:
            ################### MPC ALgorithm ##################
            if not mpc.state:
                mpc.run()
            ####################################################


        else:
            ################ Handling Actuators ################

            # Firstly, stopping MPC algorithm to allow only-user control
            if mpc.state:
                mpc.pause()

            # 1- Handling Axe Position
            while actuators_dict['mechanism1'] != actuators.axis.positions[0]:
                if actuators_dict['mechanism1'] > actuators.axis.positions[0]:
                    actuators.axis.forward(0)
                elif actuators_dict['mechanism1'] < actuators.axis.positions[0]:
                    actuators.axis.backwards(0)

            while actuators_dict['mechanism2'] != actuators.axis.positions[1]:
                if actuators_dict['mechanism2'] > actuators.axis.positions[1]:
                    actuators.axis.forward(1)
                elif actuators_dict['mechanism2'] < actuators.axis.positions[1]:
                    actuators.axis.backwards(1)

            while actuators_dict['mechanism3'] != actuators.axis.positions[2]:
                if actuators_dict['mechanism3'] > actuators.axis.positions[2]:
                    actuators.axis.forward(2)
                elif actuators_dict['mechanism3'] < actuators.axis.positions[2]:
                    actuators.axis.backwards(2)

            # 2- Handling Water / Fertilizer Channels
            actuators.distribution0.r1.value( actuators_dict['water1'] )
            actuators.distribution0.r2.value( actuators_dict['water2'] )
            actuators.distribution0.r3.value( actuators_dict['water3'] )

            actuators.distribution1.r1.value( actuators_dict['fertilizer1'] )
            actuators.distribution1.r2.value( actuators_dict['fertilizer2'] )
            actuators.distribution1.r3.value( actuators_dict['fertilizer3'] )

            # 3- Handling LED Strips PWM
            actuators.led_strip0.duty( actuators_dict['light1'] )
            actuators.led_strip1.duty( actuators_dict['light2'] )
            actuators.led_strip2.duty( actuators_dict['light3'] )
        ####################################################

        sleep(0.5)

def ServerCore():
    '''
    Abstraction for the Second Core
        - Host Web App (HTML file including the CSS and JAVASCRIPT)
        - Serve the HTML GET and POST requests
        - Updates internal variables to control Actuators through other Core
        - Updates the real-time Sensor values displayed on the Web App
    '''
    global actuators_dict
    global mpc_dict
    global sensors_dict

    server = Server()

    while True:
        # Send Sensor Data
        server.sensors_dict.update(sensors_dict)

        # Host Server
        server.wait_for_client()
        server.handle_html_request(server.identify_html_request())

        # Read User Actuator Controls
        actuators_dict.update(server.actuators_dict)

        # Read MPC Mode
        mpc_dict.update(server.mpc_dict)

        # Toggle LED for User
        server.led.toggle()

        print('\n')


#### Running Both Cores ####
def main():
    sleep(5)
    _thread.start_new_thread(ControllerCore, ())
    sleep(2)
    ServerCore()

# Main Routine
sleep(5)
_thread.start_new_thread(ControllerCore, ())
sleep(2)
ServerCore()


